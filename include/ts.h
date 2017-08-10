#ifndef __TS_H__
#define __TS_H__

#include <stdio.h>
#include "tslib.h"

int get_ts();
struct tsdev * init_ts();
void close_ts();


#endif