//
// Created by leroy on 19/10/17.
//

#ifndef LT200_DATAPLUG_STM8S_H_H
#define LT200_DATAPLUG_STM8S_H_H

/* PORT A */
#define PA_ODR      (*(volatile uint8_t *)0x5000)   /* data output latch register */
#define PA_IDR      (*(volatile uint8_t *)0x5001)   /* input pin value register */
#define PA_DDR      (*(volatile uint8_t *)0x5002)   /* data direction register */
#define PA_CR1      (*(volatile uint8_t *)0x5003)   /* control register 1 */
#define PA_CR2      (*(volatile uint8_t *)0x5004)   /* control register 2 */

/* PORT B */
#define PB_ODR      (*(volatile uint8_t *)0x5005)
#define PB_IDR      (*(volatile uint8_t *)0x5006)
#define PB_DDR      (*(volatile uint8_t *)0x5007)
#define PB_CR1      (*(volatile uint8_t *)0x5008)
#define PB_CR2      (*(volatile uint8_t *)0x5009)

/* PORT C */
#define PC_ODR      (*(volatile uint8_t *)0x500A)
#define PC_IDR      (*(volatile uint8_t *)0x500B)
#define PC_DDR      (*(volatile uint8_t *)0x500C)
#define PC_CR1      (*(volatile uint8_t *)0x500D)
#define PC_CR2      (*(volatile uint8_t *)0x500E)


/* PORT D */
#define PD_ODR      (*(volatile uint8_t *)0x500F)
#define PD_IDR      (*(volatile uint8_t *)0x5010)
#define PD_DDR      (*(volatile uint8_t *)0x5011)
#define PD_CR1      (*(volatile uint8_t *)0x5012)
#define PD_CR2      (*(volatile uint8_t *)0x5013)

/* PORT E */
#define PE_ODR      (*(volatile uint8_t *)0x5014)
#define PE_IDR      (*(volatile uint8_t *)0x5015)
#define PE_DDR      (*(volatile uint8_t *)0x5016)
#define PE_CR1      (*(volatile uint8_t *)0x5017)
#define PE_CR2      (*(volatile uint8_t *)0x5018)

/* PORT F */
#define PF_ODR      (*(volatile uint8_t *)0x5019)
#define PF_IDR      (*(volatile uint8_t *)0x501A)
#define PF_DDR      (*(volatile uint8_t *)0x501B)
#define PF_CR1      (*(volatile uint8_t *)0x501C)
#define PF_CR2      (*(volatile uint8_t *)0x501D)

/* Flash */
#define FLASH_CR1   (*(volatile uint8_t *)0x505A)   /* Flash control register 1 */
#define FLASH_CR2   (*(volatile uint8_t *)0x505B)   /* Flash control register 2 */
#define FLASH_NCR2  (*(volatile uint8_t *)0x505C)   /* Flash complementary control register 2 */
#define FLASH_FPR   (*(volatile uint8_t *)0x505D)   /* Flash protection register */
#define FLASH_NFPR  (*(volatile uint8_t *)0x505E)  /* Flash complementary protection register */
#define FLASH_IAPSR (*(volatile uint8_t *)0x505F)  /* Flash in-application programming status register */

#define FLASH_PUKR  (*(volatile uint8_t *)0x5062)  /* Flash Program memory unprotection register */

#define FLASH_DUKR  (*(volatile uint8_t *)0x5064)  /* Flash EEPROM unprotection register */

/* RESET */
#define RST_SR      (*(volatile uint8_t *)0x50B3)   /* Reset status register */

/* CLOCK */
#define CLK_ICKR    (*(volatile uint8_t *)0x50c0)   /* Internal clock control register  */
#define CLK_ECKR    (*(volatile uint8_t *)0x50c1)   /* External clock control register */

#define CLK_CMSR    (*(volatile uint8_t *)0x50c3)   /* Clock master status register */
#define CLK_SWR     (*(volatile uint8_t *)0x50c4)   /* Clock master switch register  */
#define CLK_SWCR    (*(volatile uint8_t *)0x50c5)   /* Clock switch control register  */
#define CLK_CKDIVR    (*(volatile uint8_t *)0x50c6)   /* Clock divider register */
#define CLK_PCKENR1    (*(volatile uint8_t *)0x50c7)   /* Peripheral clock gating register 1 */
#define CLK_CSSR    (*(volatile uint8_t *)0x50c8)   /* Clock security system register */
#define CLK_CCOR    (*(volatile uint8_t *)0x50c9)   /* Configurable clock control register */
#define CLK_PCKENR2    (*(volatile uint8_t *)0x50cA)   /* Peripheral clock gating register 2 */
#define CLK_HSITRIMR    (*(volatile uint8_t *)0x50cc)   /* HSI clock calibration trimming register */
#define CLK_SWIMCCR    (*(volatile uint8_t *)0x50cd)   /* SWIM clock control register  */

/* PCKENR1/2 bits */
#define PCKEN1_TIM1  0x80
#define PCKEN1_TIM2  0x20
#define PCKEN1_TIM4  0x10
#define PCKEN1_UART1 0x08
#define PCKEN1_SPI   0x20
#define PCKEN1_I2C   0x10

#define PCKEN2_ADC  0x08
#define PCKEN2_AWU  0x40


/* UART1 */

#define UART1_SR    (*(volatile uint8_t *)0x5230)   /* UART1 status register */
#define UART1_DR    (*(volatile uint8_t *)0x5231)   /* UART1 data register */
#define UART1_BRR1    (*(volatile uint8_t *)0x5232)   /* UART1 baud rate register 1 */
#define UART1_BRR2    (*(volatile uint8_t *)0x5233)   /* UART1 baud rate register 2 */
#define UART1_CR1    (*(volatile uint8_t *)0x5234)   /* UART1 control register 1 */
#define UART1_CR2    (*(volatile uint8_t *)0x5235)   /* UART1 control register 2 */
#define UART1_CR3    (*(volatile uint8_t *)0x5236)   /* UART1 control register 3 */
#define UART1_CR4    (*(volatile uint8_t *)0x5237)   /* UART1 control register 4 */
#define UART1_CR5    (*(volatile uint8_t *)0x5238)   /* UART1 control register 5 */
#define UART1_GTR    (*(volatile uint8_t *)0x5239)   /* UART1 guard time register */
#define UART1_PSCR    (*(volatile uint8_t *)0x523A)   /* UART1 prescaler register  */


#define UART_CR2_TEN (1 << 3)
#define UART_CR3_STOP2 (1 << 5)
#define UART_CR3_STOP1 (1 << 4)
#define UART_SR_TXE (1 << 7)

#endif //LT200_DATAPLUG_STM8S_H_H
