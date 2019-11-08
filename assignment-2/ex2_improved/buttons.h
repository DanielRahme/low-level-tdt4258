#ifndef BUTTONS_H
#define BUTTONS_H

#define BUTTON0 (1 << 0) //  1  = 0000 0001
#define BUTTON1 (1 << 1) //  2  = 0000 0010
#define BUTTON2 (1 << 2) //  4  = 0000 0100
#define BUTTON3 (1 << 3) //  8  = 0000 1000
#define BUTTON4 (1 << 4) // 16  = 0001 0000
#define BUTTON5 (1 << 5) // 32  = 0010 0000
#define BUTTON6 (1 << 6) // 64  = 0100 0000
#define BUTTON7 (1 << 7) //128  = 1000 0000

unsigned int readButtons(void);
void updateLeds(unsigned int button);

#endif