//
// Created by Ivan Morandi on 17/10/2017.
//
#include <iostream>
#include "adjacencylist.h"
#include "heap.h"

using namespace std;

//TODO: fix heap!!!

Heap* heap_init(Graph* graph){
    Heap* heap = new Heap();
    heap->heap = new Item[graph->number_nodes];
    heap->node_indexes = new long[graph->number_nodes];
    heap->length = graph->number_nodes;
    heap->graph = graph;

    for (int i = 0; i < graph->number_nodes; ++i) {
        heap->heap[i].node = i;
        heap->heap[i].degree = graph->nodes[i].degree;
        heap->heap[i].weight = graph_get_sum_weight(graph, i);
        heap->heap[i].max_weight = graph_get_max_weight(graph, i);
        heap->node_indexes[i] = i;
    }

    return heap;
}


// a<b => min heap
// a>b => max heap
bool heap_condition(Item a, Item b){
    if(a.max_weight > b.max_weight){ //max weight between all outgoing weights
        return true;
    }
    if(a.max_weight == b.max_weight){
        return a.weight > b.weight; //sum of all links' weight
    }
    return false;
}

void heap_deinit(Heap* heap){
    if(heap == NULL)
        return;

    delete[] heap->heap;
    delete[] heap->node_indexes;
    delete heap;
}

long heap_first_element(Heap* heap){
    if(heap == NULL)
        return -1;
    if(heap->length <= 0)
        return -1;

    return heap->heap[0].node;
}

void heap_restore(Heap* heap){
    heap_build(heap);
}

void heap_remove_node(Heap* heap,Graph* graph, long node){
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

    //cout << "After removing node: " << endl;
    //heap_print(heap);
    //cout << endl << "After restore: " << endl;

    heap_bubble_down(heap, 0);

    //fix neighbours
    for (int i = 0; i < graph->nodes[node].degree; ++i) {
        long neighbour = graph->nodes[node].neighbours[i];
        long index_neighbour = heap->node_indexes[neighbour];

        if(index_neighbour < heap->length){
//            cout << "Before reducing degree of neighbour: "<< neighbour <<endl;
//            heap_print(heap);
            Item tmp;
            tmp.degree = heap->heap[index_neighbour].degree;
            tmp.weight = heap->heap[index_neighbour].weight;
            tmp.max_weight = heap->heap[index_neighbour].max_weight;

            heap->heap[index_neighbour].degree = graph->nodes[neighbour].degree;
            heap->heap[index_neighbour].weight = graph_get_sum_weight(graph, neighbour);
            heap->heap[index_neighbour].max_weight = graph_get_max_weight(graph, neighbour);
            //heap->heap[index_neighbour].degree = graph

//            cout << "After reducing degree of neighbour: "<< neighbour <<endl;
//            heap_print(heap);
//            cout << endl << "After restoring bottom-up the heap:"<<endl;
            if(heap_condition(tmp, heap->heap[index_neighbour])) {
                heap_bubble_down(heap, index_neighbour);
            }
            else {
                heap_bubble_up(heap, index_neighbour);
            }
            //heap_print(heap);
        }
    }

    //heap_restore(heap);
}

bool heap_is_empty(Heap* heap){
    return heap->length == 0;
}

//Fix heap from bottom to top
void heap_bubble_up(Heap* heap, long i){
    long min = i;
    long p;
    bool cont = true;

    while (cont){
        if(i==0)
            return;

        cont = false;
        p = father(i);

        //if(heap->heap[i].degree < heap->heap[p].degree && i > 0){
        if(heap_condition(heap->heap[i], heap->heap[p]) && i > 0){
            heap->node_indexes[heap->heap[i].node] = p;
            heap->node_indexes[heap->heap[p].node] = i;
            //swap_long(heap->node_indexes[i], heap->node_indexes[min]);
            swap_item(heap->heap[i], heap->heap[p]);

            i = p;
            cont = true;
        }
    }
}

void heap_bubble_down(Heap* heap, long i){
    long min = i;
    bool cont = true;

    while (cont){
        cont = false;


        //if(left(i) < heap->length && heap->heap[left(i)].degree < heap->heap[min].degree)
        if(left(i) < heap->length && heap_condition(heap->heap[left(i)], heap->heap[min]))
            min = left(i);

        //if(right(i) < heap->length && heap->heap[right(i)].degree < heap->heap[min].degree)
        if(right(i) < heap->length && heap_condition(heap->heap[right(i)], heap->heap[min]))
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
        heap_bubble_down(heap, i);
    }
}

void heap_print(Heap* heap){
    for (int i = 0; i < heap->length; ++i) {
        cout << heap->heap[i].node<< " degree=" << heap->heap[i].degree << ", weight=" << heap->heap[i].weight << ", max weight=" << heap->heap[i].max_weight
             << " in pos " << heap->node_indexes[heap->heap[i].node] << " --> ID in that pos = " << heap->heap[heap->node_indexes[heap->heap[i].node]].node << endl;
    }
}

long father(long i){
    return (i-1)/2;
}

long left(long i){
    return 2*i + 1;
    //return 2*i;
}

long right(long i){
    return 2*i+2;
    //return 2*i+1;
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