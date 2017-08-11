#include "main.h"

#define STOP 	0
#define PLAY 	1
#define PAUSE 2

void *gallery_ppt(void * arg)
{
	int i = 0;
	printf("enter pthread\n");
	while(1)
	{
		int x = 0;
		int y = 0;
		if(get_ts(&x , & y)!=0)
		{
			printf("pthread_exit!\n");
			pthread_exit(0);
		}

		if(i==bmp_list_num)
		{
			i=0;
		}
		printf("%s\n",bmp_list[i] );
		show_bmp(bmp_list[i]);
		i++;

		
		sleep(1);
	}
}

int gallery()
{
	find_bmp("./photo");
	int bmp_loc = 0;

	pthread_t gallery_thread;
	while(1)
	{
		//图片触摸切换
		int x = 0;
		int y = 0;
		get_ts(&x , & y);
		if(x >=0 && x<= 100 && y >=190 && y<=290 )
		{
			if (bmp_loc == bmp_list_num)
			{
				bmp_loc = 0;
			}
			show_bmp(bmp_list[bmp_loc++]);
		}
		else if(x >=700 && x<= 800 && y >=190 && y<=290 )
		{
			if (bmp_loc == -1)
			{
				bmp_loc = bmp_list_num-1;
			}
			show_bmp(bmp_list[bmp_loc--]);
		}
		else if(x >=350 && x<= 450 && y >=380 && y<=480 )
		{
			pthread_create(&gallery_thread, NULL,gallery_ppt, NULL);
			printf("pthread_create!\n");

		}
		else if(x >=350 && x<= 450 && y >=190 && y<=290 )
		{
			break;
		}
		
		//图片幻灯片播放
	// 	while(1)
	// 	{
	// 		int x = 0;
	// 		int y = 0;
	// 		//get_ts(&x , & y);
			
	// 		if(i==bmp_list_num)
	// 		{
	// 			i=0;
	// 		}
	// 		printf("%s\n",bmp_list[i] );
	// 		show_bmp(bmp_list[i]);
	// 		i++;

			
	// 		sleep(1);
	// 	}
	 }
}

int music()
{
	show_bmp("./ui/music_play.bmp");
	find_music("./music");
	int music_loc = 0;
	int music_state = STOP;
	printf("enter music\n");
	while(1)
	{
		int x = 0;
		int y = 0;
		get_ts(&x , &y);
		//播放键操作
		if(x >=360 && x<= 445 && y >=376 && y<=460 )
		{
			//状态为未播放
			if(music_state == STOP)
			{
				show_bmp("./ui/music_stop.bmp");
				play_music(music_list[music_loc]);
				music_state = PLAY;
				printf("play begin !\n");
			}
			//状态为播放
			else if(music_state == PLAY)
			{
				show_bmp("./ui/music_play.bmp");
				pause_music();
				printf("pause music\n");
				music_state = PAUSE;
			}
			//状态为暂停
			else if(music_state == PAUSE);
			{
				show_bmp("./ui/music_stop.bmp");
				continue_music(music_list[music_loc]);
				music_state = PLAY;
				printf("replay music\n");
			}
		}
		//左键操作
		else if(x >=225 && x<= 305 && y >=376 && y<=460)
		{
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
		//退出操作
		else if(x >=1 && x<= 50 && y >=1 && y<=50 )
		{
			printf("quit music!\n");
			stop_music();
			break;
		}
	}
}

int main(int argc, char const *argv[])
{
	init_ts();
	
	while(1)
	{
		show_bmp("./ui/01.bmp");
		int x = 0;
		int y = 0;
		get_ts(&x , &y);
		if(x >=190 && x<= 295 && y >=205 && y<=287 )
		{
			gallery();
		}
		else if(x >=345 && x<= 455 && y >=325 && y<=416)
		{
			music();
		}

	}
	
	close_ts();
	return 0;
}