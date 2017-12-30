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

#define ALPHA 0.1
#define ITERATIONS 1000

void jackcomesback(Graph* graph, long number_clusters);
void jcb_iteration(Graph* graph, Heap* heap, long node);
vector<pair<long, long> >* jcb_build_triangle_graph(Graph* graph, long node);
vector<long>* jcb_intersection(Graph* graph, long node, long neighbor);
long index_least_great_element(long* array, long array_length, long start, long value);
long jcb_node_max_page_rank(DGraph* dgraph, long target);
void jcb_print_vgraph(vector<pair<long, long> >* vgraph);
void jcb_print_vector(vector<long>* v);
void jcb_print_array(long* array, long start, long end);

double* page_rank(DGraph* dgraph, long target, double alpha, int iterations);


#endif //NSD_JACKCOMESBACK_H
