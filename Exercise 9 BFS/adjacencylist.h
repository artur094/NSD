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
};
typedef struct Graph Graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

void graph_init(long number_nodes, long number_edges);
void graph_deinit();
int graph_load_file(char* name);
void graph_print();

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

int graph_compute_degree_array(fstream &file);
int graph_compute_size(fstream &file);
int graph_add_edge(long node, long neighbour);
int graph_load_data(fstream &file);
int graph_set_nodes();

void file_reset(fstream &file);


#endif //NSD_ADJACENCYLIST_H
