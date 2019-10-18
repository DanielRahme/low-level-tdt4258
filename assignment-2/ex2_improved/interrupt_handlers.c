#include <stdint.h>
#include <stdbool.h>
#include "melodies.h"
#include "efm32gg.h"
#include "buttons.h"

volatile uint8_t button = 0;
volatile bool playOneSample = false;

/*
 * TIMER1 interrupt handler 
 */
void __attribute__((interrupt)) TIMER1_IRQHandler()
{
    playOneSample = true;
	uint8_t desiredMelody = button;
	uint16_t amplitude = MAX_VOLUME;
    uint32_t tempo = 44100;
	playMelody(&desiredMelody, &amplitude, tempo);
	*TIMER1_IFC |= 1;
	
}

void __attribute__((interrupt)) LETIMER0_IRQHandler()
{
    playOneSample = true;
    *LETIMER_IFC |= (1<<2);
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__((interrupt)) GPIO_EVEN_IRQHandler()
{	
	button = *GPIO_IF;
    *GPIO_IFC = GPIO_IFC_CLEAR; //
	
    updateLeds(button);
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
	button = *GPIO_IF;
	*GPIO_IFC = GPIO_IFC_CLEAR;

    updateLeds(button);
    /*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}
