#include "Helpers.h"

#include <math.h>
#include <stdlib.h>

void shuffle(int* arr, int len){
    for (int i = 0; i < len-1; i++){
        int j = rand() % (len-i) + i;
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

double getAvg(double* arr, int len){
    if(len <= 0)
        return 0;
    double total = 0;
    for(int i=0; i<len; i++){
        total += arr[i];
    }
    total /= len;
    return total;
}

double getStdDev(double* arr, int len){
    double result = 0;
    double mean = getAvg(arr, len);

    for(int i=0; i<len; i++)
        result += fabs(arr[i] - mean);

    return sqrt(result/len);
}

double getMin(double* arr, int len){
    if(len <= 0)
        return 0;

    double minVal = arr[0];

    for(int i=1; i<len; i++){
        if(arr[i] < minVal){
            minVal = arr[i];
        }
    }
    return minVal;
}

double getMax(double* arr, int len){
    if(len <= 0)
        return 0;

    double maxVal = arr[0];

    for(int i=1; i<len; i++){
        if(arr[i] > maxVal){
            maxVal = arr[i];
        }
    }
    return maxVal;
}

int getMinIndex(double* arr, int len){
    if(len <= 0)
        return 0;

    double minVal = arr[0];
    int minIndex = 0;

    for(int i=1; i<len; i++){
        if(arr[i] < minVal){
            minVal = arr[i];
            minIndex = i;
        }
    }
    return minIndex;
}

int countNonInts(double* arr, int len){
    int result = 0;
    if(len <= 0)
        return 0;

    for(int i=0; i<len; i++){
        int round = (int) arr[i];
        if(arr[i] != round){
            result++;
        }
    }
    return result;
}
