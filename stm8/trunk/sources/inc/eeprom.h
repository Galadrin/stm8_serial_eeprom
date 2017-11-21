//
// Created by leroy on 20/10/17.
//

#ifndef LT200_DATAPLUG_EEPROM_H
#define LT200_DATAPLUG_EEPROM_H

#include "../stm8_hal/stm8s.h"

#define START_FRAME     0xF8

#define CMD_WRITE       0x55
#define CMD_READ        0xAA
#define CMD_ACK_READ    0x0F

typedef struct frame_struct_t {
    uint8_t flag;
    uint8_t cmd;
    uint8_t size;
    uint8_t data[128];
    uint16_t crc;
};

typedef union frame_t {
    uint8_t frame [1+1+1+128+2];
    struct frame_struct_t frame_struct;
};

extern union frame_t temp_frame, valid_frame;
extern uint8_t have_valid_frame, have_error_frame;

void eeprom_init(void);
void reset_frame(void);
void add_to_received(unsigned char c);

ErrorStatus store_frame(void);
void read_eeprom(struct frame_struct_t* buff, uint8_t nb_data);


#endif //LT200_DATAPLUG_EEPROM_H
