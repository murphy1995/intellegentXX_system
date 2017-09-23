#ifndef  _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <pthread.h>
#include "lcd.h"
#include "socket.h"
#include "handle_protcol.h"
#include "serial.h"

#define BMP_FILE_NAME_LEN 25
#define BMP_LIST_SIZE 10
#define MUSIC_FILE_NAME_LEN 25
#define MUSIC_LIST_SIZE 10

extern char bmp_list[BMP_LIST_SIZE][BMP_FILE_NAME_LEN] ;
extern int bmp_list_num ;

extern char music_list[MUSIC_LIST_SIZE][MUSIC_FILE_NAME_LEN];
extern int music_list_num;

extern int socket_file_fd;
extern int socket_wechat_fd ;

extern int TEMP;
extern int HUMI;

char number_HUMI[10][12] = {"./ui/b0.bmp","./ui/b1.bmp","./ui/b2.bmp","./ui/b3.bmp","./ui/b4.bmp","./ui/b5.bmp","./ui/b6.bmp","./ui/b7.bmp","./ui/b8.bmp","./ui/b9.bmp"};
char number_TEMP[10][13] = {"./ui/y0.bmp","./ui/y1.bmp","./ui/y2.bmp","./ui/y3.bmp","./ui/y4.bmp","./ui/y5.bmp","./ui/y6.bmp","./ui/y7.bmp","./ui/y8.bmp","./ui/y9.bmp"};

#endif