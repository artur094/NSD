//
// Created by Ivan Morandi on 13/10/2017.
//

#ifndef NSD_ADJACENCYLIST_H
#define NSD_ADJACENCYLIST_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <utility>
#include "queue.h"

using namespace std;

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

struct Node{
    long id;
    long degree;
    long number_triangles;
    vector<long> neighbours;
};
typedef struct Node Node;

struct Graph{
    long offset;
    long number_edges;
    long number_nodes;
    Node* nodes;
};
typedef struct Graph Graph;

/* - - - - - - - - - - - - - - - - -  FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

Graph* graph_init(long number_nodes, long offset);
void graph_deinit(Graph* graph);
void graph_print(Graph* graph);
bool graph_add_edge(Graph* graph, long node, long neighbour);
void graph_print(Graph* graph);
void graph_store_degree_file(char* filename, Graph* graph);

long graph_compute_triangles(Graph* graph);

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

bool graph_is_self_loop(int node, int neighbour);
bool graph_contains_edge(Graph* graph, long src, long dst);
long graph_get_edge(Graph* graph, long src, long dst);
bool graph_remove_edge(Graph* graph, long src, long dst);
long graph_count_intersections(vector<long> array1, long length1, long start1, vector<long> array2, long length2, long start2);
void graph_print_array(Graph* graph, long node);

void swap_vector(vector<long>* array, long index1, long index2);

long graph_count_triangles_node(Graph* graph, long node);
long graph_count_intersections(vector<long> array1, long length1, long start1, vector<long> array2, long length2, long start2);
long graph_index_least_great_element(vector<long> array, long array_length, long value);


long diameter(Graph* graph);
void BFS_cc_diameter(Graph* graph, long root, long* ids, long id, long &ids_length, long &max_distance, long &last_node);



#endif //NSD_ADJACENCYLIST_H
