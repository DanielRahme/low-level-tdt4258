#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"
<<<<<<< HEAD

#define BUTTON0 (1 << 0) //  1  = 0000 0001
#define BUTTON1 (1 << 1) //  2  = 0000 0010
#define BUTTON2 (1 << 2) //  4  = 0000 0100
#define BUTTON3 (1 << 3) //  8  = 0000 1000
#define BUTTON4 (1 << 4) // 16  = 0001 0000
#define BUTTON5 (1 << 5) // 32  = 0010 0000
#define BUTTON6 (1 << 6) // 64  = 0100 0000
#define BUTTON7 (1 << 7) //128  = 1000 0000

=======
>>>>>>> master
/*
 * function to set up GPIO mode and interrupts
 */

#define BUTTON0 (1 << 0)    //  1  = 0000 0001
#define BUTTON1 (1 << 1)    //  2  = 0000 0010
#define BUTTON2 (1 << 2)    //  4  = 0000 0100
#define BUTTON3 (1 << 3)    //  8  = 0000 1000
#define BUTTON4 (1 << 4)  	// 16  = 0001 0000
#define BUTTON5 (1 << 5)  	// 32  = 0010 0000
#define BUTTON6 (1 << 6)  	// 64  = 0100 0000
#define BUTTON7 (1 << 7)	//128  = 1000 0000

void setupGPIO()
{		
	*CMU_HFPERCLKEN0 	|= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL 		|= GPIO_PA_CTRL_DRIVE;		/* set high drive strength  on PA*/
	*GPIO_PA_MODEH 		|= GPIO_PA_MODEH_OUT;		/* set pins A8-15 as output */
	*GPIO_PA_DOUT 		|= GPIO_PA_DOUT_LOW;		/* Turn on LEDs D4-D8 (LEDs are active low) */
	*GPIO_PC_MODEL 		|= GPIO_PC_MODEL_PUSH; 		// set bUTTONs to input/push pull and glitch filter
	*GPIO_PC_DOUT = 0xff; 							//Set internal pull up
	
	//*GPIO_EXTIPSELL = 0x22222222; 			//Enabeling interrupts
	//*GPIO_EXTIFALL = 0xff; 					//Trigger on the rising edge of the  interrupt
	//*GPIO_IEN = 0xff; 						//Enable the interrupt generation

}

uint16_t readButtons(void) 				// Reads the bUTTON and returns the 
{
<<<<<<< HEAD

    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock */
    *GPIO_PA_CTRL |= GPIO_PA_CTRL_DRIVE; /* set high drive strength  on PA*/
    *GPIO_PA_MODEH |= GPIO_PA_MODEH_OUT; /* set pins A8-15 as output */
    *GPIO_PA_DOUT |= GPIO_PA_DOUT_LOW; /* Turn on LEDs D4-D8 (LEDs are active low) */
    *GPIO_PC_MODEL |= GPIO_PC_MODEL_PUSH; // set bUTTONs to input/push pull and glitch filter
    *GPIO_PC_DOUT |= 0xff; //Set internal pull up

    /*
	 * TODO set input and output pins for the joystick 
	 */

    /*
	 * Example of HW access from C code: turn on joystick LEDs D4-D8 check 
	 * efm32gg.h for other useful register definitions 
	 */
=======
	return (~*GPIO_PC_DIN);				//Due to pull up the bits are inverted
}

void updateLeds(uint16_t button)
{
	*GPIO_PA_DOUT = ~button << 8;		//inverting and shifting up since the bUTTONs are located at 8 bits lower comparedto the leds
>>>>>>> master
}

void buttonHandler(uint16_t buttonIdx)
{
	if (!buttonIdx) return;

    const uint8_t buttons[8] = {BUTTON0, BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7};
	
	updateLeds(buttons[buttonIdx]); 			//update leds
}