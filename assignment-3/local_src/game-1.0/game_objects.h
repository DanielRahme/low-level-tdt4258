
#ifndef _GAME_OBJ_H
#define _GAME_OBJ_H

#include "display.h"


enum player_speed {
    PLAYER_NEUTRAL_SPEED    = 0,
    PLAYER_UP_SPEED         = -5,
    PLAYER_DOWN_SPEED       = 5,
};


// Buttons from gamepad
typedef enum {
    PLAYER_1_UP = (1 << 1),
    PLAYER_1_DOWN = (1 << 3),
    PLAYER_2_UP = (1 << 5),
    PLAYER_2_DOWN = (1 << 7),
    PLAYER_NEUTRAL = 0
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
void update_players(unsigned char buttons);

void update_ball();
int ball_is_alive();
int get_winner();

void start_screen();
void screen_winner_p1();
void screen_winner_p2();


#endif
