#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "timer.h"

/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */
/*
 * The period between sound samples, in clock cycles 
 */
#define ever ;;
#define SAMPLE_PERIOD 10000

int main(void)
{
    //setupGPIO();
    //setupDAC();
    setupTimer(SAMPLE_PERIOD);

    for (ever) {
        if (timerInterrupted()) {
            // Toggle LED
        }
    }

    return 0;
}