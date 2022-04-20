#pragma once

#include <iostream>

/*
GLSL won't work with anything smaller than 32 bits, so this is a neccesary
waste of memory. GLSL will receive this as a uvec2
*/

struct GPUNode{
    unsigned int type;
    unsigned int val;
    friend std::ostream& operator<<(std::ostream& out, const GPUNode &n);
};
