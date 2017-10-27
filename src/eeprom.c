//
// Created by leroy on 20/10/17.
//

#include <string.h>
#include <stdio.h>
#include "../inc/eeprom.h"
#include "../inc/crc16.h"

struct {
    unsigned char flag:1;
    unsigned char cmd:1;
    unsigned char size:1;
    unsigned char data:1;
    unsigned char crc:1;
    unsigned char spare:3;
} flags;

union frame_t temp_frame, valid_frame;
uint8_t have_valid_frame, have_error_frame;
static unsigned char rcv_index = 0;

void eeprom_init(void) {
}

void reset_frame(void)
{    
    rcv_index = 0;
    memset ( &flags, 0x00, sizeof(flags));
    memset ( temp_frame.frame, 0x00, sizeof(temp_frame));
}


void add_to_received(unsigned char c)
{
    temp_frame.frame[rcv_index] = c;
    switch (rcv_index) {
        case 0 :
            if (temp_frame.frame_struct.flag == START_FRAME) {
                rcv_index ++;
                flags.flag = 1;
            }
            break;
        case 1 :
            if (temp_frame.frame_struct.cmd == CMD_WRITE) {
                rcv_index ++;
                flags.cmd = 1;
            }
	    else if (temp_frame.frame_struct.cmd == CMD_READ) {
                rcv_index ++;
                flags.cmd = 1;
            }
            break;
        case 2 :
            if (temp_frame.frame_struct.size <= 128) {
                rcv_index ++;
                flags.size = 1;
            }
            break;
        default:
            if (rcv_index == 0) {
                break;
            }
            if ( rcv_index <= (temp_frame.frame_struct.size + 2)) {
                rcv_index ++;
                flags.data = 1;
            }
            else if (rcv_index == (temp_frame.frame_struct.size + 3)) {
                temp_frame.frame_struct.crc = (c << 8);
                rcv_index ++;
            }
            else if (rcv_index == (temp_frame.frame_struct.size + 4)) {
                flags.crc = 1;
                temp_frame.frame_struct.crc |= (c & 0x00FF);
            }
            else {
	    }
            break;
    }
    if ( flags.crc ) {
        unsigned short check_crc;
        /* stop Timer to avoid reset during crc calculation */
        TIM4_Cmd(DISABLE);
        check_crc = crc16(0x00,
					 temp_frame.frame, 
					 (uint16_t) (temp_frame.frame_struct.size + 3));
        if (temp_frame.frame_struct.crc == check_crc) {
            //printf("t: c=%02X s=%02X\r\n", temp_frame.frame_struct.cmd, temp_frame.frame_struct.size);
            memcpy(valid_frame.frame, temp_frame.frame, sizeof(valid_frame.frame));
            have_valid_frame = 1;
        } 
        else {
	    /* CRC error, reset frame */
            have_error_frame = 1;
        }

	    rcv_index = 0;
        memset ( &flags, 0x00, sizeof(flags));
        memset ( temp_frame.frame, 0x00, sizeof(temp_frame));
        TIM4_Cmd(ENABLE);
    }
}

ErrorStatus store_frame(void) {
    uint8_t index = 0;
    union frame_t check;

    if(have_valid_frame && valid_frame.frame_struct.cmd == CMD_WRITE) {
        FLASH_Unlock(FLASH_MEMTYPE_DATA);
        for (index = 0; index < 128; index += 4) {
            // to erase, simply write 0x00
            FLASH_ProgramWord(FLASH_DATA_START_PHYSICAL_ADDRESS + index, 0x00000000);
            FLASH_ProgramWord(FLASH_DATA_START_PHYSICAL_ADDRESS + index,
                              (uint32_t) (valid_frame.frame_struct.data) + index);
        }
        FLASH_Lock(FLASH_MEMTYPE_DATA);
        read_eeprom(&check.frame_struct , valid_frame.frame_struct.size);
        if ( memcmp (check.frame_struct.data, valid_frame.frame_struct.data, valid_frame.frame_struct.size) == 0)
        {
            return SUCCESS;
        }
    }
    return ERROR;
}

void read_eeprom(struct frame_struct_t* buff, uint8_t nb_data)
{
    uint8_t index;
    uint16_t crc;

    buff->flag = START_FRAME;
    buff->cmd    = CMD_ACK_READ;
    buff->size   = nb_data;
    for (index = 0; index < nb_data; index ++ ) {
        // to erase, simply write 0x00
        buff->data[index] = FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS + index);
    }
    crc = crc16(0x00, (const uint8_t *) buff, (uint16_t) (&buff->data[index] - &buff->flag));
    buff->data[index++] = (uint8_t) ((crc & 0xFF00) >> 8);
    buff->data[index] = (uint8_t) ((crc & 0x00FF));
}

