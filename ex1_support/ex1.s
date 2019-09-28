        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
	// Exception vector table
	// This table xcontains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                0 */
	      .long   _reset                  /* Reset Handler               4 */
	      .long   dummy_handler           /* NMI Handler                 8 */
	      .long   dummy_handler           /* Hard Fault Handler          b */
	      .long   dummy_handler           /* MPU Fault Handler           f */
	      .long   dummy_handler           /* Bus Fault Handler           14 */
	      .long   dummy_handler           /* Usage Fault Handler         18 */
	      .long   dummy_handler           /* Reserved                    1b */
	      .long   dummy_handler           /* Reserved                    1f */
	      .long   dummy_handler           /* Reserved                    24 */
	      .long   dummy_handler           /* Reserved                    28 */
	      .long   dummy_handler           /* SVCall Handler              2b */
	      .long   dummy_handler           /* Debug Monitor Handler       2f */
	      .long   dummy_handler           /* Reserved                    34 */
	      .long   dummy_handler           /* PendSV Handler              38 */
	      .long   dummy_handler           /* SysTick Handler             3b */

	      /* External Interrupts */
	      .long   dummy_handler										/*	 3f */
	      .long   gpio_handler            /* GPIO even handler 			 44 */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler			//12
	      .long   dummy_handler			//13
	      .long   dummy_handler			//14
	      .long   dummy_handler			//15
	      .long   dummy_handler			//16
	      .long   dummy_handler			//17
	      .long   dummy_handler			//18
	      .long   dummy_handler			//19
	      .long   dummy_handler			//20
	      .long   dummy_handler			//21
	      .long   dummy_handler			//22
	      .long   dummy_handler			//23
	      .long   dummy_handler			//24
	      .long   dummy_handler			//25
	      .long   timer_handler			//26
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
    // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 
	// activate GPIO clock
	ldr r1, cmu_base_addr
	ldr r2, [r1, #CMU_HFPERCLKEN0] 
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3
	str r2, [r1, #CMU_HFPERCLKEN0]	
	//activate LE clock
	mov r2, #0x10
	str r2, [r1, #CMU_HFCORECLKEN0]		// HFCORE

	
	// set high drive strength
	mov r3, #0x3
	str r3, [r0, #GPIO_CTRL]

	// set port A 8-15 to output    
	ldr r0, =GPIO_PA_BASE
	mov r2, #0x55555555 
	str r2, [r0, #GPIO_MODEH]	// sets PORTA as output 
	//initialize LEDs
	mov r3, #0xfe00
	str r3, [r0, #GPIO_DOUT]	// sets the left most LED as high as the default


	// set port C 0-7 as input
	ldr r1, =GPIO_PC_BASE
	mov r2, #0x33333333
	mov r3, #0xff
	str r2, [r1, #GPIO_MODEL] // sets PORTC in filtered input mode
	str r3, [r1, #GPIO_DOUT]  // Pull up
	

// timer clock
	ldr r0, cmu_base_addr
	mov r1, #0x10000
	str r1, [r0, #CMU_LFCLKSEL]		// selects the ULFRCO as the low frequency clock
	mov r1, #0x6
	str r1, [r0, #CMU_LFCLKEN0]		// Enables the low frequency clock
	//mov r1, #0x200
	//str r1, [r0, #CMU_PRESEC0]	// LFAPRESC0	clock prescaler


	// LETIMER0 setup
	ldr r0, =LETIMER_BASE		 // LETIMER_BASE
	mov r1, #0x100				 
	str r1, [r0, #LETIMER_COMP0] // compare register: sets the frequency of the LETIMER interrupt
	mov r1, #(1<<9)
	str r1, [r0, #LETIMER_CTRL]	 // sets the compare regiser as the top of the LETIMER counter
	mov r1, #0x4
	str r1, [r0, #LETIMER_IEN]	 // Enables the LETIMER, and sets the interrupt to trigger when the counter underflows
	mov r1, #0x1	
	str r1, [r0, #LETIMER_CMD]	 // starts the LETIMER


	//enable interrupts
	ldr r0, =GPIO_BASE
	mov r1, #0x22222222
	mov r2, #0xff
	mov r3, #0x00
	str r1, [r0, #GPIO_EXTIPSELL]	// select PORTC as interrupt pins 
	str r3, [r0, #GPIO_EXTIFALL]	// dissable falling edge interrupt trigger
	str r2, [r0, #GPIO_EXTIRISE]	// enable ising edge interrupt trigger
	str r2, [r0, #GPIO_IEN]			// enable GPIO interrupts
	ldr r4, =ISER0
	ldr r5, =#0x802					// GPIO handler, mov instruction does not work for #0x802
	mov r6, #(1<<26)				// LETIMER handler
	orr r5, r5, r6					
	str r5, [r4]					// enable interrupt handlers 

	// enable sleep
	ldr r1, =SCR 
    mov r2, #4    
    str r2, [r1] 	// enters deep sleep when waiting for interrupt
	
	// variable init
	ldr r4, =leds
	mov r5, #0x8000
	str r5, [r4]
	
	ldr r4, =blinker
	mov r5, #0x5500
	str r5, [r4]
	
	ldr r4, =buttons
	mov r5, #0x40
	str r5, [r4]
	
	ldr r4, =speed
	mov r5, #0x100
	str r5, [r4]
	
	ldr r4, =state
	mov r5, #1
	str r5, [r4]
	
	ldr r4, =inverter
	mov r5, #0xff00
	str r5, [r4]  

		
	ldr r4, =timerIRQ
	mov r5, #0x0
	str r5, [r4]  
	
/*------start of main code-------*/
main:
	ldr r4, =timerIRQ	//reset LETIMER flag
	mov r5, #0x0
	str r5, [r4]	
	wfi					// enter deep sleep and wait for interrupt
	ldr r4, =timerIRQ	// load timerIRQ
	ldr r5, [r4]
	cmp r5, #0x100		// if timerIRQ  
	bne main			// is not 0x100 then branch to main
						// else
	bl state_select		// interperate the button presses
	ldr r2, =state		// load state
	ldr r3, [r2]
	cmp r3, #1			// find out which state to branch to
	beq shift_led_left	// branch to the correct state
	cmp r3, #2
	beq shift_led_right
	cmp r3, #3
	beq blink
	b main
	
state_select:
	push {lr}				// pushing the linker register in order to branch and link within the state select 
	ldr r4, =buttons		// button address
	ldr r6, =state			// state address
	ldr r5, [r4]			// load button press 

	cmp r5, #0x01					// if button 1 is pressed
	beq select_full_blink			// blink all of the LEDs		 

	cmp r5, #0x02					// if button 2 is pressed
	beq select_alternating_blink 	// blink every other LED

	cmp r5, #0x04					// if button 3 is pressed
	beq select_invert				// invert the output of the moving LED
	
	cmp r5, #0x08					// if button 4 is pressed
	beq select_led_positional_blink	// blink based the moving LED possition

	cmp r5, #0x10					// if button 5 is pressed 
	beq select_left			 		// move the LED left

	cmp r5, #0x20					// if button 6 is pressed 
	beq select_increase				// increase the speed/frequency of the LETIMER => blink/move LED faster

	cmp r5, #0x40					// if button 7 is pressed 
	beq select_right				// move the LED right

	cmp r5, #0x80					// if button 8  is pressed
	beq select_decrease				// decrease the speed/frequency of the LETIMER => blink/move LED slower

state_select_end:
	bl reset_buttons				// reset the button press. Ensures that each button press is only read once and not continually 
	pop {lr}						
	bx lr    					// return

//////////////////////////////////////////
// selection options
//////////////////////////////////////////

select_left:
	mov r7, #1					// store left shift state (1)
	str r7, [r6]				// to the state
	b state_select_end
	
select_led_positional_blink: 
	ldr r8, =blinker			// retrive blinker address
	ldr r9, =leds				// and the leds address
	ldr r10, [r9]				// retrive the current led position
	str r10, [r8]				// store current led position as blinker
	mov r7, #3					// store the blinker state (3)
	str r7, [r6]				// to the state	
	b state_select_end
	
select_alternating_blink:		
	ldr r8, =blinker			// retrive blinker address
	mov r10, #0x5500			// set alternating pattern
	str r10, [r8]				// store alternating pattern as blinker
	mov r7, #3					// store the blinker state (3)
	str r7, [r6]				// to the state	
	b state_select_end

select_full_blink:				
	ldr r8, =blinker			// retrive blinker address
	mov r10, #0xff00			// set all leds to the same value
	str r10, [r8]				// store same as blinker
	mov r7, #3					// store the blinker state (3)
	str r7, [r6]				// to the state	
	b state_select_end
	
select_increase:	
	bl increase_speed			// increase the speed 
	b state_select_end
	
select_right:					
	mov r7, #2					// store the right shift state
	str r7, [r6]				// to the state	
	b state_select_end
	
select_decrease:				
	bl decrease_speed			// decrease the speed
	b state_select_end			
	
select_invert:
	ldr r7, =inverter
	ldr r8, [r7]
	mov r9, #0xff00
	eor r8, r8, r9
	str r8, [r7]
	b state_select_end

reset_buttons:
	ldr r4, =buttons			// button address
	mov r9, #0x0000				
	str r9, [r4]				//reset buttons
	bx lr
	
//////////////////////////////////////////	
// states
//////////////////////////////////////////
		
shift_led_right:
	ldr r0, =GPIO_PA_BASE
	ldr r5, =leds				// led address
	ldr r2, [r5]				// led values
	ldr r7, =inverter
	ldr r3, [r7]				// bits to toggle
	eor r4, r2, r3				// toggle led bits
	str r4, [r0, #GPIO_DOUT]	// write output leds
	cmp r2, #0x8000				// checks if led is at the last stop
	beq reset_right				// reset the led position
	lsl r2, r2, #1				// shift led to the right on the board
right_end:	
	str r2, [r5]
	b main					// return to main

reset_right:
	mov r2, #0x0100			// reset led position
	b right_end				// return to right shift loop			
			
shift_led_left:
	ldr r0, =GPIO_PA_BASE
	ldr r5, =leds			// led address
	ldr r2, [r5]			// led values
	ldr r7, =inverter
	ldr r3, [r7]			// bits to toggle
	eor r4, r2, r3			// toggle led bits
	str r4, [r0, #GPIO_DOUT]// write output leds
	cmp r2, #0x0100			// checks if led is at the last stop
	beq reset_left			// reset the led position
	lsr r2, r2, #1			// shift led to the left on the board
left_end:	
	str r2, [r5]
	b main

reset_left:
	mov r2, #0x8000			// reset led position
	b left_end				// return to left shift loop			
			
blink:
	ldr r0, =GPIO_PA_BASE	
	ldr r5, =blinker		 // load blinker address
	ldr r2, [r5]			 // laod blinker values
	str r2, [r0, #GPIO_DOUT] // set blikner to the LED output
	mov r3, #0xff00				
	eor r2, r2, r3			// invert blinker
	str r2, [r5]			// store inverted blinker
	b main

/////////////////////////////////////////////
// other functions
/////////////////////////////////////////////	
	
decrease_speed:
	push {lr}			
	ldr r8, =speed		//load the current speed of the timer
	ldr r9, [r8]		 
	cmp r9, #0x200		// if the current speed
	bgt decrease_end	// is smaller than 0x200, then
	add r9, r9, #0x10	// decrease the speed by 0x10
decrease_end:
	ldr r10, =LETIMER_BASE			
	str r9, [r10, #LETIMER_COMP0]	// store the new speed in the compare0 register
	str r9, [r8]					// store the new speed in speed register (not really needed anymore if compare0 register is used instead )
	pop {lr}
	bx lr

increase_speed:
	push {lr}
	ldr r8, =speed
	ldr r9, [r8]
	cmp r9, #0x11
	blt increase_end
	sub r9, r9, #0x10
increase_end:
	ldr r10, =LETIMER_BASE
	str r9, [r10, #LETIMER_COMP0]
	str r9, [r8]
	pop {lr}
	bx lr
	
	/////////////////////////////////////////////////////////////////////////////
	//
	// GPIO handler
	// The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	ldr r8, =GPIO_BASE
	ldr r9, [r8, GPIO_IF]	// read GPIO interrupt flags
	ldr r10, =buttons		// read button address
	str r9, [r10]			// store GPIO interrupt flags as button presses
	str r9, [r8, GPIO_IFC]	// clear GPIO interrupt flags
	bx lr	

	/////////////////////////////////////////////////////////////////////////////
		.thumb_func
timer_handler:
	push {r0 - r12}
	ldr r0, =LETIMER_BASE
	ldr r1, [r0, #LETIMER_IF]	// laod LETIMER interrupt flag
	str r1, [r0, #LETIMER_IFC]	// clear LETIMER interrupt flag	
	ldr r4, =timerIRQ		
	mov r5, #0x100
	str r5, [r4]				// interrupt filtering flag
	pop {r0 - r12}
	bx lr

        .thumb_func
dummy_handler:  
        b .  // do nothing

cmu_base_addr:
			.long CMU_BASE
			
	.section .data
buttons:
	.word 0	// thsi is used to store which button was pressed. ensures that button press is only used once
leds:
	.word 0	// this is used as the position of the moving LED
blinker:
	.word 0	// this is used as the blinking pattern to alternate
speed: 
	.word 0	// this is used to modify the speed (frequency)
state:
	.word 0	// this is used to select the desired led behavior
inverter:
	.word 0	// This is used to invert the output of the moving LED
timerIRQ:	
	.word 0	// This is used to filter out the LETIMER interrupts from the button interrupts
	
