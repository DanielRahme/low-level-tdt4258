
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

    
    display_fill_all(BLACK);

    display_draw_rect(0, 10, 20, 100, RED);
    display_draw_rect(30, 10, 20, 100, WHITE);
    display_draw_rect(60, 10, 20, 100, BLUE);


}


void display_fill_all(uint16_t color)
{
    for (int i = 0; i < info.xres * info.yres; i++) {
        buf[i] = color;
    }
}


void display_draw_rect(int x, int y, int width, int height, int color)
{
    for (int row = y; row < y+height; row++) {
        for (int col = x; col < x+width; col++) {
            if (row + height > info.yres) break;
            if (col + width > info.xres) break;
            buf[row*info.xres + col] = color;
        }
    }
}



// WIP
// WIP
// WIP
// WIP
void display_update(int x, int y, int width, int heigth)
{
    struct fb_copyarea rect;
    rect.dx = 0;
    rect.dy = 0;
    rect.width = info.xres;
    rect.height = info.yres;

    ioctl(fb, FB_DRAW, &rect);
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
