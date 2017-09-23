#include "serial.h"

int serial_fd = -1;
struct termios termios_old;//为了在后面进行关闭串口的时候，恢复原来的设置
struct termios termios_current;//为了在原来的基础上改变串口的设置（属性）

int init_serial()
{
		//1. 打开串口设备
	serial_fd = open("/dev/s3c2410_serial3",O_RDWR);
	if(serial_fd == -1){
		printf("main:open serial error!\n");
	}


	//2.设置串口
	
	//获取串口属性
	
	tcgetattr(serial_fd, &termios_old);
	tcgetattr(serial_fd, &termios_current);


	//设置位raw模式
	cfmakeraw(&termios_current);
	//设置波特率
	cfsetspeed(&termios_current,B115200);

	//设置bit位 8位
	termios_current.c_cflag &= ~CSIZE;//11 0000
	termios_current.c_cflag |= CS8; //11 0000

	//设置停止位
	termios_current.c_cflag &= ~CSTOPB;//

	//关闭奇偶校验
	termios_current.c_cflag &= ~PARENB;

	termios_current.c_cflag &= ~CRTSCTS;

	//printf("test\n");
	return 0;

}

int TEMP = 0;
int HUMI = 0;

int read_serial()
{
	if(-1 == serial_fd){
		init_serial();
	}
	tcflush(serial_fd,TCIOFLUSH);

	termios_current.c_cc[VTIME] = 0;
	termios_current.c_cc[VMIN] = 1;

	tcsetattr(serial_fd,TCSANOW,&termios_current);
	char buff[7] = {0};
	buff[6] = '\0';
	int  ret  = -1;
	ret = read(serial_fd,buff,6);
	//printf("test2\n");
	if(ret == -1){
		printf("read error ! errno = %d\n",ret );
		printf("%d,%d,%d,%d,%d,%d,%d\n", EACCES,EDQUOT,EEXIST,EFAULT,EFBIG,EINTR,EINVAL);
	}
	if (buff[0] != 'T')
	{
		return -1;
	}
	TEMP = (buff[1] - '0')*10 + (buff[2] - '0');
	HUMI = (buff[4] - '0')*10 + (buff[5] - '0');
	printf("[serial]%s!\n", buff);
	sleep(1);
	return 0;
}
int write_serial(char *com)
{
	if(-1 == serial_fd){
		init_serial();
	}
	tcflush(serial_fd,TCIOFLUSH);

	termios_current.c_cc[VTIME] = 0;
	termios_current.c_cc[VMIN] = 1;

	tcsetattr(serial_fd,TCSANOW,&termios_current);
	write(serial_fd, com, 6);
	printf("write_serial success\n");
       	sleep(1);
}

int close_serial()
{
	//4. 关闭串口
	tcsetattr(serial_fd, TCSANOW,&termios_old);
	close(serial_fd);
	serial_fd = -1;
	
	return 0;
}

