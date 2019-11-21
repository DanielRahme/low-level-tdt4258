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



int main(int argc, char *argv[])
{
    display_init();
    display_update_all();

    objects_init();
    update_players_disp();

    for (int i = 0; i < 50; i++) {
        update_players_pos(PLAYER_1_UP, PLAYER_2_DOWN);
        update_players_disp();
    }

    display_close();

    printf("Hello World, I'm game!\n");
    exit(EXIT_SUCCESS);
}
