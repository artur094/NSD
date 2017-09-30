#ifndef NSD_ADJACENCYLIST_H
#define NSD_ADJACENCYLIST_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

struct Graph{
    long *neighbours;
    long *graph;
    long *graph_degree;
    long *graph_triangles;
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
