//
// Created by Ivan Morandi on 17/10/2017.
//

//TODO: in the report, write the avg degree density (#edges / #nodes) and the edge density (2*#edges)/(#nodes * (#nodes-1))

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist.h"
#include "heap.h"

using namespace std;

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    cout << "Loading graph..."<<endl;

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    cout << "Initializing heap..."<<endl;
    Heap* heap = heap_init(graph);

    cout << "Building heap..."<<endl;
    heap_build(heap);

    cout << "Printing heap..."<<endl;
    heap_print(heap);

    long x = 5;
    for (int i = graph->graph[x]; i < graph->graph[x] + graph->graph_degree[x]; ++i) {
        long y = heap->node_indexes[graph->neighbours[i]];
        heap->heap[y].degree--;
    }

    cout << "Building heap..."<<endl;
    heap_build(heap);

    cout << "Printing heap..."<<endl;
    heap_print(heap);

    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}
