//
// Created by leroy on 20/10/17.
//

#ifndef LT200_DATAPLUG_EEPROM_H
#define LT200_DATAPLUG_EEPROM_H

#include "typedef.h"

#define START_FRAME 0xF8

#define CMD_WRITE   0x55
#define CMD_READ    0xAA

union frame_t {
    uint8_t frame [1+1+1+128+2];
    struct frame_struct_t {
        uint8_t flag;
        uint8_t cmd;
        uint8_t size;
        uint8_t data[128];
        uint16_t crc;
    } frame_struct;
};

void add_to_received(unsigned char c);

#endif //LT200_DATAPLUG_EEPROM_H
