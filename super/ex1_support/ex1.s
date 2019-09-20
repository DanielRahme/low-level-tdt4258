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
	    
	// set high drive strength
	mov r3, #0x3
	str r3, [r0, #GPIO_CTRL]
	    
	//enable interrupts
	ldr r0, =GPIO_BASE
	ldr r4, =ISER0
	mov r1, #0x22222222
	mov r2, #0xff
	mov r3, #0x00
	ldr r5, =#0x802
	str r1, [r0, #GPIO_EXTIPSELL]	// select interrupt pins
	str r3, [r0, #GPIO_EXTIFALL]	// trigger at falling edge
	str r2, [r0, #GPIO_EXTIRISE]	// trigger at falling edge
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
	    
/*------start of main code-------*/


main:	
	ldr r0, =GPIO_PA_BASE	
	ldr r4, =buttons			//led address
	ldr r5, [r4]
	cmp r5, #1
	beq shift_led_left
	cmp r5, #0x40
	beq shift_led_right
	b blink   	

shift_led_right:
	ldr r0, =GPIO_PA_BASE
	ldr r5, =leds			// led address
	ldr r2, [r5]			// led values
	mov r3, #0xff00			// bits to toggle
	eor r4, r2, r3			// toggle led bits
	str r4, [r0, #GPIO_DOUT]// write output leds
	cmp r2, 0x8000			// checks if led is at the last stop
	beq reset_right			// reset the led position
	lsl r2, r2, #1			// shift led to the right on the board
right_end:	
	str r2, [r5]
	bl delay			// delay between shifts
	b main

reset_right:
	mov r2, 0x0100		// reset led position
	b right_end			// return to right shift loop			
			
shift_led_left:
	ldr r0, =GPIO_PA_BASE
	ldr r5, =leds			// led address
	ldr r2, [r5]			// led values
	mov r3, #0xff00			// bits to toggle
	eor r4, r2, r3			// toggle led bits
	str r4, [r0, #GPIO_DOUT]// write output leds
	cmp r2, 0x0100			// checks if led is at the last stop
	beq reset_left			// reset the led position
	lsr r2, r2, #1			// shift led to the left on the board
left_end:	
	str r2, [r5]
	bl delay			// delay between shifts
	b main

reset_left:
	mov r2, 0x8000		// reset led position
	b right_end			// return to left shift loop			
			

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

delay: 
	mov r1, #0x100000	// number of loops
delay_loop:
	sub	r1, r1, #1		// i--
	cmp r1, #0			// compare against zero
	bne delay_loop		// loop if not zero
	bx lr				// return	

	
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
/*
	// C equivalent: buttons ^= interrupt_flag 
	
	ldr r8, =GPIO_BASE
	ldr r9, [r8, GPIO_IF]	// read interrupt flags
	ldr r10, =buttons		// read button address
	ldr r11, [r10]			// read previous buttons
	mov r12, r9				// make a copy of interrupt flags
	lsl r12, r12, #8		// shift interrupt flags up
	eor r12, r12, r11		// toggle pressed button with interrupt flags
	str r12, [r10]			// store button presses
	str r9, [r8, GPIO_IFC]	// clear interrupt flags
	bx lr 
	*/
	/*
	ldr r1, =GPIO_BASE
	ldr r3, [r1, GPIO_IF]	// read interrupt flags
	ldr r4, =leds 			//led address
	ldr r5, [r4]			//load led value
	mov r6, #0xff00			//for toggeling
	eor r5, r5, r6			// toggle led
	str r5, [r4]			// store toggled led
	str r3, [r1, GPIO_IFC]	// clear interrupt flags
	bx lr 
	
	*/
	
	/*
	str r3, [r1, GPIO_IFC]	// clear interrupt flags
	cmp r3, #0x1
	beq shift_led_left  	//if sw1 pressed then shift led left
	cmp r3, #0x2
	beq blink
	cmp r3, #0x4
	beq shift_led_right
	bx lr
	*/
	/////////////////////////////////////////////////////////////////////////////
	
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
	
