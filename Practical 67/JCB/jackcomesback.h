//
// Created by Ivan Morandi on 22/12/2017.
//

#ifndef NSD_JACKCOMESBACK_H
#define NSD_JACKCOMESBACK_H

#include <vector>
#include <utility>
#include <cstdlib>
#include "adjacencylist.h"
#include "directed_adjacencylist.h"
#include "heap.h"

#define ALPHA 0
#define ITERATIONS 50

void jackcomesback(Graph* graph, long number_clusters, int pagerank_iterations, double pagerank_alpha);
void jcb_iteration(Graph* graph, Heap* heap, long node, int pagerank_iterations, double pagerank_alpha);
vector<pair<long, long> >* jcb_build_triangle_graph(Graph* graph, long node);
vector<long>* jcb_intersection(Graph* graph, long node, long neighbor);
long index_least_great_element(long* array, long array_length, long start, long value);
long jcb_node_max_page_rank(DGraph* dgraph, long target);
void jcb_print_edgelist(DEdgelist* edgelist);
void jcb_print_vector(vector<long>* v);
void jcb_print_array(long* array, long start, long end);
void jcb_fix_dgraph_node_weight(Graph* graph, DGraph* dgraph, long nodeTarget);

long page_rank(DGraph* dgraph, long target, double alpha, int iterations); //if the target node IS in the directed_graph
double* page_rank_v2(DGraph* dgraph, double alpha, int iterations); //if the target node IS NOT in the directed_graph

long jcb_node_max_page_rank(Graph* graph, DGraph* dgraph, long target, int pagerank_iterations, double pagerank_alpha);
DEdgelist* jcb_neighbours_d2(Graph* graph, long node);

#endif //NSD_JACKCOMESBACK_H
