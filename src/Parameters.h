#pragma once

const int POPULATION = 10000;
const int GENERATIONS = 200;

const int MAX_DEPTH = 6;
const int CHROMOSOME_LEN = 128;

const int TRAIN_COUNT = 700;

//size must be >= winners*(multiplication+1)
const int TOURNAMENT_SIZE = 6;
const int TOURNAMENT_WINNERS = 1;
const int TOURNAMENT_MULTIPLICATION = 3;

const int CONST_CHANCE = 200; //fraction out of 1000
const int GROW_RATE = 750;

// [Crossover, Mutation, isEmpty]
const int OP_WEIGHTS[] = {7,1};

// terminal, +, -, *, /, mod, round, if
const int RET_INT_W[] = {3, 1, 1, 0, 1, 1, 2, 0};
// terminal, +, -, *, /, toFloat, if
const int RET_FLOAT_W[] = {3, 1, 1, 0, 1, 2, 0};

// input, const
const int VAR_INT_W[] = {3, 1};

// int ==, float  ==
const int RET_BOOL_W[] = {6,1};
