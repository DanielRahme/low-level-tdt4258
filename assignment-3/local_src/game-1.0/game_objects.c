
#include "display.h"
#include "game_objects.h"

#define SCREEN_X 320
#define SCREEN_Y 240

static object_t player_1;
static object_t player_2;
static object_t ball;


void objects_init()
{
    // X, Y, Width, Height
    rect_t tmp = {0, 80, 32, 80};
    player_1.pos = tmp;
    player_1.velocity.x = PLAYER_NEUTRAL_SPEED;
    player_1.velocity.y = PLAYER_NEUTRAL_SPEED;
    player_1.color = RED;
    display_draw_rect(player_1.pos, player_1.color);


    tmp.x = SCREEN_X - 32;
    player_2.pos = tmp;
    player_2.velocity.x = PLAYER_NEUTRAL_SPEED;
    player_2.velocity.y = PLAYER_NEUTRAL_SPEED;
    player_2.color = BLUE;
    display_draw_rect(player_2.pos, player_2.color);

    rect_t tmp2 = {SCREEN_X/2, SCREEN_Y/2, 16, 16};
    ball.pos = tmp2;
    ball.velocity.x = 1;
    ball.velocity.y = 1;
    ball.color = WHITE;
    display_draw_rect(ball.pos, ball.color);

    //display_update_all();
}

void update_players_disp()
{
    // Player 1 update
    rect_t tmp1         = {0, 0, 32, SCREEN_Y};
    display_draw_rect(tmp1, BLACK);
    display_draw_rect(player_1.pos, player_1.color);
    display_update(tmp1);
    
    // Player 2 update
    rect_t tmp2         = {SCREEN_X - 32, 0, 32, SCREEN_Y};
    display_draw_rect(tmp2, BLACK);
    display_draw_rect(player_2.pos, player_2.color);
    display_update(tmp2);
}


void update_pos(object_t *obj)
{
    obj->pos.x += obj->velocity.x;
    obj->pos.y += obj->velocity.y;
}


void update_players_pos(player_btn_t btn_p1, player_btn_t btn_p2)
{
    player_1.velocity.x = PLAYER_NEUTRAL_SPEED; // Should never change
    player_2.velocity.x = PLAYER_NEUTRAL_SPEED; // Should never change
    player_1.velocity.y = PLAYER_NEUTRAL_SPEED; 
    player_2.velocity.y = PLAYER_NEUTRAL_SPEED; 

    // Update velocity with respect to button pressed
    if (btn_p1 == PLAYER_1_UP) {
        player_1.velocity.y = PLAYER_UP_SPEED;

    } else if (btn_p1 == PLAYER_1_DOWN) {
        player_1.velocity.y = PLAYER_DOWN_SPEED;
    } 

    if (btn_p2 == PLAYER_2_UP) {
        player_2.velocity.y = PLAYER_UP_SPEED;

    } else if (btn_p2 == PLAYER_2_DOWN) {
        player_2.velocity.y = PLAYER_DOWN_SPEED;
    } 

    // Update player positions
    update_pos(&player_1);
    update_pos(&player_2);

    // Check limits
    const int upper_limit = (SCREEN_Y-1) - player_1.pos.height;
    const int lower_limit = 0;

    if (player_1.pos.y > upper_limit) player_1.pos.y = upper_limit;
    if (player_1.pos.y < lower_limit) player_1.pos.y = lower_limit;
    if (player_2.pos.y > upper_limit) player_2.pos.y = upper_limit;
    if (player_2.pos.y < lower_limit) player_2.pos.y = lower_limit;
}
