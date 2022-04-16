#pragma once

// https://programming-idioms.org/idiom/10/shuffle-a-list/1995/c
void shuffle(int* arr, int len);

double getAvg(double* arr, int len);
double getMin(double* arr, int len);
double getMax(double* arr, int len);
int getMinIndex(double* arr, int len);
double getStdDev(double* arr, int len);

int countNonInts(double* arr, int len);
