#include "ts.h"

struct tsdev *ts;

struct tsdev * init_ts()
{
	//1.打开触摸屏设备文件  ts_open()
	ts = ts_open("/dev/event0", 0);
	//2.配置触摸屏 ts_config()
	ts_config(ts);
	return ts;
}

int ts_count = 0;

int get_ts(int *x ,int *y)
{
	struct ts_sample sample;

	//3.读取触摸屏参数 ts_read()
	ts_read(ts, &sample, 1); //这里会有一个阻塞

	printf("x = %d, y = %d, pressure = %d\n", sample.x, sample.y, sample.pressure);

	if (sample.pressure == 255)
	{
		ts_count++;
		if(ts_count == 3) 
		{
			*x = sample.x;
			*y=sample.y;
			ts_count = 0;
			printf("%d\n", sample.tv.tv_sec);
			return -1;
		}
	}
	return 0;

}

void close_ts()
{
	//4.关闭触摸屏设备文件 ts_close()
	ts_close(ts);
}