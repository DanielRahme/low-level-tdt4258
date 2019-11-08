#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <assert.h>

#include "display.h"



int main(int argc, char *argv[])
{
    display_init();
    display_update_all();
    display_close();

    printf("Hello World, I'm game!\n");

    exit(EXIT_SUCCESS);
}
