#pragma once

const int POPULATION = 5000;
const int INIT_DEPTH = 5;
const int GENERATIONS = 3000;

const int TRAIN_COUNT = 700;

//size must be >= winners*(multiplication+1)
const int TOURNAMENT_SIZE = 4;
const int TOURNAMENT_WINNERS = 1;
const int TOURNAMENT_MULTIPLICATION = 1;


const int CONST_CHANCE = 200; //fraction out of 1000
const int GROW_RATE = 750;

// [Crossover, Mutation, isEmpty]
const int OP_WEIGHTS[] = {9,1,0};

// Function weighths
// [+, -, *, /, toFloat, IF]
const int FLOAT_WEIGHTS[] = {1,1,0,1,3,0};
// [+, -, *, /, mod, round, IF]
const int INT_WEIGHTS[] = {1,1,0,1,1,2,0};
