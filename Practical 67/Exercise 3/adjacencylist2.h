#ifndef NSD_ADJACENCYLIST_H
#define NSD_ADJACENCYLIST_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "community.h"

using namespace std;

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

struct Graph{
    Community **communities;
    long *neighbours;
    long *neighbours_weight;
    long *graph;
    long *graph_degree;
    long *graph_triangles;
    long number_communities;
    long number_nodes;
    long number_edges;
    long offset;
};
typedef struct Graph Graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

Graph* graph_init(long number_nodes, long number_edges, long offset);
void graph_deinit(Graph*);
Graph* graph_load_file(char* name);
void graph_print(Graph*);

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

bool graph_merge_communities(Graph* graph, long v, long u);
bool graph_substitute_neighbour(Graph* graph, long node, long old_node, long new_node);
bool graph_contains_edge(Graph* graph, long src, long dst);
long graph_get_edge(Graph* graph, long src, long dst);
bool graph_remove_edge(Graph* graph, long src, long dst);

void graph_communities_to_file(Graph* graph, char* filename);
long* graph_get_communities(Graph* graph);

void graph_print_array(Graph* graph, long start, long end);
void graph_sort_new_neighbor(Graph* graph, long node, long neighbor_index);


long graph_compute_triangles(Graph* graph);
bool graph_compute_degree_array(Graph*, fstream &file);
bool graph_compute_size(fstream &file, long &number_nodes, long &number_edges, long &offset);
bool graph_add_edge(Graph*, long node, long neighbour);
bool graph_load_data(Graph*, fstream &file);
bool graph_set_nodes(Graph*);
long graph_count_triangles_node(Graph* graph, long node);
long graph_count_intersections(long* array, long start1, long end1, long start2, long end2);
long graph_index_least_great_element(long* array, long array_length, long start, long value);

void file_reset(fstream &file);

#endif //NSD_ADJACENCYLIST_H
