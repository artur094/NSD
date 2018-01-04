//
// Created by Ivan Morandi on 20/12/2017.
//

#ifndef NSD_CLUSTER_H
#define NSD_CLUSTER_H

#include <vector>

using namespace std;

struct Community {
    vector<long> array;
    long number_nodes;
    double metric;
};
typedef struct Community Community;

Community* community_init();
void community_deinit(Community* community);
bool community_insert(Community* community, long node);
void community_merge(Community* community, Community*  slave);
bool community_contains(Community* community, long node);
long community_first_greater_than(Community* community, long node);
void community_print(Community* community);



#endif //NSD_CLUSTER_H
