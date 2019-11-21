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
    display_update_all();
    display_close();

    printf("Hello World, I'm game!\n");

    exit(EXIT_SUCCESS);
}
