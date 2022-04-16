#version 430 core

layout(std430, binding = 0) writeonly buffer SSBO {
    float data[];
};

layout(std430, binding = 0) buffer GL_data {
    float floats[8 * 899];
    int ints[62 * 899];
};


layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

void main() {
    uint ident = gl_GlobalInvocationID.x;
    uint val = gl_WorkGroupID.x;
    data[ident] = val;
}
