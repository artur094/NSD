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
    long pointer;
};
typedef struct Heap Heap;

Heap* heap_init(Graph* graph);
void heap_deinit(Heap* heap);
void heap_restore(Heap* heap);
void heap_print(Heap* heap);
void heap_remove_node(Heap* heap,Graph* graph, long node);
bool heap_is_empty(Heap* heap);
long heap_first_element(Heap* heap);

void heap_bubble_down(Heap* heap, long i);
void heap_bubble_up(Heap* heap, long i);

long heap_pointer_next(Heap* heap);
void heap_fix(Heap* heap, Graph* graph);

void heap_build(Heap* heap);
long left(long i);
long right(long i);
long father(long i);
void swap_item(Item &a, Item &b);
void swap_long(long &a, long &b);

#endif //NSD_HEAP_H
