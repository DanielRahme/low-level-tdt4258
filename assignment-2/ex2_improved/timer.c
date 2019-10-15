#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to setup the timer 
 */
void setupTimer(uint32_t period)
{
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
    *TIMER1_TOP = period;
    *TIMER1_IEN = 1; /* enable timer interrupt */
    *ISER0   |= (1 << 12); /* enable timer interrupt handling */
    *TIMER1_CMD = 1;
    /* poll TIMER1_CNT register for baseline solution*/
}

void setupLeTimer(uint32_t period)
{
  /*  *CMU_HFCORECLKEN0 |= (1<<4); // kanskje 5. bit
    *CMU_LFCLKSEL     |= (1<<0);
    *CMU_LFACLKEN0    |= (1<<2);
    
*/
    *LETIMER_COMP  = period;
    *LETIMER_CTRL |= (1<<9);
    *LETIMER_IEN  |= (1<<3);
    *ISER0        |= (1<<26);
    *LETIMER_CMD  |= (1<<0);
}