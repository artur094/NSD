#ifndef NSD_ADJACENCYLIST_H
#define NSD_ADJACENCYLIST_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

struct Graph{
    long *neighbours_in;
    long *neighbours_out;
    long *graph_in;
    long *graph_out;
    long *graph_degree_in;
    long *graph_degree_out;
    long number_nodes;
    long number_edges;
    long offset;

    long *last_neighbor_in;
    long *last_neighbor_out;
};
typedef struct Graph Graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

Graph* graph_init(long number_nodes, long number_edges, long offset);
void graph_deinit(Graph*);
Graph* graph_load_file(char* name);
void graph_print(Graph*);

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

bool graph_compute_degree_array(Graph*, fstream &file);
bool graph_compute_size(fstream &file, long &number_nodes, long &number_edges, long &offset);
bool graph_add_edge_in(Graph*, long node, long neighbour);
bool graph_add_edge_out(Graph*, long node, long neighbour);
bool graph_load_data(Graph*, fstream &file);
bool graph_set_nodes(Graph*);

bool fast_graph_add_edge_out(Graph* graph, long node, long neighbour);
bool fast_graph_add_edge_in(Graph* graph, long node, long neighbour);

void file_reset(fstream &file);

#endif //NSD_ADJACENCYLIST_H
