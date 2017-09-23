#include "main.h"

#define STOP 	0
#define PLAY 	1
#define PAUSE 2
#define BASE_VOLUME 106

int x = 0;
int y = 0;

void *ts_thread(void *arg)
{
	while(1)
	{
		get_ts(&x ,&y);
	}
}

void *recv_we_thread(void * arg)
{

	char msg_buff[8] = {0};
	protocol * msg_buff_p = (protocol *)msg_buff;
	while(1){
		recv_wechat(msg_buff,8);
		msg_buff_p = (protocol *)msg_buff;
		int val = protocol_test(msg_buff_p);
		if(val == 1)
			{
				write_serial("11");
				write_serial("21");

			}
			else if (val == 0)
			{
				write_serial("33");
			}
	}

}


int gallery()
{
	find_bmp("./photo");
	int bmp_loc = 0;
	show_bmp(bmp_list[bmp_loc++]);
	//show_bmp_overlay("./ui/touming.bmp",0 ,280);
	while(1)
	{
		//图片触摸切换
		if(x >=0 && x<= 100 && y >=190 && y<=290 )
		{
			x = 0;
			y = 0;
			if (bmp_loc == bmp_list_num)
			{
				bmp_loc = 0;
			}
			printf("%s\n",bmp_list[bmp_loc] );
			show_bmp(bmp_list[bmp_loc++]);
		}
		else if(x >=700 && x<= 800 && y >=190 && y<=290 )
		{
			x = 0;
			y = 0;
			if (bmp_loc == -1)
			{
				bmp_loc = bmp_list_num-1;
			}
			printf("%s\n",bmp_list[bmp_loc] );
			show_bmp(bmp_list[bmp_loc--]);
		}
		else if (x >=350 && x<= 450 && y >=380 && y<=480)
		{
			x = 0;
			y = 0;
			//图片幻灯片播放
			while(1)
			{
				if(bmp_loc==bmp_list_num)
				{
					bmp_loc=0;
				}
				printf("%s\n",bmp_list[bmp_loc] );
				show_bmp(bmp_list[bmp_loc]);
				bmp_loc++;
				if (x && y)
				{
					break;
				}
				
				sleep(1);
			}
		}
		else if(x >=1 && x<= 50 && y >=1 && y<=50 )
		{
			break;
		}
		
	 }
	 return 0;
}

int music()
{
	show_bmp("./ui/music_play.bmp");
	find_music("./music");
	int music_loc = 0;
	int music_state = STOP;
	printf("enter music\n");

	int volume = BASE_VOLUME;
	while(1)
	{
		//播放键操作
		if(x >=360 && x<= 445 && y >=376 && y<=460 )
		{
			x = 0;
			y = 0;
			int flag = 0;
			//状态为未播放
			if(music_state == STOP)
			{
				show_bmp("./ui/music_stop.bmp");
				play_music(music_list[music_loc]);
				music_state = PLAY;
				printf("play begin !\n");
			}
			//状态为暂停
			else if(music_state == PAUSE && flag == 0)
			{
				show_bmp("./ui/music_stop.bmp");
				continue_music(music_list[music_loc]);
				music_state = PLAY;
				flag = 1;
				printf("replay music\n");
				continue;
			}

			//状态为播放
			else if(music_state == PLAY && flag == 0)
			{
				show_bmp("./ui/music_play.bmp");
				pause_music();
				printf("pause music\n");
				music_state = PAUSE;
			}
		}
		//左键操作
		else if(x >=225 && x<= 305 && y >=376 && y<=460)
		{
			x = 0;
			y = 0;
			music_loc--;
			if (music_loc == -1)
			{
				music_loc = music_list_num;
			}
			play_music(music_list[music_loc]);
			if(music_state == STOP ||music_state == PAUSE)
			{
				show_bmp("./ui/music_play.bmp");
				music_state = PLAY;
			}
			printf("last song\n");
		}
		//右键操作
		else if(x >=503 && x<= 582 && y >=376 && y<=460)
		{
			x = 0;
			y = 0;
			music_loc++;
			if (music_loc == music_list_num)
			{
				music_loc = 0;
			}
			play_music(music_list[music_loc]);
			if(music_state == STOP ||music_state == PAUSE)
			{
				show_bmp("./ui/music_play.bmp");
				music_state = PLAY;
			}
			printf("next song\n");
		}
		//降低音量操作
		else if(x >=70 && x<= 110 && y >=400 && y<=445 )
		{
			x = 0;
			y = 0;
			volume--;
			volume_ctrl_music(volume);
		}
		//提高音量操作
		else if(x >=690 && x<= 730 && y >=400 && y<=445 )
		{
			x = 0;
			y = 0;
			volume++;
			volume_ctrl_music(volume);
		}
		//退出操作
		else if(x >=1 && x<= 50 && y >=1 && y<=50 )
		{
			x = 0;
			y = 0;
			printf("quit music!\n");
			stop_music();
			break;
		}
	}
	return 0;
}

int record()
{
	while(1)
	{
		show_bmp("./ui/record.bmp");
		if(x >=273 && x<= 526 && y >=54 && y<=309 )
		{
			x = 0;
			y = 0;
			//1. 录音
			system("arecord -d3 -c1 -r16000 -twav -fS16_LE example.wav &");
			show_bmp("./ui/record0.bmp");
			sleep(1);
			show_bmp("./ui/record1.bmp");
			sleep(1);
			show_bmp("./ui/record2.bmp");
			sleep(1);
			show_bmp("./ui/record3.bmp");
			sleep(1);
			//3. 发送录音的文件
			send_file("./example.wav");
			//4.接收服务器发送的识别数据
			int ret = recv_result();
			if(ret == 1)
			{
				write_serial("11");
				write_serial("21");

			}
			else if (ret == -1)
			{
				write_serial("33");
			}
			//close_file_socket();
		}
		else if(x >=1 && x<= 50 && y >=1 && y<=50 )
		{
			x = 0;
			y = 0;
			printf("quit record!\n");
			break;
		}
	}
	
	//4. 关闭socket
	close_file_socket();
	return 0;
}

int enviroment()
{
	show_bmp("./ui/enviroment.bmp");
	protocol * pro =  create_pro();
	show_bmp_overlay(number_TEMP[0], 75 , 140);
	show_bmp_overlay(number_TEMP[0], 115 , 140);
	show_bmp_overlay(number_HUMI[0], 460 , 137);
	show_bmp_overlay(number_HUMI[0], 500 , 137);
	printf("enter enviroment!\n");
	while(1)
	{
		if(x >=1 && x<= 50 && y >=1 && y<=50 )
		{
			x = 0;
			y = 0;
			printf("quit enviroment!\n");
			break;
		}
		if(read_serial() == 0)
		{
			show_bmp_overlay(number_TEMP[TEMP/10], 75 , 140);
			show_bmp_overlay(number_TEMP[TEMP%10], 115 , 140);
			show_bmp_overlay(number_HUMI[HUMI/10], 460 , 137);
			show_bmp_overlay(number_HUMI[HUMI%10], 500 , 137);
			//发送温度数据 5s
			putData(pro,WX_APP,WX_APP_T,0,0,TEMP);
			host2net(pro);
			send_wechat(pro,8);
			
			//发送湿度数据 
			
			putData(pro,WX_APP,WX_APP_H,0,0,HUMI);
			host2net(pro);
			send_wechat(pro,8);
		}
		
		//printf("test3\n");
	}
	close_serial();
	
	return 0;
}

int main(int argc, char const *argv[])
{
	init_ts();
	init_lcd();
	
	pthread_t ts_thread_id;
	pthread_create(&ts_thread_id, NULL,ts_thread, NULL);
	
	pthread_t recv_we_thread_id;
	pthread_create(&recv_we_thread_id, NULL,recv_we_thread, NULL);
	
	// pthread_t send_we_thread_id;
	// pthread_create(&send_we_thread_id, NULL,send_we_thread, NULL);
		
	show_bmp("./ui/open1.bmp");
	sleep(1);
	show_bmp("./ui/open2.bmp");
	sleep(1);
	show_bmp("./ui/open3.bmp");
	sleep(1);
	while(1)
	{
		show_bmp("./ui/01.bmp");
		if(x >=190 && x<= 295 && y >=205 && y<=287 )
		{
			x = 0;
			y = 0;
			gallery();
		}
		else if(x >=345 && x<= 455 && y >=203 && y<=293)
		{
			x = 0;
			y = 0;
			record();
		}
		else if(x >=348 && x<= 455 && y >=325 && y<=416)
		{
			x = 0;
			y = 0;
			music();
			
		}
		else if(x >=499 && x<= 603 && y >=76 && y<=166)
		{
			x = 0;
			y = 0;
			enviroment();
			
		}
	}
	
	close_ts();
	close_lcd();
	return 0;
}