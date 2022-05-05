#include "Fitness.h"

#include <cmath>

float mean_squared_error(int* target, int* result, int len){
    float error = 0;

    for(int i=0; i<len; i++){
        float loss = pow(target[i] - result[i], 2);
        error += loss;
    }
    return error/len;
}

float mean_error(int* target, int* result, int len){
    float error = 0;

    for(int i=0; i<len; i++){
        float loss = abs(target[i] - result[i]);
        error += loss;
    }
    return error/len;
}

float hitrate(int* target, int* result, int len){
    int hits = 0;

    for(int i=0; i<len; i++){
        if(target[i] == result[i])
            hits++;
    }
    return hits/(float)len;
}
