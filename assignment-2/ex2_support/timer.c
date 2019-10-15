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
	//*TIMER1_IEN = 1;		/* enable timer interrupt */
	//*ISER0 |= (1<<12);		/* enable timer interrupt handling */
	*TIMER1_CMD = 1;
	/* poll TIMER1_CNT register for baseline solution*/
}
