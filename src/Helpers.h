#pragma once

#include <cmath>

// shuffle comes from:
// https://programming-idioms.org/idiom/10/shuffle-a-list/1995/c
void shuffle(int* arr, int len);
int countNonInts(double* arr, int len);


template<class T>
double getAvg(T* arr, int len){
    if(len <= 0)
        return 0;
    double total = 0;
    for(int i=0; i<len; i++){
        total += arr[i];
    }
    total /= len;
    return total;
};


template<class T>
T getMin(T* arr, int len){
    if(len <= 0)
        return 0;

    T minVal = arr[0];

    for(int i=1; i<len; i++){
        if(arr[i] < minVal){
            minVal = arr[i];
        }
    }
    return minVal;
};


template<class T>
T getMax(T* arr, int len){
    if(len <= 0)
        return 0;

    T maxVal = arr[0];

    for(int i=1; i<len; i++){
        if(arr[i] > maxVal){
            maxVal = arr[i];
        }
    }
    return maxVal;
};


template<class T>
int getMinIndex(T* arr, int len){
    if(len <= 0)
        return 0;

    T minVal = arr[0];
    int minIndex = 0;

    for(int i=1; i<len; i++){
        if(arr[i] < minVal){
            minVal = arr[i];
            minIndex = i;
        }
    }
    return minIndex;
};


template<class T>
double getStdDev(T* arr, int len){
    double result = 0;
    double mean = getAvg(arr, len);

    for(int i=0; i<len; i++)
        result += fabs(arr[i] - mean);

    return sqrt(result/len);
};
