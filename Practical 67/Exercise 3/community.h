//
// Created by Ivan Morandi on 20/12/2017.
//

#ifndef NSD_CLUSTER_H
#define NSD_CLUSTER_H

using namespace std;

struct Community {
    long *array;
    long length;
    long number_nodes;
    double metric;
};
typedef struct Community Community;

Community* community_init();
Community* community_init(long length);
void community_deinit(Community* community);
void community_resize(Community* community);
bool community_insert(Community* community, long node);
Community* community_merge(Community* community, Community*  slave);
bool community_contains(Community* community, long node);
long community_first_greater_than(Community* community, long node);
void community_print(Community* community);



#endif //NSD_CLUSTER_H
