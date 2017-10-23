//
// Created by leroy on 20/10/17.
//

#include <string.h>
#include "../inc/eeprom.h"
#include "../inc/crc16.h"
#include "../inc/stm8s003.h"

union frame_t temp_frame;
unsigned char flag, cmd, size, data, crc;

void putc(char c)
{
    while(!(USART1_SR & USART_SR_TXE));

    USART1_DR = c;
}


void add_to_received(unsigned char c)
{
    static unsigned char index = 0;
    temp_frame.frame[index] = c;

    switch (index) {
        case 0 :
            if (temp_frame.frame_struct.flag == START_FRAME) {
                index ++;
                flag = 1;
                putc('F');
            }
            break;
        case 1 :
            if (temp_frame.frame_struct.cmd == CMD_WRITE || temp_frame.frame_struct.cmd == CMD_READ) {
                index ++;
                cmd = 1;
                putc('C');
            }
            break;
        case 2 :
            if (temp_frame.frame_struct.size <= 128) {
                index ++;
                size = 1;
                putc('S');
            }
            break;
        default:
            if (index == 0) {
                break;
            }
            if( 3 <= index <= temp_frame.frame_struct.size + 3) {
                index ++;
                data = 1;
                putc('D');
            }
            if(index == temp_frame.frame_struct.size + 4) {
                temp_frame.frame_struct.crc = (c << 8);
                index ++;
                putc('C');
            }
            if(index == temp_frame.frame_struct.size + 5) {
                crc = 1;
                temp_frame.frame_struct.crc = (c);
                putc('C');
            }
            break;
    }
    if ( flag && cmd && size && data && crc) {
        unsigned short check_crc = crc16(0x00, temp_frame.frame, (uint16_t) (temp_frame.frame_struct.size + 3));
        if (memcmp(check_crc, temp_frame.frame_struct.crc, sizeof(uint16_t)) == 0) {
            putc('1');
            putc((char) (check_crc >> 8));
        } else {
            putc((char) (check_crc & 0x00FF));
        }
    }
}