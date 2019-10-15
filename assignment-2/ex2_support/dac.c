#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0; /* enable DAC clock*/
    *DAC0_CTRL = 0x50010; /* set prescaler correctly, F_DAC = 437.5kHz */
    *DAC0_CH0CTRL = 1; /* enable left and right DAC channels */
    *DAC0_CH1CTRL = 1;
    /*
	*DAC0_CH0DATA = data;
	*DAC0_CH1DATA = data; use this to write data to DAC. data must be 12bits.
	*/
}