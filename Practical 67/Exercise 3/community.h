//
// Created by Ivan Morandi on 20/12/2017.
//

#ifndef NSD_CLUSTER_H
#define NSD_CLUSTER_H

#include <vector>

using namespace std;

struct Community {
    Community* parent;
    long node;
    long degreeIn;
    long degreeOut;
    long number_nodes;
    string nodes;
};
typedef struct Community Community;

Community* community_find_boss(Community* community);

Community* community_init(long node);
void community_deinit(Community* community);
bool community_insert(Community* community, long node);
void community_merge(Community* community, Community*  slave, long weight);
bool community_contains(Community* community, long node);
long community_first_greater_than(Community* community, long node);
void community_print(Community* community);
bool community_quality_function(Community* dst, Community* src, long weight);
bool community_quality_function_edit_distance(Community* dst, Community* src, long weight);


#endif //NSD_CLUSTER_H
