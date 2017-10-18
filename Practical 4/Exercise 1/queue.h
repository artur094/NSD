//
// Created by Ivan Morandi on 26/09/2017.
//

#ifndef NSD_QUEUE_H
#define NSD_QUEUE_H

using namespace std;

struct Item{
    long val;
    Item* next;
};
typedef struct Item Item;

struct Queue{
    Item* first;
    Item* last;
    long length;
};
typedef struct Queue Queue;

Item* node_init(long val);
void node_deinit(Item* node);
Queue* queue_init();
void queue_deinit(Queue* queue);
long queue_pop(Queue* queue);
void queue_push(Queue* queue, long node);
long queue_first(Queue* queue);
long queue_last(Queue* queue);
void queue_print(Queue* queue);
bool queue_is_empty(Queue* queue);

#endif //NSD_QUEUE_H
