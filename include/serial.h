#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

int read_serial();
int close_serial();


#endif

