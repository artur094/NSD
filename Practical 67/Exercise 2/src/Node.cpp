//
// Created by Luca De Mori on 18/10/2017.
//

#include "Node.h"

Node::Node(){
    ID =0;
    degree=0;
    first_neigh_pos=0;
}

unsigned int Node::getLastNeighbourPos(){
    return first_neigh_pos+degree;
}

bool Node::operator()(const Node& lhs, const Node& rhs) const {
    return lhs.degree < rhs.degree ;
}

ostream& operator<<(ostream& out, const Node& obj){
    out << "ID:" << obj.ID << "  d:" << obj.degree;
}

