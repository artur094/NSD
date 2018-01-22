//
// Created by Ivan Morandi on 20/12/2017.
//

#ifndef NSD_CLUSTER_H
#define NSD_CLUSTER_H

#include <vector>

using namespace std;

struct Community {
    long ID;
    vector<long> nodes;
    vector<long> neighbours;
    vector<long> neighbours_weight;
    vector<long> neighbours_triangle;
    double pr_value; //pagerank
    long inEdges;
    long outEdges;
};
typedef struct Community Community;

#include "adjacencylist.h"

Community* community_init(Graph* graph, long id);
void community_set_node(Graph* graph, Community* community);
void community_deinit(Community* community);
bool community_insert(Community* community, long node);
void community_merge(Community* community, Community*  slave);
bool community_contains(Community* community, long node);
long community_first_greater_than(Community* community, long node);
void community_print(Community* community);

void community_vector_intersect_neighbours(Graph* graph, Community* dst, Community* src);
void community_vector_intersect_triangles(Graph* graph, Community* dst, Community* src);


#endif //NSD_CLUSTER_H
