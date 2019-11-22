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
    int fd = open("/dev/gamepad", O_RDONLY);
    //int fd = open("/dev/mychardev", O_RDONLY);
    if (fd < 0) {
        printf("Fuck we did not open\n");
        close(fd);
        exit(EXIT_SUCCESS);
    }

    while (1) {
        unsigned char buttons = 0;
        int read_return_val = read(fd, &buttons, 1);

        if (read_return_val < 0) {
            printf("Fuck we did not read: %d\n", read_return_val);
        }

        printf("\nRead value: %ud, returned value from func: %d\n", buttons, read_return_val);

        // wait
        volatile unsigned long long i;
        for (i = 0; i < 40000; i++);
    }
    close(fd);

    /*
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
    */

    printf("Hello World, I'm game!\n");
    exit(EXIT_SUCCESS);
}
