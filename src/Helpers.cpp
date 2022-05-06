#include "Helpers.h"

void shuffle(int* arr, int len){
    for (int i = 0; i < len-1; i++){
        int j = rand() % (len-i) + i;
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
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
