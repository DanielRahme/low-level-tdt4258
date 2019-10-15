#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "buttons.h"

unsigned int readButtons(void) 				// Reads the bUTTON and returns the 
{
	return (~*GPIO_PC_DIN);				//Due to pull up the bits are inverted
}

void updateLeds(unsigned int button)
{
	*GPIO_PA_DOUT = ~button << 8;		//inverting and shifting up since the bUTTONs are located at 8 bits lower comparedto the leds
}