//
// Created by leroy on 20/10/17.
//

#include <string.h>
#include <stdio.h>
#include "../inc/eeprom.h"
#include "../inc/crc16.h"
#include "../inc/stm8s003.h"

union frame_t temp_frame;
struct {
    unsigned char flag:1;
    unsigned char cmd:1;
    unsigned char size:1;
    unsigned char data:1;
    unsigned char crc:1;
    unsigned char spare:3;
} flags;

static unsigned char index = 0;

void putc(char c)
{
    while(!(USART1_SR & USART_SR_TXE));

    USART1_DR = c;
}

void eeprom_init(void) {
    index = 0;
    memset ( &flags, 0x00, sizeof(flags));
    memset ( temp_frame.frame, 0x00, sizeof(temp_frame));
}

void add_to_received(unsigned char c)
{
    temp_frame.frame[index] = c;

    switch (index) {
        case 0 :
            if (temp_frame.frame_struct.flag == START_FRAME) {
                index ++;
                flags.flag = 1;
//                putc('F');
            }
            break;
        case 1 :
            if (temp_frame.frame_struct.cmd == CMD_WRITE) {
                index ++;
                flags.cmd = 1;
                //printf("commande %s\r\n", "Write");
            }
	    else if (temp_frame.frame_struct.cmd == CMD_READ) {
                index ++;
                flags.cmd = 1;
                //printf("commande %s\r\n", "READ");
            }
            break;
        case 2 :
            if (temp_frame.frame_struct.size <= 128) {
                index ++;
                flags.size = 1;
//                printf("size %d\r\n", temp_frame.frame_struct.size);
//                printf("d = ");
            }
            break;
        default:
            if (index == 0) {
                break;
            }
            if ( index <= (temp_frame.frame_struct.size + 2)) {
                index ++;
                flags.data = 1;
//                putc(c);
            }
            else if (index == (temp_frame.frame_struct.size + 3)) {
                temp_frame.frame_struct.crc = (c << 8);
                index ++;
//                printf("\r\nC");
            }
            else if (index == (temp_frame.frame_struct.size + 4)) {
                flags.crc = 1;
                temp_frame.frame_struct.crc |= (c & 0x00FF);
//                putc('C');
            }
            else {
		putc('E');
	    }
            break;
    }
    if ( flags.crc ) {
        unsigned short check_crc = crc16(0x00, 
					 temp_frame.frame, 
					 (uint16_t) (temp_frame.frame_struct.size + 3));
	printf("\r\nC = %04X / R = %04X\r\n", check_crc, temp_frame.frame_struct.crc);
        if (temp_frame.frame_struct.crc == check_crc) {
            putc('1');
            //printf("CRC Match\r\n");
        } 
        else {
	    /* CRC error, reset frame */
            putc('0');
        }

	    index = 0;
            memset ( &flags, 0x00, sizeof(flags));
            memset ( temp_frame.frame, 0x00, sizeof(temp_frame));
    }
}
