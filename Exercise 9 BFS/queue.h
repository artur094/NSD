//
// Created by Ivan Morandi on 26/09/2017.
//

#ifndef NSD_QUEUE_H
#define NSD_QUEUE_H

using namespace std;

struct Node{
    long val;
    Node* next;
};
typedef struct Node Node;

struct Queue{
    Node* first;
    Node* last;
    long length;
};
typedef struct Queue Queue;

Node* node_init(long val);
void node_deinit(Node* node);
Queue* queue_init();
void queue_deinit(Queue* queue);
Node* queue_pop(Queue* queue);
void queue_push(Queue* queue, Node node);
long queue_first(Queue* queue);
long queue_last(Queue* queue);
void queue_print(Queue* queue);

#endif //NSD_QUEUE_H
