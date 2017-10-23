/*
 * main.c
 *
 *  Created on: 16 oct. 2017
 *      Author: leroy
 */


// Source code under CC0 1.0

#include "../inc/typedef.h"
#include "../inc/stm8s003.h"
#include "../inc/eeprom.h"

#define rmi() \
{ __asm__("rim"); } /* Enable interrupts */

#define wfi() \
{ __asm__("wfi\n"); } /* Wait For Interrupt */


void putchar(char c)
{
    while(!(USART1_SR & USART_SR_TXE));

    USART1_DR = c;
}

void TIM4_OVF() __interrupt 23
{
    /* reset the received data */

}

void USARTRX_DataFull() __interrupt 18
{
    unsigned char c;
    c = USART1_DR;
    add_to_received(c);

    /* sign of life */
    GPIO_Toggle_Pin(GPIO_PORT_D, GPIO_PIN_0);


}

void setup_gpio() {
    /* Unused I/O pins must not be left floating to avoid extra current consumption. They must be
put into one of the following configurations:
• connected to V DD or V SS by external pull-up or pull-down resistor and kept as input
floating (reset state),
• configured as input with internal pull-up/down resistor,
• configured as output push-pull low.
 */
    GPIO_Config_Pin(GPIO_PORT_D, GPIO_PIN_6, PIN_MODE_INPUT_PU_NO_INT);
    GPIO_Config_Pin(GPIO_PORT_D, GPIO_PIN_5, PIN_MODE_OUTPUT_PP);
    GPIO_Config_Pin(GPIO_PORT_D, GPIO_PIN_0, PIN_MODE_OUTPUT_PP);
}

void setup_uart() {
    USART1_CR2 = USART_CR2_TEN | USART_CR2_REN | USART_CR2_RIEN; // Allow TX and RX
    USART1_CR3 &= ~(USART_CR3_STOP1 | USART_CR3_STOP2); // 1 stop bit
    USART1_BRR2 = 0x03; USART1_BRR1 = 0x68; // 9600 baud
}

void enter_wait_mode() {
    /* When an internal or external interrupt request occurs, the CPU wakes-up from Wait mode
and resumes processing.*/
}

void main(void)
{
	CLK_Init(CLK_SRC_HSI, CLK_HSI_DIV_NONE, CLK_CPU_DIV_MASTER_NONE);
    CLK_Enable(PCKEN1_TIM4 | PCKEN1_UART1, 0x00);
    setup_gpio();

    setup_uart();

    eeprom_init();

    rmi();

    for(;;)
	{
        /* wait for start frame ID */
        do {
            wfi();
        }
        while(1);
	}
}

