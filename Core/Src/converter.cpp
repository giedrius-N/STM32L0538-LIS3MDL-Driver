/*
 * converter.c
 *
 *  Created on: Mar 3, 2024
 *      Author: gnavi
 */
#include "converter.h"

int16_t twos_comp_to_dec(uint16_t value)
{
    uint16_t sign_mask = 0x8000;

    // if positive
    if ( (value & sign_mask) == 0 ) {
        return value;
    //  if negative
    } else {
        // invert all bits, add one, and make negative
        return -(~value + 1);
    }
}


