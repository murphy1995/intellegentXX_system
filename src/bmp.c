#include "bmp.h"

#define BMP_W_800 800
#define BMP_H_480 480
#define BMP_PIX_SIZE 3
//bmp 解析相关的代码
#define BMP_H_INFO 54
#define BMP_FILE_NAME_LEN 25
#define BMP_LIST_SIZE 10

char bmp_list[BMP_LIST_SIZE][BMP_FILE_NAME_LEN] = {0};
int bmp_list_num = 0;


int find_bmp(char * bmp_path)
{

	bmp_list_num=0;
	memset(bmp_list , 0 , sizeof(bmp_list));
	//打开当前目录

	DIR * dirp;
	dirp = opendir(bmp_path);
	if(NULL == dirp)
	{
		printf("open %s error\n",bmp_path);
		return -1;
	}

	//读取目录
	struct dirent *_dir_p;
	while(1)
	{
	    _dir_p = readdir(dirp);
	    if(_dir_p == NULL)
	    {
	    	closedir(dirp);
	    	printf("_dir_p == NULL!\n");
	    	return 0;
	    }

	    if(NULL != strstr(_dir_p->d_name,".bmp"))
	    {
	    	//TODO:越界检测
	    	strcat(bmp_list[bmp_list_num],"./photo/");
	    	strcat(bmp_list[bmp_list_num],_dir_p->d_name);
	    	bmp_list_num++;
	    }
	    //printf("d_name = %s\n",_dir_p->d_name);
	}
}

int proc_bmp_800_480(char *bmp_path,unsigned int * lcd_buff)
{

	//打开图片
	int fd_bmp = open(bmp_path,O_RDONLY);
	if(-1 == fd_bmp)
	{
		printf("open %s error\n",bmp_path);
		return -1;
	}

	//偏移过文件头
	int ret = lseek(fd_bmp,54,SEEK_SET);
	if(-1 == ret)
	{
		printf("lseek error!\n");
		return -1;
	}

	//读数据
	char buff[BMP_W_800 * BMP_H_480 * BMP_PIX_SIZE] = {0};
	read(fd_bmp, buff, BMP_W_800 * BMP_H_480 * BMP_PIX_SIZE);

	//RGB信息 转换为 ARGB信息
	int i = 0;
	int j = 0;
	for(i = 0 ;i < BMP_H_480; i++) 
	{
		for(j = 0 ;j < BMP_W_800; j++)
		{
			lcd_buff[(BMP_H_480 - 1 - i)*BMP_W_800 + j] = 0x0 <<24 |buff[(i*BMP_W_800+j)*BMP_PIX_SIZE] << 0 | \
										buff[(i*BMP_W_800+j)*BMP_PIX_SIZE + 1] <<8 | \
										buff[(i*BMP_W_800+j)*BMP_PIX_SIZE + 2] << 16;
		}
	}

	close(fd_bmp);

	return 0;
}


int get_bmp_w_h(char * bmp_info_buff,unsigned int *w,unsigned int *h){

	//1. 检查传入的指针是否为空
	if(bmp_info_buff == NULL || w == NULL || h == NULL){
		printf("pos NULL\n");
		return -1;
	}
	//2. 偏移到图片的宽度信息(14+4 字节)
	//3. 偏移到图片的高度信息
	//获取宽度信息,转换大小端（bmp格式是大端，linux是小端）
	unsigned int _bmp_w = *(unsigned int *)(bmp_info_buff + 18);   //18 偏移到图片的宽度信息的偏移量
	unsigned int _bmp_h = *(unsigned int *)(bmp_info_buff + 18 + 4); //4 偏移过bmp的宽度信息
	
	//printf("get_bmp_w_h: w = %d, h = %d\n",_bmp_w, _bmp_h);
	
	//4. 赋值给*w，以及*h
	*w = _bmp_w;
	*h = _bmp_h;

	return 0;
}

int proc_bmp_not_800_480(char *bmp_path,unsigned int * lcd_buff,unsigned int x,unsigned int y)
{

	//1. 打开图片
	int fd_bmp = open(bmp_path,O_RDONLY);
	if(-1 == fd_bmp)
	{
		printf("open %s error\n",bmp_path);
		return -1;
	}



	//2. 获取图片的长宽
	char bmp_info_buff[BMP_H_INFO] = {0};
	read(fd_bmp, bmp_info_buff,BMP_H_INFO);
	
	unsigned int bmp_w = 0; 
	unsigned int bmp_h = 0;
	get_bmp_w_h(bmp_info_buff,&bmp_w,&bmp_h);

	//3. 偏移过bmp头信息
	int ret = lseek(fd_bmp,54,SEEK_SET);
	if(-1 == ret)
	{
		printf("lseek error!\n");
		return -1;
	}

	//读取图片信息
	int fill_zero_line = 4 - (bmp_w*BMP_PIX_SIZE % 4);
	if (fill_zero_line == 4)
	{
		fill_zero_line = 0;
	}

	char buff[BMP_W_800 * BMP_H_480 * BMP_PIX_SIZE] = {0};
	read(fd_bmp, buff,bmp_w * bmp_h * BMP_PIX_SIZE +bmp_h * fill_zero_line);

	unsigned int _bmp_w = 0;
	unsigned int _bmp_h = 0;
	if(bmp_h + y > BMP_H_480){
		_bmp_h = BMP_H_480 - y;
	}
	else{
		_bmp_h = bmp_h;
	}

	if(bmp_w + x > BMP_W_800){
		_bmp_w = BMP_W_800 - x;
	}
	else{
		_bmp_w = bmp_w;
	}
	//4. 在对应的x，y坐标上刷入图片信息
	int i = 0;
	int j = 0;
	for(i = 0 ;i < _bmp_h; i++) 
	{
		for(j = 0 ;j < _bmp_w; j++)
		{
			// lcd_buff[(BMP_H_480-i-y-1)*BMP_W_800 + (BMP_W_800-j-x-1)] = 0x0 <<24 |buff[(i*bmp_w+j)*BMP_PIX_SIZE +i*fill_zero_line] << 0 | \
			// 				buff[(i*bmp_w+j)*BMP_PIX_SIZE +i*fill_zero_line+ 1] <<8 | \
			// 							buff[(i*bmp_w+j)*BMP_PIX_SIZE +i*fill_zero_line+ 2] << 16;
			lcd_buff[(_bmp_h - i+y)*BMP_W_800 + (j+x)] = 0x0 <<24 |buff[(i*bmp_w+j)*BMP_PIX_SIZE + i*fill_zero_line  ] << 0 | \
										buff[(i*bmp_w+j)*BMP_PIX_SIZE + i*fill_zero_line + 1] <<8 | \
										buff[(i*bmp_w+j)*BMP_PIX_SIZE + i*fill_zero_line + 2] << 16;
			
		}
	}

	printf("proc_bmp_not_800_480 success\n");
	//5. 关闭图片
	close(fd_bmp);

	return 0;
}