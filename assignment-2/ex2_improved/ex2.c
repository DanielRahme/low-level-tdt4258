#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "melodies.h"
#include "buttons.h"

/*
 * Declaration of peripheral setup functions 
 */

void setupTimer(uint32_t period);
//  void setupLeTimer(uint32_t period);
void setupDAC();
void setupNVIC();
void setupGPIO();



int main(void)
{
    const unsigned int TIMER_PERIOD = 317;

    setupGPIO();
    setupDAC();
    setupTimer(TIMER_PERIOD);
    *EMU_CTRL |= (1<<1);
    *SCR = 2; // Set Deep sleep bit
    
    
    while (1) {
        __asm("wfi");

    }
    return 0;
}

