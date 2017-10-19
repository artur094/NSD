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

long* core_decomposition(Graph* graph);
long max(long a, long b);

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    cout << "Loading graph..."<<endl;

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    cout << "Computing core decomposition..."<<endl;
    core_decomposition(graph);

    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

//TODO: non cancella bene i degree credo
long* core_decomposition(Graph* graph){
    Heap* heap = heap_init(graph);
    heap_restore(heap);

    long* cd = new long[graph->number_nodes];

    long n = graph->number_nodes-1;
    long c = 0;

    while(! heap_is_empty(heap)){
        long v = heap->heap[0].node;
        //cout << "Removing " << v + heap->graph->offset << " ..." <<endl;
        c = max(c, heap->heap[0].degree);

        //heap_print(heap);
        //cout << "After " << endl;

        heap_remove_node(heap, v);
        cd[v] = n;

        //heap_print(heap);

        cout << v+graph->offset << " with c= " << c << endl;
        n--;
    }

    heap_deinit(heap);

    return cd;
}

long max(long a, long b){
    return a>b ? a : b;
}