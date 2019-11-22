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

typedef enum {
    IDLE,
    PLAY,
    SCORE
} state_t;

static int fd = -1;



unsigned char read_gamepad()
{
    unsigned char buttons = 0;
    int read_return_val = read(fd, &buttons, 1);
    return buttons;
}


void game_loop()
{
    //// LOOP
    while (ball_is_alive()) {
        update_players(read_gamepad());
        update_ball();
        //for (volatile unsinged long long i = 0; i < 40000; i++);
    }
}


void fsm(state_t *current_state)
{
    static int init_once = 0;

    switch (*current_state) {

        case IDLE: {
                       // Outputs
                       // IDLE screen
                       if (!init_once) {
                           start_screen();
                           init_once = 1;
                       }
                       // Next state condition
                       if (read_gamepad() > 0) *current_state = PLAY;
                       break;
                   }

        case PLAY: {
                       // Exit game_loop() when a player wins
                       display_init();
                       display_update_all();
                       objects_init();
                       game_loop();
                       *current_state = SCORE;
                       break;
                   }

        case SCORE: {
                        const int winner = get_winner();

                        if (winner == 1) screen_winner_p1();
                        else screen_winner_p2();

                        printf("Winner is player: %d\n", winner);
                        for (volatile unsigned int i = 0; i < 1000000; i++); // Wait
                        *current_state = IDLE;
                        init_once = 0;
                        break;
                    }

        default:{ 
                    init_once = 0;
                    *current_state = IDLE;
                }
    }
}

void init()
{
    // Device driver 

    display_init();
    display_update_all();
    objects_init();
}

int main(int argc, char *argv[])
{
    fd = open("/dev/gamepad", O_RDONLY);
    if (fd < 0) {
        printf("Fuck we did not open\n");
        close(fd);
        exit(EXIT_SUCCESS);
    }


    init();
    state_t state = IDLE;

    while(1) {
        fsm(&state);
    }

    close(fd);

    /*

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
