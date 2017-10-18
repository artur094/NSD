//
// Created by Ivan Morandi on 17/10/2017.
//
#include <iostream>
#include "adjacencylist.h"
#include "heap.h"

using namespace std;

Heap* heap_init(Graph* graph){
    Heap* heap = new Heap();
    heap->heap = new Item[graph->number_nodes];
    heap->node_indexes = new long[graph->number_nodes];
    heap->length = graph->number_nodes;
    heap->graph = graph;

    for (int i = 0; i < graph->number_nodes; ++i) {
        heap->heap[i].node = i;
        heap->heap[i].degree = graph->graph_degree[i];
        heap->node_indexes[i] = i;
    }

    return heap;
}

void heap_min_heap_restore(Heap* heap, long i){
    long min = i;
    bool cont = true;

    while (cont){
        cont = false;

        if(left(i) < heap->length && heap->heap[left(i)].degree < heap->heap[min].degree)
            min = left(i);

        if(right(i) < heap->length && heap->heap[right(i)].degree < heap->heap[min].degree)
            min = right(i);

        if(i != min){
            swap_long(heap->node_indexes[i], heap->node_indexes[min]);
            swap_item(heap->heap[i], heap->heap[min]);
            i = min;
            cont = true;
        }
    }
}

void heap_build(Heap* heap){
    for (int i = heap->length/2; i >= 0; --i) {
        heap_min_heap_restore(heap, i);
    }
}

void heap_print(Heap* heap){
    for (int i = 0; i < heap->length; ++i) {
        cout << heap->heap[i].node << " degree " << heap->heap[i].degree << endl;
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