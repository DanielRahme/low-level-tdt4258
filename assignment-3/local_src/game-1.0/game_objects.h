
#ifndef _GAME_OBJ_H
#define _GAME_OBJ_H

#include "display.h"


enum player_speed {
    PLAYER_NEUTRAL_SPEED    = 0,
    PLAYER_UP_SPEED         = 1,
    PLAYER_DOWN_SPEED       = -1,
};


// Buttons from gamepad
typedef enum {
    PLAYER_1_UP,
    PLAYER_1_DOWN,
    PLAYER_2_UP,
    PLAYER_2_DOWN,
    PLAYER_NEUTRAL
} player_btn_t;


// Direction and speed on X and Y axis
typedef struct {
    int x;
    int y;
} velocity_t;


// Objects: Player rectangles and ball
typedef struct {
    rect_t pos;
    velocity_t velocity;
    int color;
} object_t;


// Functions
void objects_init();

void redraw_players();
void update_players_pos(player_btn_t btn_p1, player_btn_t btn_p2);

void update_ball();


#endif
