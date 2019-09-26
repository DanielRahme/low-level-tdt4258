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
	// activate GPIO 
	ldr r1, cmu_base_addr
	ldr r2, [r1, #CMU_HFPERCLKEN0] 
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3
	str r2, [r1, #CMU_HFPERCLKEN0]	
	
	// set port C 0-7 as input
	ldr r1, =GPIO_PC_BASE
	mov r2, #0x33333333
	mov r3, 0xff
	str r2, [r1, #GPIO_MODEL]
	str r3, [r1, #GPIO_DOUT]
	
	// set port A 8-15 to output    
	ldr r0, =GPIO_PA_BASE
	mov r2, #0x55555555 
	str r2, [r0, #GPIO_MODEH]
	
	// disable ramblock
	mov r2, #7
	ldr r1, =EMU_BASE
    str r2, [r0]
	    
	// set high drive strength
	mov r3, #0x3
	str r3, [r0, #GPIO_CTRL]

	// timer interrupt
	ldr r0, =CMU_BASE
	mov r1, 0x10000
	str r1, [r0, 0x028]		// LFCLKSEL
	mov r1, 0x6
	str r1, [r0, 0x058]		// LFACLKEN
	mov r1, 0x600
	str r1, [r0, 0x068]		// LFAPRESC0
	mov r1, 0x4
	str r1, [r0, 0x03c]		// CMU_IEN
	    
	//enable interrupts
	ldr r0, =GPIO_BASE
	ldr r4, =ISER0
	mov r1, #0x22222222
	mov r2, #0xff
	mov r3, #0x00
	ldr r5, =#0x802
	str r1, [r0, #GPIO_EXTIPSELL]	// select interrupt pins
	str r3, [r0, #GPIO_EXTIFALL]	// trigger at falling edge
	str r2, [r0, #GPIO_EXTIRISE]	// trigger at rising edge
	str r2, [r0, #GPIO_IEN]			// enable interrupts
	str r5, [r4]					// enable interrupt handling
	
	ldr r4, =leds
	mov r5, 0x8000
	str r5, [r4]
	
	ldr r4, =blinker
	mov r5, 0x5500
	str r5, [r4]
	
	ldr r4, =buttons
	mov r5, 0x40
	str r5, [r4]
	
	ldr r4, =speed
	mov r5, #0x81000
	str r5, [r4]
	
	ldr r4, =state
	mov r5, #1
	str r5, [r4]
	
	ldr r4, =inverter
	mov r5, #0xff00
	str r5, [r4]
	    
	ldr r1, =SCR
	mov r2, #6	
	str r2, [r1]

/*------start of main code-------*/


main:	
	
	ldr r0, =GPIO_PA_BASE	
	bl state_select
	ldr r2, =state
	ldr r3, [r2]
	cmp r3, #1
	beq shift_led_left
	cmp r3, #2
	beq shift_led_right
	cmp r3, #3
	beq blink
	b main
	
state_select:
	push {lr}
	ldr r4, =buttons		// button address
	ldr r6, =state			// state address
	ldr r5, [r4]			// load button press 

	cmp r5, #0x01					// if button 1 is pressed
	beq select_full_blink				 

	cmp r5, #0x02					// if button 2 is pressed
	beq select_alternating_blink 

	cmp r5, #0x04					// if button 3 is pressed
	beq select_invert				
	
	cmp r5, #0x08					// if button 4 is pressed
	beq select_sleep			 

	cmp r5, #0x10					// if button 5 is pressed 
	beq select_left			 

	cmp r5, #0x20					// if button 6 is pressed 
	beq select_increase				 

	cmp r5, #0x40					// if button 7 is pressed 
	beq select_right				 

	cmp r5, #0x80					// if button 8  is pressed
	beq select_decrease				

state_select_end:
	bl reset_buttons
	pop {lr}
	bx lr    					// return

//////////////////////////////////////////
// selection options
//////////////////////////////////////////

select_left:
	mov r7, #1					// else store left shift state (1)
	str r7, [r6]				// to the state
	b state_select_end
	
select_led_positional_blink: 
	ldr r8, =blinker			// else retrive blinker address
	ldr r9, =leds				// and the leds address
	ldr r10, [r9]				// retrive the current led position
	str r10, [r8]				// store current led position as blinker
	mov r7, #3					// store the blinker state (3)
	str r7, [r6]				// to the state	
	b state_select_end
	
select_alternating_blink:		
	ldr r8, =blinker			// else retrive blinker address
	mov r10, #0x5500			// set alternating pattern
	str r10, [r8]				// store alternating pattern as blinker
	mov r7, #3					// store the blinker state (3)
	str r7, [r6]				// to the state	
	b state_select_end

select_full_blink:				
	ldr r8, =blinker			// else retrive blinker address
	mov r10, #0xff00			// set all leds to the same value
	str r10, [r8]				// store same as blinker
	mov r7, #3					// store the blinker state (3)
	str r7, [r6]				// to the state	
	b state_select_end
	
select_increase:	
	bl increase_speed			// increase the speed 
	b state_select_end
	
select_right:			// is not pressed skip
	mov r7, #2					// else store the right shift state
	str r7, [r6]				// to the state	
	b state_select_end
	
select_decrease:				// is not pressed skip
	bl decrease_speed			// else decrease the speed
	b state_select_end
	
select_invert:
	ldr r7, =inverter
	ldr r8, [r7]
	mov r9, #0xff00
	eor r8, r8, r9
	str r8, [r7]
	b state_select_end
	
select_sleep:
	//// sleep code ///
	b state_select_end	

reset_buttons:
	ldr r4, =buttons		// button address
	mov r9, 0x0000				
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
	cmp r2, 0x8000				// checks if led is at the last stop
	beq reset_right				// reset the led position
	lsl r2, r2, #1				// shift led to the right on the board
right_end:	
	str r2, [r5]
	bl delay				// delay between shifts
	b main					// return to main

reset_right:
	mov r2, 0x0100			// reset led position
	b right_end				// return to right shift loop			
			
shift_led_left:
	ldr r0, =GPIO_PA_BASE
	ldr r5, =leds			// led address
	ldr r2, [r5]			// led values
	ldr r7, =inverter
	ldr r3, [r7]			// bits to toggle
	eor r4, r2, r3			// toggle led bits
	str r4, [r0, #GPIO_DOUT]// write output leds
	cmp r2, 0x0100			// checks if led is at the last stop
	beq reset_left			// reset the led position
	lsr r2, r2, #1			// shift led to the left on the board
left_end:	
	str r2, [r5]
	bl delay				// delay between shifts
	b main

reset_left:
	mov r2, 0x8000			// reset led position
	b left_end				// return to left shift loop			
			

blink:
	ldr r0, =GPIO_PA_BASE	
	ldr r5, =blinker		// blinker address
	ldr r2, [r5]			// blinker values
	str r2, [r0, #GPIO_DOUT]
	mov r3, #0xff00
	eor r2, r2, r3
	str r2, [r5]
	bl delay
	b main

/////////////////////////////////////////////
// other functions
/////////////////////////////////////////////

delay: 
	ldr r3, =speed
	ldr r1, [r3]
delay_loop:
	sub	r1, r1, #1		// i--
	cmp r1, #0			// compare against zero
	bne delay_loop		// loop if not zero
	bx lr				// return	
	
decrease_speed:
	push {lr}
	ldr r8, =speed
	ldr r9, [r8]
	cmp r9, #0x100000
	bgt decrease_end
	add r9, r9, #0x10000
decrease_end:
	str r9, [r8]
	pop {lr}
	bx lr

increase_speed:
	push {lr}
	ldr r8, =speed
	ldr r9, [r8]
	cmp r9, #0x13000
	blt increase_end
	sub r9, r9, #0x10000
increase_end:
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
	ldr r9, [r8, GPIO_IF]	// read interrupt flags
	ldr r10, =buttons		// read button address
	str r9, [r10]			// store button presses
	str r9, [r8, GPIO_IFC]	// clear interrupt flags
	bx lr	

	/////////////////////////////////////////////////////////////////////////////
		.thumb_func
timer_handler:
		push {r0 - r11}
		ldr r0, =CMU_BASE
		ldr r1, [r0, #0x030]	// CMU interrupt flag

		str r1, [r0, #0x038]	// clear interrupt flag
		pop {r0 - r11}
		bx lr

        .thumb_func
dummy_handler:  
        b .  // do nothing

cmu_base_addr:
			.long CMU_BASE
			
	.section .data
buttons:
	.word 0
leds:
	.word 0
blinker:
	.word 0
speed: 
	.word 0
state:
	.word 0
inverter:
	.word 0	
	
