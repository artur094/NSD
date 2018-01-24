//
// Created by Luca De Mori on 18/10/2017.
//

#ifndef NSD05_NODE_H
#define NSD05_NODE_H

#include <iostream>

using namespace std;

class Node{
public:
    unsigned int ID;                // the node ID
    unsigned int degree;            // the degree of the node
    unsigned int first_neigh_pos;   // where in the neighbour list we can find the neighbours

    Node();

    unsigned int getLastNeighbourPos();
    //void setC(uint16_t);
    bool operator()(const Node& lhs, const Node& rhs) const;
    friend ostream& operator<<(ostream& out, const Node& _node_ptr);

};


#endif //NSD05_NODE_H
