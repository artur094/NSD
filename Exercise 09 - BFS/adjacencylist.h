#ifndef NSD_ADJACENCYLIST_H
#define NSD_ADJACENCYLIST_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

struct Graph{
    long *neighbours;
    long *graph;
    long *graph_degree;
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

int graph_compute_degree_array(Graph*, fstream &file);
int graph_compute_size(fstream &file, long &number_nodes, long &number_edges, long &offset);
int graph_add_edge(Graph*, long node, long neighbour);
int graph_load_data(Graph*, fstream &file);
int graph_set_nodes(Graph*);

void file_reset(fstream &file);


#endif //NSD_ADJACENCYLIST_H
