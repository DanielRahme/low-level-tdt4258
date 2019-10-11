#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to setup the timer 
 */
void setupTimer(uint16_t period)
{
	// Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0 2.
	*CMU_HFCORECLKEN0	|= CMU2_HFPERCLKEN0_TIMER1; 

	// Write the period to register TIMER1_TOP 3=0x100
	*TIMER1_TOP			|= period; 

	// Enable timer interrupt generation by writing 1 to TIMER1_IEN
	*TIMER1_IEN			|= OF | UF;

	// ENABELING THE 12 BIT FOR TIMER
	*ISER0				|= 1<<12; 

	// Start the timer by writing  1 to TIMER1_CMD
	*TIMER1_CMD			|= 1;
}


int timerInterrupted()
{
	return *TIMER1_IF;
}