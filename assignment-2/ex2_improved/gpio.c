#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"


/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{

    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock */
    *GPIO_PA_CTRL |= GPIO_PA_CTRL_DRIVE;       /* set high drive strength  on PA*/
    *GPIO_PA_MODEH |= GPIO_PA_MODEH_OUT;       /* set pins A8-15 as output */
    *GPIO_PA_DOUT |= GPIO_PA_DOUT_LOW;         /* Turn on LEDs D4-D8 (LEDs are active low) */
    *GPIO_PC_MODEL |= GPIO_PC_MODEL_PUSH;      // set bUTTONs to input/push pull and glitch filter
    *GPIO_PC_DOUT |= 0xff; 					   //Set internal pull up

    *GPIO_EXTIPSELL 	|= 0x22222222; 			//Enabeling interrupts
	*GPIO_EXTIFALL 		|= 0xff; 				//Trigger on the rising edge of the  interrupt
	*GPIO_IEN 			|= 0xff; 				//Enable the interrupt generation
    *ISER0              |= 1 << 1;	
	*ISER0              |= 1 << 11;
}
