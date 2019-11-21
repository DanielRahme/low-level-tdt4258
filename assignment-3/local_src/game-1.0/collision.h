#include <stdbool.h>
#include "game_objects.h"
#include "display.h"

#define DISP_HEIGHT 240
#define DISP_WIDTH 320

#define GOAL_LEFT 1
#define GOAL_RIGHT 2
#define PADDLE_WIDTH 32

#define LEFT 0
#define RIGHT (DISP_WIDTH - PADDLE_WIDTH - 1)
#define TOP 0
#define BOTTOM DISP_HEIGHT

typedef struct
{
    int x_left;
    int x_right;
    int y_top;
    int y_bottom;
}abs_pos;

abs_pos get_abs_pos(object_t *game_object);
bool detect_collision(object_t* object_1,object_t* object_2);
bool goal_collision(object_t *ball,int side);
bool frame_collision(object_t *ball,int side);
bool paddle_collision(object_t *ball,object_t* paddle);

