//
// Created by Ivan Morandi on 20/12/2017.
//

#ifndef NSD_COMMUNITY_H
#define NSD_COMMUNITY_H

#include <vector>

using namespace std;

//IMPORTANT: ALL NODES ARE COMMUNITIES!!!
struct Community {
    long ID;
    vector<long> nodes;
    vector<long> neighbours; //just one node representing the community
    vector<long> neighbours_comm; //community ID of the node
    vector<long> neighbours_weight; //weight to reach the community (sum of links)
    vector<long> neighbours_triangle; //intersection between communities

    long number_neighbour;
    long number_triangles;
    double pr_value; //pagerank
    long inEdges; //edges inside the community
    long outEdges; //edges going outside the community
};
typedef struct Community Community;

#endif //NSD_COMMUNITY_H
