
#ifndef _GAME_OBJ_H
#define _GAME_OBJ_H

#include "display.h"

typedef enum {
    NEUTRAL,
    UP,
    DOWN,
    RIGHT,
    LEFT
} dir_t;

typedef struct {
    rect_t pos;
    dir_t dir;
    int speed;
    int color;
} object_t;


void objects_init();


#endif
