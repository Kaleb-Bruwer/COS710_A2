#include "Node.h"

// getParams will obviously need a rework when more functions are added
enum NodeReturnType Node::getParam1(){
    // assumes this is a valid Node
    if(type != 0 || val == 0) //0 parameter nodes
        return NONE;
    if(val & 32){
        if(val == 6)
            return FLOAT;
        return INT;
    }
    else{
        if(val == 37)
            return INT;
        return FLOAT;
    }
}

enum NodeReturnType Node::getParam2(){
    // assumes this is a valid Node
    if(type != 0 || val == 0) //0 parameter Nodes
        return NONE;
    if(val == 6 || val == 37) //1 parameter functions
        return NONE;

    return (val & 32) ? FLOAT : INT;
}
