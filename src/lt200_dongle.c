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
#include "../inc/crc16.h"

#define ACK "\xF8\xF0\x01\xFF\x83\x70"
#define NACK "\xF8\xF0\x01\x00\xC3\x30"

#undef DISCOVERY
//#define DISCOVERY
#ifdef DISCOVERY
#define GPIO_LED    GPIOD, GPIO_PIN_0
#else
#define GPIO_LED    GPIOD, GPIO_PIN_1
#endif

#define GPIO_D3     GPIOD, GPIO_PIN_3
#define GPIO_C4     GPIOC, GPIO_PIN_4
#define GPIO_C6     GPIOC, GPIO_PIN_6

void putchar(uint8_t c) {
    while ((UART1->SR & (u8) UART1_FLAG_TXE) == RESET);
    UART1_SendData8(c);
}

INTERRUPT_HANDLER(TIM4_handler, ITC_IRQ_TIM4_OVF)
{
    GPIO_WriteReverse(GPIO_D3);

    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    reset_frame();

    GPIO_WriteLow(GPIO_C4);
}

INTERRUPT_HANDLER(UART1_rxFull, ITC_IRQ_UART1_RX)
{
    unsigned char c;
    UART1_GetFlagStatus(UART1_FLAG_OR);
//    c = UART1->DR;
    c = UART1_ReceiveData8();
    GPIO_WriteReverse(GPIO_C4);
    TIM4_Cmd(DISABLE);
    TIM4_SetCounter(0x00);
    TIM4_Cmd(ENABLE);
//    putchar(c);
    add_to_received(c);
}

void setup_clock() {

    CLK_DeInit();
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);


#ifdef DISCOVERY
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE,
                          CLK_CURRENTCLOCKSTATE_DISABLE);
#else
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE,
                          CLK_CURRENTCLOCKSTATE_DISABLE);
    /* wait until external clock establishment */
    while(CLK_GetSYSCLKSource() != CLK_SOURCE_HSE);

#endif
    /* Disable all peripherals and enable only timer and UART */
    CLK->PCKENR1 = 0x00;
    CLK->PCKENR2 = 0x00;
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);

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
    GPIO_Init(GPIO_D3, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIO_C4, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIO_C6, GPIO_MODE_OUT_PP_LOW_FAST);
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
    uint32_t prescal = 0;
    uint32_t timer_freq = 500;

    TIM4_DeInit();
    /* setup timer at 2ms */
    prescal = ((uint32_t)CLK_GetClockFreq() / 128) / timer_freq;
    //prescal = ((uint32_t)10000000 / 128) / timer_freq;

    TIM4_TimeBaseInit(TIM4_PRESCALER_128, (uint8_t) prescal);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);
}

void enter_wait_mode() {
    /* When an internal or external interrupt request occurs, the CPU wakes-up from Wait mode
and resumes processing.*/
}

void send_ack(void){
    int i;
    for (i = 0; i < sizeof(ACK) - 1; i++) {
        putchar((uint8_t) ACK[i]);
    }
}

void send_nack(void) {
    int i;
    for (i = 0; i < sizeof(NACK) - 1; i++)
        putchar((uint8_t) NACK[i]);
}

void main(void) {
    disableInterrupts();

    setup_clock();
    setup_gpio();
    /* Configure the UART1 */
    setup_uart();
    /* Configure Timer */
    setup_timer();
    eeprom_init();
    enableInterrupts();

    CLK_CCOConfig(CLK_OUTPUT_MASTER);

GPIO_WriteLow(GPIO_LED);
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
                    uint16_t crc = 0x00;
                    if (valid_frame.frame_struct.data[0] > 128) {
                        send_nack();
                    } else {
                        read_eeprom(&frame_to_send.frame_struct, valid_frame.frame_struct.data[0]);
                        crc = crc16(0x00, (const uint8_t *) &frame_to_send.frame,
                                    (uint16_t) (frame_to_send.frame_struct.size + 3));
                        putchar(frame_to_send.frame_struct.flag);
                        putchar(frame_to_send.frame_struct.cmd);
                        putchar(frame_to_send.frame_struct.size);
                        for (i = 0; i < frame_to_send.frame_struct.size; i++)
                            putchar(frame_to_send.frame_struct.data[i]);
                        // crc 16
                        putchar((uint8_t) (crc >> 8));
                        putchar((uint8_t) (crc & 0x00FF));
                    }
                }
                have_valid_frame = 0;
                GPIO_WriteLow(GPIO_LED);
            }
            if(have_error_frame) {
                GPIO_WriteHigh(GPIO_LED);
                send_nack();
                have_error_frame = 0;
                GPIO_WriteLow(GPIO_LED);
            }
    }
}
