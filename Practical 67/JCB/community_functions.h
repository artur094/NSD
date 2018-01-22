//
// Created by Ivan Morandi on 20/12/2017.
//

#ifndef NSD_COMMUNITY_F_H
#define NSD_COMMUNITY_F_H

#include <vector>
#include "adjacencylist.h"
#include "community.h"

Community* community_init(Graph* graph, long id);
void community_set_node(Graph* graph, Community* community);
void community_deinit(Community* community);
bool community_insert(Community* community, long node);
void community_merge(Graph* graph, Community* dst, Community*  src);
bool community_contains(Community* community, long node);
bool community_get_neighbour_index(Community* community, long node);
long community_first_greater_than(Community* community, long node);
void community_print(Community* community);

void community_vector_intersect_neighbours(Graph* graph, Community* dst, Community* src);
void community_vector_intersect_triangles(Graph* graph, Community* dst, Community* src);

void community_fix_duplicates_and_order(Community* community);
void community_sort_neighbour(Community* community, long neighbour_index);
void community_fix_when_neighbor_change(Graph* graph, Community* community, Community* new_community, Community* old_community);
long community_get_intersections(Community* dst, Community* src);

double community_get_merge_value(Community* dst, Community* src, long intersection, long triangles);
bool community_quality_function(Community* dst, Community* src, long intersection, long triangles);

long community_get_numbers_triangle_to_comm(Graph* graph, Community* dst, Community* src);

void community_swap_vector(vector<long> &array, long index1, long index2);

#endif //NSD_COMMUNITY_F_H
