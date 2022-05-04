#include "Node.h"

using namespace std;

enum NodeReturnType Node::getParamType(int param){
    if(param == 0){
        if(type != 0 || val == 0) //0 parameter nodes
            return NONE;
        if(val & 32){
            if(val == 37 || val == 38)
                return INT;
            return FLOAT;
        }
        else{
            if(val == 6)
                return FLOAT;
            return INT;
        }
    }

    else if(param == 1){
        if(type != 0 || val == 0) //0 parameter Nodes
            return NONE;
        if(val == 6 || val == 37) //1 parameter functions
            return NONE;

        return (val & 32) ? FLOAT : INT;
    }

    else if(param == 2){
        if(val == 38) return FLOAT;
        if(val == 7) return INT;
        return NONE;
    }
    
    return NONE;
}


// getParams will obviously need a rework when more functions are added
enum NodeReturnType Node::getParam1(){
    // assumes this is a valid Node
    if(type != 0 || val == 0) //0 parameter nodes
        return NONE;
    if(val & 32){
        if(val == 37)
            return INT;
        return FLOAT;
    }
    else{
        if(val == 6)
            return FLOAT;
        return INT;
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

ostream& operator<<(ostream& out, const Node &n){
    out << "(" << (int)n.type << ", " << (int)n.val << ")";
    return out;
}

bool operator==(const Node &lhs, const Node &rhs){
    return (lhs.type == rhs.type) && (lhs.val == rhs.val);
}

bool operator!=(const Node &lhs, const Node &rhs){
    return !(lhs == rhs);
}
