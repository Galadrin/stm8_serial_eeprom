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

/*!
 * eeprom_init - here is the eeprom initialisation
 */
void eeprom_init(void) {

}

/*!
 * reset_frame - reset all values to zero
 */
void reset_frame(void)
{
    if (rcv_index > 0) {
        rcv_index = 0;
        memset ( &flags, 0x00, sizeof(flags));
        memset ( temp_frame.frame, 0x00, sizeof(temp_frame));
        have_error_frame = 1;
    }
}

/*!
 * add_to_received - this function manage the received byte.
 * @param c - the received byte.
 */
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
            /* avoid useless storage and memory overflow
             * NACK frame will be sent by Timer4 timeout
             */
            if (rcv_index == 0 || rcv_index >= sizeof(temp_frame.frame_struct)) {
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
            rcv_index = 0;
            memset ( &flags, 0x00, sizeof(flags));
            memset ( temp_frame.frame, 0x00, sizeof(temp_frame));
            have_valid_frame = 1;
        } 
        else {
	    /* CRC error, reset frame */
            have_error_frame = 1;
        }
        TIM4_Cmd(ENABLE);
    }
}

/*!
 * store_frame - write a received valid frame to the eeprom
 * @return SUCCESS or ERROR
 */
ErrorStatus store_frame(void) {
    uint8_t index = 0;

    if(have_valid_frame && (valid_frame.frame_struct.cmd == CMD_WRITE)) {
        /*Define FLASH programming time*/
        FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
        /* Unlock Data memory */
        FLASH_Unlock(FLASH_MEMTYPE_DATA);

        for (index = 0; index <= valid_frame.frame_struct.size; index++) {
            /* Program byte at address */
            FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS + index,
                              valid_frame.frame_struct.data[index]);
            while (FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET);
            if (FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS + index) != valid_frame.frame_struct.data[index]){
                FLASH_Lock(FLASH_MEMTYPE_DATA);
                return ERROR;
            }
        }
        FLASH_Lock(FLASH_MEMTYPE_DATA);
    }
    return SUCCESS;
}

/*!
 * read_eeprom - read the nb_data bytes from eeprom
 * @param buff - the buffer were read data are stored
 * @param nb_data - the number of byte to read
 */
void read_eeprom(struct frame_struct_t* buff, uint8_t nb_data)
{
    uint8_t index;

    buff->flag = START_FRAME;
    buff->cmd    = CMD_ACK_READ;
    buff->size   = nb_data;
    for (index = 0; index < nb_data; index ++ ) {
        buff->data[index] = FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS + index);
    //    buff->data[index] = index;
    }
    //crc = crc16(0x00, (const uint8_t *) buff, (uint16_t) (buff->size + 3));
    //buff->data[index++] = 0x55;
    //buff->data[index] = 0x78;
    //memcpy(&buff->data[index], &crc, sizeof(crc));
    //buff->data[index++] = (uint8_t) ((crc & 0xFF00) >> 8);
    //buff->data[index] = (uint8_t) ((crc & 0x00FF));
}

