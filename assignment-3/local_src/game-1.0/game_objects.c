
#include "display.h"
#include "game_objects.h"

#define SCREEN_X 320
#define SCREEN_Y 240

static object_t player_1 = {};
static object_t player_2 = {};
static object_t ball = {};


void objects_init()
{
    // X, Y, Width, Height
    rect_t tmp = {0, 80, 32, 80};
    player_1.pos = tmp;
    player_1.dir = NEUTRAL;
    player_1.color = RED;
    display_draw_rect(player_1.pos, player_1.color);


    tmp.x = SCREEN_X - 32;
    player_2.pos = tmp;
    player_2.dir = NEUTRAL;
    player_2.color = BLUE;
    display_draw_rect(player_2.pos, player_2.color);

    rect_t tmp2 = {SCREEN_X/2, SCREEN_Y/2, 16, 16};
    ball.pos = tmp2;
    ball.dir = RIGHT;
    ball.color = WHITE;
    display_draw_rect(ball.pos, ball.color);

    display_update_all();
}
