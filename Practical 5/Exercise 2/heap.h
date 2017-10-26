//
// Created by Ivan Morandi on 17/10/2017.
//

#ifndef NSD_HEAP_H
#define NSD_HEAP_H

struct Item{
    long node;
    long degree;
};
typedef struct Item Item;

struct Heap{
    Graph* graph;
    Item* heap;
    long* node_indexes;
    long length;
};
typedef struct Heap Heap;

Heap* heap_init(Graph* graph);
void heap_deinit(Heap* heap);
void heap_restore(Heap* heap);
void heap_print(Heap* heap);
void heap_remove_node(Heap* heap, long node);
bool heap_is_empty(Heap* heap);

void heap_min_heap_restore(Heap* heap, long i);
void heap_build(Heap* heap);
long left(long i);
long right(long i);
void swap_item(Item &a, Item &b);
void swap_long(long &a, long &b);

#endif //NSD_HEAP_H
