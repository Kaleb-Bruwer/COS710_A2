#pragma once

/*
A tree is a sequence of Nodes, where each takes only one byte and can be
eather an internal or terminal node
*/

// union allows direct access to either bit-fields or the full value
struct Node{
    union{
       struct{
           unsigned char type :2; //00: function, 01: const, 10: int input, 11: float input
           unsigned char val  :6;
       };
       unsigned char full;
    };
};

struct Node getRandFunc();
struct Node getRandTerminal();

void generateFullTree(unsigned short maxDepth);
