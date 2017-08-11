#include "mp3.h"

char music_list[MUSIC_LIST_SIZE][MUSIC_FILE_NAME_LEN] = {0};
int music_list_num = 0;


int find_music(char * music_path)
{

	music_list_num=0;
	memset(music_list, 0 , sizeof(music_list));
	//打开当前目录

	DIR * dirp;
	dirp = opendir(music_path);
	if(NULL == dirp)
	{
		printf("open %s error\n",music_path);
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

	    if(NULL != strstr(_dir_p->d_name,".mp3"))
	    {
	    	//TODO:越界检测
	    	strcat(music_list[music_list_num],"./music/");
	    	strcat(music_list[music_list_num],_dir_p->d_name);
	    	music_list_num++;
	    }
	    //printf("d_name = %s\n",_dir_p->d_name);
	}
}


int play_music(char * music_name){

	stop_music();
	char cmd_buff[256] = {0};
	sprintf(cmd_buff,"madplay %s &",music_name);
	system(cmd_buff);
	return 0;

}


int stop_music(){

	system("killall -KILL madplay");
	return 0;
}


int pause_music(){

	system("killall -STOP madplay");
	return 0;
}

int continue_music(){

	system("killall -CONT madplay");
	return 0;
}

