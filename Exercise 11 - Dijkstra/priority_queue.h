//
// Created by Ivan Morandi on 26/09/2017.
//

#ifndef NSD_QUEUE_H
#define NSD_QUEUE_H

using namespace std;

struct Node{
    long val;
    int priority;
    Node* next;
};
typedef struct Node Node;

struct PriorityQueue{
    Node* first;
    Node* last;
    long length;
};
typedef struct PriorityQueue PriorityQueue;

Node* node_init(long val, int priority);
void node_deinit(Node* node);
PriorityQueue* priority_queue_init();
void priority_queue_deinit(PriorityQueue* queue);
long priority_queue_pop(PriorityQueue* queue);
void priority_queue_push(PriorityQueue* queue, long node, int priority);
long priority_queue_first(PriorityQueue* queue);
long priority_queue_last(PriorityQueue* queue);
void priority_queue_print(PriorityQueue* queue);
bool priority_queue_is_empty(PriorityQueue* queue);
void priority_queue_update(PriorityQueue* queue, long nodeID, int new_priority);

#endif //NSD_QUEUE_H
