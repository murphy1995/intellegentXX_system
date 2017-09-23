#ifndef __MP3_H__
#define __MP3_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
int play_music(char * music_name);
int stop_music();
int pause_music();
int continue_music();
int volume_ctrl_music(int volume);

#define MUSIC_FILE_NAME_LEN 25
#define MUSIC_LIST_SIZE 10

#endif