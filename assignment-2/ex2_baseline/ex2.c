/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */
/*
 * The period between sound samples, in clock cycles 
 */

#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "melodies.h"
#include "buttons.h"

/*
 * Declaration of peripheral setup functions 
 */

void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();
void setupGPIO();

//void updateLeds(uint16_t button);
//uint16_t readButtons(void);


/*
 * Your code will start executing here 
 */

enum tempos {
    FAST_TEMPO = 44100,
    NORMAL_TEMPO = 88200,
    SLOW_TEMPO = 176400
};

int main(void)
{
    setupGPIO();
    setupDAC();
    setupTimer(317);
    setupNVIC();

    uint32_t tempo = FAST_TEMPO;
    uint16_t amplitude = MAX_VOLUME;
    uint8_t desiredMelody = 0;

    while (1) {
        if (readButtons() > 0) {
            updateLeds(readButtons());
            desiredMelody = readButtons();
        }

        playMelody(&desiredMelody, &amplitude, tempo);
    }
    return 0;
}

void setupNVIC()
{
    /*
	 * TODO use the NVIC ISERx registers to enable handling of
	 * interrupt(s) remember two things are necessary for interrupt
	 * handling: - the peripheral must generate an interrupt signal - the
	 * NVIC must be configured to make the CPU handle the signal You will
	 * need TIMER1, GPIO odd and GPIO even interrupt handling for this
	 * assignment. 
	 */
}

/*
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler 
 */
