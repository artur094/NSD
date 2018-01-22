//
// Created by Ivan Morandi on 22/12/2017.
//

#ifndef NSD_JACKCOMESBACK_H
#define NSD_JACKCOMESBACK_H

#include <vector>
#include <utility>
#include <cstdlib>
#include "adjacencylist.h"
#include "community.h"
#include "heap.h"

void jackcomesback(Graph* graph, string filename, long iterations, double alpha);
bool jackcomesback_iteration(Graph* graph, Heap* heap, long node);
long jackcomesback_best_neighbour(Graph* graph, long node);
void jackcomesback_fix_pr(Graph* graph, long node);
void jackcomesback_communities_to_file(string name, Graph* graph);


#endif //NSD_JACKCOMESBACK_H
