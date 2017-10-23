//
// Created by leroy on 23/10/17.
//

#ifndef LT200_DATAPLUG_CRC16_H
#define LT200_DATAPLUG_CRC16_H

#include "typedef.h"

/**
 * crc16 - compute the CRC-16 for the data buffer
 * @crc:	previous CRC value
 * @buffer:	data pointer
 * @len:	number of bytes in the buffer
 *
 * Returns the updated CRC value.
 */
uint16_t crc16(uint16_t crc, uint8_t const *buffer, uint16_t len);


#endif //LT200_DATAPLUG_CRC16_H
