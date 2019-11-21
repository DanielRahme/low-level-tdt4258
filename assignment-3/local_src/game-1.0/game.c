#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <assert.h>

// Libraries
#include "display.h"
#include "game_objects.h"
#include "collision.h"
//#include "physics.h"



int main(int argc, char *argv[])
{
    display_init();
    display_update_all();

    objects_init();
    redraw_players();

    for (int i = 0; i < 1000; i++) {
        update_players_pos(PLAYER_1_DOWN, PLAYER_NEUTRAL);
        redraw_players();
        update_ball();
        update_players_pos(PLAYER_1_UP, PLAYER_NEUTRAL);
        redraw_players();
        update_ball();
    }

    display_close();

    printf("Hello World, I'm game!\n");
    exit(EXIT_SUCCESS);
}
