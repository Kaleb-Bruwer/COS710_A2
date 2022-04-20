#include "GPUNode.h"

using namespace std;

ostream& operator<<(ostream& out, const GPUNode &n){
    out << "(" << n.type << ", " << n.val << ")";
    return out;
}
