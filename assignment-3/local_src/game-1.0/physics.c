
#include "physics.h"

void physics(object_t* ball,object_t* player_2, object_t* player_1 ){

    if (frame_collision(ball,TOP) || frame_collision(ball,BOTTOM)){
        (*ball).velocity.y *= -1;
    }
    if (frame_collision(player_1,TOP) || frame_collision(player_1,BOTTOM)){
        (*player_1).velocity.y *= -1;
    }

    if (frame_collision(player_2,TOP) || frame_collision(player_2,BOTTOM)){
        (*player_2).velocity.y *= -1;
    }

    if (paddle_collision(ball, player_1)){
        (*ball).velocity.x *= -1;
        (*ball).velocity.y += (*player_1).velocity.y;
    } 

    if (paddle_collision(ball, player_2)){
        (*ball).velocity.x *= -1;
        (*ball).velocity.y += (*player_2).velocity.y;
    } 
}

int game_over(object_t* ball,object_t* player_2, object_t* player_1 ) {
    int winner = 0;
    if (!paddle_collision(ball, player_1) &&  (goal_collision(ball, LEFT))){
        winner = 2;
    }
    if (!paddle_collision(ball, player_2) &&  (goal_collision(ball, RIGHT))){
        winner = 1;
    }
return winner;
}
