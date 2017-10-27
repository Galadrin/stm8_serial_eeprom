/*
 * main.c
 *
 *  Created on: 16 oct. 2017
 *      Author: leroy
 */


// Source code under CC0 1.0

#include <stdio.h>
#include "../stm8_hal/stm8s.h"
#include "../inc/eeprom.h"
#include "../stm8_hal/stm8s_uart1.h"

#define ACK "\xF8\xF0\x01\xFF\x83\x70"
#define NACK "\xF8\xF0\x01\x00\xC3\x30"

#undef DISCOVERY
//#define DISCOVERY
#ifdef DISCOVERY
#define GPIO_LED    GPIOD, GPIO_PIN_0
#else
#define GPIO_LED    GPIOD, GPIO_PIN_1
#endif

void putchar(uint8_t c) {
    while ((UART1->SR & (u8) UART1_FLAG_TXE) == RESET);
    UART1_SendData8(c);
}

INTERRUPT_HANDLER(TIM4_handler, ITC_IRQ_TIM4_OVF)
{
    GPIO_WriteReverse(GPIO_LED);

    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    reset_frame();
}

INTERRUPT_HANDLER(UART1_rxFull, ITC_IRQ_UART1_RX)
{
    unsigned char c;
    c = UART1->DR;
    TIM4_Cmd(DISABLE);
    TIM4_SetCounter(0x00);
    TIM4_Cmd(ENABLE);
    add_to_received(c);
}

void setup_gpio() {
    /* Unused I/O pins must not be left floating to avoid extra current consumption. They must be
put into one of the following configurations:
• connected to V DD or V SS by external pull-up or pull-down resistor and kept as input
floating (reset state),
• configured as input with internal pull-up/down resistor,
• configured as output push-pull low.
 */
    /* UART GPIO */
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);

    /* GPIOD_0 => LED carte eval */
    //GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);

    /* GPIOB_4 => LED carte définitive */
    GPIO_Init(GPIO_LED, GPIO_MODE_OUT_PP_LOW_FAST);
}

void setup_uart() {
    UART1_DeInit();
    /* UART1 and UART3 configured as follow:
          - BaudRate = 9600 baud
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Receive and transmit enabled
          - UART1 Clock disabled
     */
    /* Configure the UART1 */
    UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
               UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    /* Enable UART1 Transmit interrupt*/
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    UART1_Cmd(ENABLE);
}

void setup_timer() {
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 250);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);
}

void enter_wait_mode() {
    /* When an internal or external interrupt request occurs, the CPU wakes-up from Wait mode
and resumes processing.*/
}

void send_ack(void){
    int i;
    putchar(sizeof(ACK));
    for (i = 0; i < sizeof(ACK); i++) {
        putchar((uint8_t) ACK[i++]);
    }
}

void send_nack(void) {
    int i;
    for (i = 0; i < sizeof(NACK); i++)
        putchar((uint8_t) NACK[i++]);
}

void main(void) {
    disableInterrupts();

    CLK_DeInit();
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE,
                          CLK_CURRENTCLOCKSTATE_DISABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);

    /* Configure the UART1 */
    setup_gpio();
    setup_uart();
    setup_timer();
    eeprom_init();

    enableInterrupts();

    for (;;) {
        wfi();
            if(have_valid_frame) {
                GPIO_WriteHigh(GPIO_LED);
                if (valid_frame.frame_struct.cmd == CMD_WRITE) {
                    if (store_frame() == SUCCESS)
                        send_ack();
                    else
                        send_nack();
                }
                if (valid_frame.frame_struct.cmd == CMD_READ) {
                    union frame_t frame_to_send;
                    uint8_t i;
                    read_eeprom(&frame_to_send.frame_struct, valid_frame.frame_struct.data[0]);

                    putchar(frame_to_send.frame_struct.flag);
                    putchar(frame_to_send.frame_struct.cmd);
                    putchar(frame_to_send.frame_struct.size);
                    for (i = 0; i < frame_to_send.frame_struct.size; i++)
                        putchar(frame_to_send.frame_struct.data[i]);
                    // crc 16
                    putchar(frame_to_send.frame[i++]);
                    putchar(frame_to_send.frame[i++]);

                }
                have_valid_frame = 0;
                GPIO_WriteLow(GPIO_LED);
            }
            if(have_error_frame) {
                send_nack();
                have_error_frame = 0;
            }
    }
}
