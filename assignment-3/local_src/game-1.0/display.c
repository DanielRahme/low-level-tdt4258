
#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <assert.h>
#include "display.h"


#define BACKGROUND_COLOR 0xff00
#define FB_DRAW 0x4680

// My globals
static int fb = 0;
static uint16_t* buf = NULL;
static uint32_t disp_size = 0;
static struct fb_var_screeninfo info = {0};


void display_init()
{
    fb = open("/dev/fb0", O_RDWR);
    assert(fb > 0);

    // Get screen info: width, height and bits per pixel
    assert(0 == ioctl(fb, FBIOGET_VSCREENINFO, &info));

    printf("x: %d\ty: %d\tbits-per-pixel:%d\n", info.xres, info.yres, info.bits_per_pixel);

    disp_size = info.xres * info.yres * info.bits_per_pixel/8;
    buf = mmap(NULL, disp_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    assert(buf != MAP_FAILED);


    // To be removed (fill whole screen instead)
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 200; x++) {
            buf[y*info.xres + x] = BACKGROUND_COLOR;
        }
    }
}


void display_update_all()
{
    struct fb_copyarea rect;
    rect.dx = 0;
    rect.dy = 0;
    rect.width = info.xres;
    rect.height = info.yres;

    ioctl(fb, FB_DRAW, &rect);
}

void display_close()
{
    munmap(buf, disp_size);
    close(fb);
}
