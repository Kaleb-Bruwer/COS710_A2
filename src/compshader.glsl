#version 440

const int nRecords = 899;

// Assumed to be same size as number of trees (starts)
layout(std430, binding = 0) writeonly buffer fit{
    float fitness[];
};

layout(std430, binding = 1) buffer GL_data{
    float floats[8 * 899];
    int ints[62 * 899];
};

layout(std430, binding = 2) buffer trees{
    uvec2 nodes[];
};

layout(std430, binding = 3) buffer startIndexes{
    uint starts[];
};

layout(local_size_x = nRecords, local_size_y = 1, local_size_z = 1) in;

int nParameters(uvec2 node){
    if(node.x != 0)
        return 0;
    if(node.y == 6 || node.y == 37)
        return 1;
    return 2;
}

// 0: int, 1:float, -1: none
int inputType(uvec2 node, uint param){
    if(param == 0){
        switch(node.y){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 37:
                return 0;
            case 6:
            case 33:
            case 34:
            case 35:
            case 36:
                return 1;
        }
    }
    else{
        switch(node.y){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                return 0;
            case 33:
            case 34:
            case 35:
            case 36:
                return 1;
            case 6:
            case 37:
                return -1;
        }
    }
}

void main() {
    // uint ident = gl_GlobalInvocationID.x;
    uint r = gl_LocalInvocationID.x; //Data record
    uint nodeIndex = starts[gl_WorkGroupID.x];

    int compStackI[32];
    float compStackF[32];
    int stackIndexI = 0;
    int stackIndexF = 0;

    while(nodes[nodeIndex] != uvec2(0,0)){
        // Go to 1st function node
        while(nodes[nodeIndex].x != 0)
            nodeIndex--;

        int leftI, rightI;
        float leftF, rightF;

        int nParams = nParameters(nodes[nodeIndex]);
        uint filled = 0;

        if(nodes[nodeIndex+1].x != 0){
            if(nodes[nodeIndex+1].x == 2) //int
                leftI = ints[nRecords*nodes[nodeIndex+1].y + r];
            else//float
                leftF = floats[nRecords*nodes[nodeIndex+1].y + r];
            filled++;

            if(nodes[nodeIndex+2].x != 0){
                if(nodes[nodeIndex+2].x == 2) //int
                    rightI = ints[nRecords*nodes[nodeIndex+2].y + r];
                else//float
                    rightF = floats[nRecords*nodes[nodeIndex+2].y + r];
                filled++;
            }
        }
        // If not enough terminals, take values from stack
        if(filled < nParams){
            if(filled == 0){
                if(inputType(nodes[nodeIndex], 0) == 0){
                    leftI = compStackI[stackIndexI-1];
                    stackIndexI--;
                }
                else{
                    leftF = compStackF[stackIndexF-1];
                    stackIndexF--;
                }
                filled++;
            }
            if(filled < nParams){
                if(inputType(nodes[nodeIndex], 0) == 0){
                    rightI = compStackI[stackIndexI-1];
                    stackIndexI--;
                }
                else{
                    rightF = compStackF[stackIndexF-1];
                    stackIndexF--;
                }
            }
        }

        // calc result, put on stack
        if(nodes[nodeIndex].y < 32){
            int result;
            switch(nodes[nodeIndex].y){
                case 1: //+
                    result = leftI + rightI;
                    break;
                case 2: //-
                    result = leftI - rightI;
                    break;
                case 3: //*
                    result = leftI * rightI;
                    break;
                case 4: //divide
                    if(rightI == 0) rightI = 1;
                    result = leftI / rightI;
                    break;
                case 5: //mod
                    result = leftI % max(rightI, 1);
                    break;
                case 6: //round
                    result = int(leftF);
                    break;
            }
            compStackI[stackIndexI] = result;
            stackIndexI++;
        }
        else{
            float result;

            switch(nodes[nodeIndex].y){
                case 33: //+
                    result = leftF + rightF;
                    break;
                case 34: //-
                    result = leftF - rightF;
                    break;
                case 35: //*
                    result = leftF * rightF;
                    break;
                case 36: //divide
                    if(rightF == 0) rightF = 1;
                    result = leftF / rightF;
                    break;
                case 37:
                    result = float(leftI);
                    break;
            }
            compStackF[stackIndexF] = result;
            stackIndexF++;
        }
        nodeIndex--;
    }
    fitness[gl_WorkGroupID.x] = compStackI[0];
}
