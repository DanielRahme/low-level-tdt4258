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


enum tempos {
    FAST_TEMPO = 44100,
    NORMAL_TEMPO = 88200,
    SLOW_TEMPO = 176400
};
void initiate(void)
{

}

int main(void)
{
    setupGPIO();
    setupDAC();
    setupTimer(317);
    //*EMU_CTRL |= (1<<1);
    //*SCR = 4; // Set Deep sleep bit
    
    /*
    uint32_t tempo = FAST_TEMPO;
    uint16_t amplitude = MAX_VOLUME;
    uint8_t desiredMelody = 0;
    */
    
    // The butten is updated by the GPIO pin isr
    extern volatile uint8_t button; 
    while (1) {
        //__asm("wfi");

        // Moved to timer interrupt isr
       /* if (button > 0) {
            updateLeds(button);
            desiredMelody = button;
            button = 0; 
        }
        playMelody(&desiredMelody, &amplitude, tempo);
        */
    }
    return 0;
}

