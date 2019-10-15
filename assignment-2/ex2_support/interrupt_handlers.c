#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * TIMER1 interrupt handler 
 */
void __attribute__((interrupt)) TIMER1_IRQHandler()
{
    //playOneSample = true;
    *TIMER1_IFC |= 1;
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__((interrupt)) GPIO_EVEN_IRQHandler()
{
    /*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}

/*
 * GPIO odd pin interrupt handler 
 */
void __attribute__((interrupt)) GPIO_ODD_IRQHandler()
{
    /*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}
