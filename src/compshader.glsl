#version 430

layout(std430, binding = 0) writeonly buffer SSBO {
    float data[];
};

layout(std430, binding = 1) buffer GL_data {
    float floats[8 * 899];
    int ints[62 * 899];
};

layout(std430, binding = 2) buffer trees {
    uvec2 trees[];
};

layout(std430, binding = 3) buffer startIndexes {
    uint i[];
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

void main() {
    uint ident = gl_GlobalInvocationID.x;
    uint val = gl_WorkGroupID.x;
    data[ident] = val;
}
