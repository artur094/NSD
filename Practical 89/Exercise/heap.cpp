//
// Created by Ivan Morandi on 17/10/2017.
//
#include <iostream>
#include "adjacencylist.h"
#include "heap.h"

using namespace std;

Heap* heap_init(Graph* graph, double* score){
    Heap* heap = new Heap();
    heap->heap = new Item[graph->number_nodes];
    heap->node_indexes = new long[graph->number_nodes];
    heap->length = graph->number_nodes;
    heap->graph = graph;

    for (int i = 0; i < graph->number_nodes; ++i) {
        heap->heap[i].node = i;
        heap->heap[i].score = score[i];
        heap->node_indexes[i] = i;
    }

    return heap;
}

void heap_deinit(Heap* heap){
    if(heap == NULL)
        return;

    delete[] heap->heap;
    delete[] heap->node_indexes;
    delete heap;
}

void heap_restore(Heap* heap){
    heap_build(heap);
}

void heap_remove_node(Heap* heap, long node){
    if(heap == NULL)
        return;

    long index = heap->node_indexes[node];

    //move the node at the end
    long last_node = heap->heap[heap->length - 1].node;
    swap_item(heap->heap[index], heap->heap[heap->length-1]);
    heap->node_indexes[node] = heap->length-1;
    heap->node_indexes[last_node] = index;

    heap->node_indexes[node] = heap->length-1;
    heap->length--;

    /*
    for (int i = 0; i < heap->graph->graph_degree[node]; ++i) {
        long neighbour = heap->graph->neighbours[heap->graph->graph[node] + i];
        cout << "Neighbour of " << node + heap->graph->offset << " --> " << neighbour + heap->graph->offset
                << " in pos " << heap->node_indexes[neighbour] << " --> ID in that pos = " << heap->heap[heap->node_indexes[neighbour]].node+ heap->graph->offset << endl;
    }*/

    //reduce degree of neighbours
    for (int i = 0; i < heap->graph->graph_degree[node]; ++i) {
        long neighbour = heap->graph->neighbours[heap->graph->graph[node] + i];
        long index_neighbour = heap->node_indexes[neighbour];

        heap->heap[index_neighbour].score--;
    }

    heap_restore(heap);
}

bool heap_is_empty(Heap* heap){
    return heap->length == 0;
}

void heap_min_heap_restore(Heap* heap, long i, long length){
    long min = i;
    bool cont = true;

    while (cont){
        cont = false;

        if(left(i) < length && heap->heap[left(i)].score < heap->heap[min].score)
            min = left(i);

        if(right(i) < length && heap->heap[right(i)].score < heap->heap[min].score)
            min = right(i);

        if(i != min){
            heap->node_indexes[heap->heap[i].node] = min;
            heap->node_indexes[heap->heap[min].node] = i;
            //swap_long(heap->node_indexes[i], heap->node_indexes[min]);
            swap_item(heap->heap[i], heap->heap[min]);

            i = min;
            cont = true;
        }
    }
}

void heap_build(Heap* heap){
    for (int i = heap->length/2; i >= 0; --i) {
        heap_min_heap_restore(heap, i, heap->length);
    }
}

void heap_print(Heap* heap){
    for (int i = 0; i < heap->length; ++i) {
        cout << heap->heap[i].node + heap->graph->offset << " degree " << heap->heap[i].score
             << " in pos " << heap->node_indexes[heap->heap[i].node] << " --> ID in that pos = " << heap->heap[heap->node_indexes[heap->heap[i].node]].node+ heap->graph->offset << endl;
    }
}

void heap_sort(Heap* heap){
    //cout << "Building..." << endl;
    heap_build(heap);

    for (int i = heap->length - 1; i > 0; --i) {
        heap->node_indexes[heap->heap[i].node] = 0;
        heap->node_indexes[heap->heap[0].node] = i;
        swap_item(heap->heap[0], heap->heap[i]);

        heap_min_heap_restore(heap, 0, i);
    }

}

long left(long i){
    return 2*i;
}

long right(long i){
    return 2*i+1;
}

void swap_item(Item &a, Item &b){
    Item tmp = a;
    a = b;
    b = tmp;
}

void swap_long(long &a, long &b){
    long tmp = a;
    a = b;
    b = tmp;
}
