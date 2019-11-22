
#ifndef _DISPLAY_H
#define _DISPLAY_H

//#include <stdint.h>

// Colors
#define BLACK   0x0000
#define WHITE   0xffff
#define RED     0xf800
#define GREEN   0x07e0
#define BLUE    0x001f
#define YELLOW 0xffc5

typedef struct {
    int x;
    int y;
    int width;
    int height;
} rect_t;



void display_init();
void display_update_all();
void display_close();
void display_fill_all(int color);
void display_draw_rect(rect_t rect, int color);
void display_update(rect_t my_rect);

#endif
