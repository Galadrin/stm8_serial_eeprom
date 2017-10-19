/*
 * main.c
 *
 *  Created on: 16 oct. 2017
 *      Author: leroy
 */


// Source code under CC0 1.0
#include <stdint.h>
#include <stdio.h>
#include "../inc/stm8s.h"

void select_master_clock() {
    /* Set the Clock option */

    /* Set SWIEN bit in CLK_SWCR to enable int if suitable */
    /* select target clock source in CLK_SWR */

    /* wait until CLK_SWCR.SWIF  = 1 */

    /* clear the CLK_SWCR.SWIF flag */
    /* enable the CLK_SWCR.SWEN to enable the clock */
}

void peripheral_clock_gating() {
    /* After a device reset, all peripheral clocks are enabled. You can disable the clock to any
peripheral by clearing the corresponding PCKEN bit in the Peripheral clock gating register 1
(CLK_PCKENR1) and in the Peripheral clock gating register 2 (CLK_PCKENR2). But you
have to disable properly the peripheral using the appropriate bit, before stopping the
corresponding clock.
To enable a peripheral, you must first enable the corresponding PCKEN bit in the
CLK_PCKENR registers and then set the peripheral enable bit in the peripheral’s control
registers. */
    CLK_PCKENR1 &= PCKEN1_TIM4 | PCKEN1_UART1;
    CLK_PCKENR2 &= 0x00;
}

void setup_gpio() {
    /* Unused I/O pins must not be left floating to avoid extra current consumption. They must be
put into one of the following configurations:
• connected to V DD or V SS by external pull-up or pull-down resistor and kept as input
floating (reset state),
• configured as input with internal pull-up/down resistor,
• configured as output push-pull low.
 */
    PA_DDR = 0x00;
    PA_CR1 = 0xFF;
    PA_CR2 = 0x00;

    PB_DDR = 0x00;
    PB_CR1 = 0xFF;
    PB_CR2 = 0x00;

    PC_DDR = 0x00;
    PC_CR1 = 0xFF;
    PC_CR2 = 0x00;

    PD_DDR = 0x00;
    PD_CR1 = 0xFF;
    PD_CR2 = 0x00;

    PE_DDR = 0x00;
    PE_CR1 = 0xFF;
    PE_CR2 = 0x00;

    PF_DDR = 0x00;
    PF_CR1 = 0xFF;
    PF_CR2 = 0x00;

}

void enter_wait_mode() {
    /* When an internal or external interrupt request occurs, the CPU wakes-up from Wait mode
and resumes processing.*/
}

void putchar(char c)
{
	while(!(UART1_SR & UART_SR_TXE));

	UART1_DR = c;
}

void main(void)
{
	unsigned long i = 0;

    CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
    CLK_PCKENR1 |= 0xFF; // Enable peripherals

	UART1_CR2 = UART_CR2_TEN; // Allow TX and RX
	UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
	UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; // 9600 baud

	for(;;)
	{
        printf("\xAA\x55\x00\x55\xAA");
        for (i = 0; i < 10000; i++); // Sleep
	}
}
