#ifndef NSD_DIRECTED_ADJACENCYLIST_H
#define NSD_DIRECTED_ADJACENCYLIST_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <utility>

using namespace std;

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

struct DGraph{
    long *neighbours_in;
    long *neighbours_in_weight;
    long *neighbours_out;
    long *neighbours_out_weight;
    long *number_neighbours;
    long *graph_in;
    long *graph_out;
    long *graph_degree_in;
    long *graph_degree_out;
    long *weight_out_sum;
    long *weight_in_sum;
    long *node_weight;
    long number_nodes;
    long number_edges;
    long offset;

    long *last_neighbor_in;
    long *last_neighbor_out;
};
typedef struct DGraph DGraph;

struct DEdge{
    long nodeA;
    long nodeB;
    long weight;
};
typedef struct DEdge DEdge;

typedef vector<DEdge*> DEdgelist;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

DEdgelist* edgelist_init();
void edgelist_deinit(DEdgelist* edgelist);
DEdge* dedge_init(long nodeA, long nodeB, long weight);
void dedge_deinit(DEdge* dEdge);


DGraph* dgraph_init(long number_nodes, long number_edges, long offset);
void dgraph_deinit(DGraph*);
DGraph* dgraph_load_file(char* name);
DGraph* dgraph_load_graph(DEdgelist* edgelist);
void dgraph_print(DGraph*);

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

bool dgraph_compute_degree_array_from_file(DGraph*, fstream &file);
bool dgraph_compute_degree_array(DGraph*, DEdgelist* edgelist);
bool dgraph_compute_size_from_file(fstream &file, long &number_nodes, long &number_edges, long &offset);
bool dgraph_compute_size(DEdgelist* edgelist, long &number_nodes, long &number_edges, long &offset);
bool dgraph_add_edge_in(DGraph*, long node, long neighbour);
bool dgraph_add_edge_out(DGraph*, long node, long neighbour);
bool dgraph_load_data_from_file(DGraph*, fstream &file);
bool dgraph_load_data(DGraph* graph, DEdgelist* edgelist);
bool dgraph_set_nodes(DGraph* graph);

void dgraph_set_node_weight(DGraph* dgraph, long graph_node, long weight);

bool fast_graph_add_edge_out(DGraph* graph, long node, long neighbour, long weight);
bool fast_graph_add_edge_in(DGraph* graph, long node, long neighbour);

void dgraph_file_reset(fstream &file);
void dgraph_swap(long &a, long &b);

#endif //NSD_DIRECTED_ADJACENCYLIST_H
