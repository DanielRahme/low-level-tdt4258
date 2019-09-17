	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 
		//activate GPIO 
		ldr r1, cmu_base_addr
		ldr r2, [r1, #CMU_HFPERCLKEN0] 
		mov r3, #1
		lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
		orr r2, r2, r3
		str r2, [r1, #CMU_HFPERCLKEN0]
		
		// set port C 0-7 as input
		ldr r1, =GPIO_PC_BASE
	    mov r2, #0x33333333 
	    str r2, [r1, #GPIO_MODEH]
	    mov r3, 0xff
	   	str r3, [r1, #GPIO_DOUT]
		
		
		//;set port A 8-15 to output    
		ldr r0, =GPIO_PA_BASE
	    mov r2, #0x55555555 
	    str r2, [r0, #GPIO_MODEH]
	    
	    //;set high drive strength
	    mov r3, #0x3
	    str r3, [r0, #GPIO_CTRL]
	    
	   
	    //mov r4, #0xfe00 
	    mov r2, #0x10101010
	    mov r3, #0xff00
	main_loop:
	    ror r2, r2, #1
	    eor r4, r2, r3		// toggle r2 bits
	    str r4, [r0, #GPIO_DOUT]	// 
	    b delay
		
	delay: 
		mov r1, #0x100000
	delay_loop:
		sub	r1, r1, #1
		cmp r1, #0
		bne delay_loop
		b main_loop