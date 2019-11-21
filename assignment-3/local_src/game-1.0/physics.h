
#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "collision.h"
#include "game_objects.h"

void physics(object_t* ball,object_t* player_2, object_t* player_1 );
int game_over(object_t* ball,object_t* player_2, object_t* player_1 );

#endif
