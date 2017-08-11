#ifndef  _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <pthread.h>
#include "lcd.h"

#define BMP_FILE_NAME_LEN 25
#define BMP_LIST_SIZE 10
#define MUSIC_FILE_NAME_LEN 25
#define MUSIC_LIST_SIZE 10

extern char bmp_list[BMP_LIST_SIZE][BMP_FILE_NAME_LEN] ;
extern int bmp_list_num ;

extern char music_list[MUSIC_LIST_SIZE][MUSIC_FILE_NAME_LEN];
extern int music_list_num;

#endif