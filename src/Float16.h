#pragma once

#include <cstring>

// I didn't design this function, but it's far from the scope of the assignment
// so I assume it doesn't matter
// The comments are mine, though

// src: https://stackoverflow.com/questions/22210684/16-bit-floats-and-gl-half-float
short floatToFloat16(float value){
    short fltInt16;
    int fltInt32;

    // Move float into an int for bitwise operations
    memcpy(&fltInt32, &value, 4);

    // Exclude sign bit
    // Mantissa is shortened by the bit-shift
    // Exponent adjusted by subtraction of 112
    // Higher 3 bits of exponent lost when val assigned to a 16 bit variable
    fltInt16 = ((fltInt32 & 0x7fffffff) >> 13) - (0x38000000 >> 13);
    // Copy over sign bit
    fltInt16 |= ((fltInt32 & 0x80000000) >> 16);

    return fltInt16;
}
