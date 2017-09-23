#include "socket.h"
#include "handle_protcol.h"


int socket_file_fd = -1;
int  socket_file_port = 23333;

int socket_init(int *socket_fd, int port){
	//1. 创建socket
	*socket_fd = socket(AF_INET,SOCK_STREAM,0);

    	//2. 构造服务器地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port =  htons(port);
	addr.sin_addr.s_addr = inet_addr(SERVICE_IP);

	printf("start to connect %s \n",SERVICE_IP);
	//3. 连接服务器
	int ret = connect(*socket_fd, (struct sockaddr *) &addr,sizeof(struct sockaddr));
	if(-1 == ret){
		printf("connect failed!\n");
		return -1;
	}
	printf("connect!\n");
	return 0;
}

/****************************
用以发送语音文件和接收识别信息的程序
***************************/

//发送文件
int send_file(char *file_path)
{
	if (socket_file_fd == -1)
	{
		socket_init(&socket_file_fd,socket_file_port);
	}
	FILE *file_p = fopen(file_path,"r");
	if(file_p ==  NULL){
		printf("open file error!\n");
	}

	int file_size = 0;
	fseek(file_p,0,SEEK_END);//将文件指针偏移到文件末尾
	file_size = ftell(file_p);//获取当前的偏移量（偏移量就是文件大小）
	fseek(file_p,0,SEEK_SET);//将文件指针偏移到文件开头

	

	//5. 发送文件大小
	char send_buff[2048] = {0};
	int len = 0;
	sprintf(send_buff, "%d\n", file_size);
	len = send(socket_file_fd,send_buff,2048,0);

	//6. 读取文件内容
	//7. 发送文件内容
	
	bzero(send_buff,sizeof(send_buff));
	int total = 0;
	while((len = fread(send_buff, sizeof(char),sizeof(send_buff),file_p)) > 0)
	{

		int ret = send(socket_file_fd,send_buff,len,0);
		total += ret;
		printf("send:%d/%d/%d\n",len,total,file_size );
		bzero(send_buff,sizeof(send_buff));
	}
	printf("send_file success!\n");
	fclose(file_p);
	return 0;

}

const char *lantern[2]={"开灯","关灯"};
//接收识别结果
int recv_result()
{
	char msg_buff[2048] = {0};
	if(socket_file_fd == -1){
		socket_init(&socket_file_fd,socket_file_port);
	}
	printf("waiting  recv_result\n");
	int msg_len = recv(socket_file_fd,msg_buff,sizeof(msg_buff),0);
	printf("===============================\n");
	printf("%s\n", msg_buff);
	//TODO:对结果进行处理
	//第1种方式： 找一个xml的解析库
	//第2种方式： strstr()
	if(strstr((const char *)msg_buff , lantern[0]) != NULL)
	{
		printf("%s\n",lantern[0] );
		printf("===============================\n");
		return 1;
	}
	if(strstr((const char *)msg_buff , lantern[1]) != NULL)
	{
		printf("%s\n",lantern[1] );
		printf("===============================\n");
		return -1;
	}	
	
	return 0;
}
//关闭socket

int close_file_socket(){

	//8. 关闭socket，关闭文件
	close(socket_file_fd);
	socket_file_fd = -1;
	return 0;
}

/****************************
用以接收和发送给小程序的socket
***************************/

int socket_wechat_fd = -1;
int socket_wechat_port = 24444;

int send_wechat(char *data , unsigned int len)
{
	if (socket_wechat_fd == -1)
	{
		socket_init(&socket_wechat_fd, socket_wechat_port);
	}
	char connect_flag;
	printf("point1\n");
	// if(recv(socket_wechat_fd, &connect_flag, sizeof(char), 0) <= 0)
	// {
	// 	close_wechat_socket();
	// 	sleep(2);
	// 	return -1;
	// }
	printf("point2\n");
	send(socket_wechat_fd, data , len , 0);
	printf("point3\n");
	return 0;
}

int recv_wechat(char *msg_buff , unsigned int len)
{
	if (socket_wechat_fd == -1)
	{
		socket_init(&socket_wechat_fd, socket_wechat_port);
	}
	//6. 收数据
	ssize_t msg_len = 0;
	msg_len = recv(socket_wechat_fd, msg_buff, len, 0);
	if (msg_len <= 0)
	{
		close_wechat_socket();
		sleep(2);
		return -1;
	}
	printf("[wx recv]:%s\n", msg_buff);//可能越界
	return 0;
}
int close_wechat_socket()
{
	close(socket_wechat_fd);
	socket_wechat_fd = -1;
	return 0;
}