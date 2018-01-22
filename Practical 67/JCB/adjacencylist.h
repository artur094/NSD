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
#include "community.h"

#define ALPHA 0.1

using namespace std;

struct Node{
    long id;
    long degree;
    long number_triangles;
    Community* community;
    vector<long> neighbours;
    vector<long> weight;
    vector<long> triangles_nodes;
};

struct Graph{
    long offset;
    long number_edges;
    long number_nodes;
    long number_communities;
    Node* nodes;
};

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

typedef struct Node Node;

typedef struct Graph Graph;

typedef vector<pair<long, long> > Edgelist;

/* - - - - - - - - - - - - - - - - -  FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

Graph* graph_init(long number_nodes, long offset);
void graph_deinit(Graph* graph);
Graph* graph_load_file(char* name);
Graph* graph_load_edgelist(Edgelist* edgelist);
Graph* graph_load_edgelist(Edgelist* edgelist, long number_nodes, long offset);
bool graph_add_edge(Graph* graph, long node, long neighbour, long weight);
void graph_print(Graph* graph);
bool graph_add_edge(Graph* graph, long node, long neighbour);
Graph* graph_load_file(char* name);
void graph_store_degree_file(char* filename, Graph* graph);
void graph_store_file(char* filename, Graph* graph);

long graph_compute_triangles(Graph* graph);

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

void reset_file(fstream &file);
bool graph_is_self_loop(int node, int neighbour);
Edgelist* graph_file_to_edgelist(fstream &file);
bool graph_compute_size(Edgelist* edgelist, long &number_nodes, long &number_edges, long &offset);
bool graph_compute_degree(Graph* graph, Edgelist* edgelist);
bool graph_contains_edge(Graph* graph, long src, long dst);
long graph_get_edge(Graph* graph, long src, long dst);
bool graph_remove_edge(Graph* graph, long src, long dst);
long graph_count_intersections(vector<long> array1, long length1, long start1, vector<long> array2, long length2, long start2);
void graph_print_array(Graph* graph, long node);
void graph_communities_to_file(Graph* graph, char* filename);
long* graph_get_communities(Graph* graph);
bool graph_substitute_neighbour(Graph* graph, long node, long old_node, long new_node);
void graph_sort_new_neighbor(Graph* graph, long node, long neighbor_index);

void swap_vector(vector<long>* array, long index1, long index2);

void graph_merge(Graph* graph, long nodeA, long nodeB);
bool graph_load_data(Graph* graph, Edgelist* edgelist);

long graph_count_triangles_node(Graph* graph, long node);
long graph_count_intersections(vector<long> array1, long length1, long start1, vector<long> array2, long length2, long start2);
long graph_index_least_great_element(vector<long> array, long array_length, long value);

void page_rank(Graph* graph, double alpha, int iterations);

void graph_set_triangles_node(Graph* graph, long node);

void graph_print_communities(Graph* graph);
void graph_fix_communities(Graph* graph);


#endif //NSD_ADJACENCYLIST_H
