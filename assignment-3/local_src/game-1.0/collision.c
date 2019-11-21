
#include "collision.h"


abs_pos get_abs_pos(object_t *game_object){
    abs_pos result;
    result.x_left   = (*game_object).pos.x;
    result.x_right  = (*game_object).pos.x + (*game_object).pos.width;
    result.y_top    = (*game_object).pos.y;
    result.y_bottom = (*game_object).pos.y + (*game_object).pos.height;
    return result;
}

bool detect_collision(object_t* object_1,object_t* object_2){
    abs_pos obj_1 = get_abs_pos(object_1);
    abs_pos obj_2 = get_abs_pos(object_2);

    bool y_col = ( obj_1.y_bottom >= obj_2.y_top && obj_1.y_top <= obj_2.y_bottom );
    bool x_col = ( obj_1.x_left <= obj_2.x_right && obj_1.x_right >= obj_2.x_left );
    return(y_col && x_col);
}


bool goal_collision(object_t *ball,int side) { // side = left or right
    object_t goal;
    goal.pos.x = side;
    goal.pos.y = 0;
    goal.pos.width = PADDLE_WIDTH -1;
    goal.pos.height = DISP_HEIGHT;

    return detect_collision(ball, &goal);
}

bool frame_collision(object_t *ball,int side){  // side = top or or bottom
    object_t frame;
    frame.pos.x = 0;
    frame.pos.y = side;
    frame.pos.width = DISP_WIDTH;
    frame.pos.height = 1;

    return detect_collision(ball, &frame);    
}

bool paddle_collision(object_t *ball,object_t* paddle){
    return detect_collision(ball, paddle);    
}


