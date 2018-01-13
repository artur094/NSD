#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "queue.h"

QNode* node_init(long val){
    QNode* node = new QNode();
    node->next = NULL;
    node->val = val;
    return node;
}

void node_deinit(QNode* node){
    delete node;
}

Queue* queue_init(){
    Queue* q = new Queue();
    q->first = NULL;
    q->last = NULL;
    q->length = 0;
    return q;
}

void queue_deinit(Queue* queue){
    if(queue == NULL)
        return;

    QNode* node = queue->first;
    QNode* tmp;

    while(node != NULL)
    {
        tmp = node;
        node = node->next;
        node_deinit(tmp);
    }

    delete queue;
}

long queue_pop(Queue* queue){
    if(queue == NULL)
        return NULL;
    if(queue->first == NULL)
        return NULL;
    if(queue->length <= 0)
        return NULL;

    QNode* node = queue->first;
    queue->first = node->next;
    queue->length--;

    if(queue->length == 0) {
        queue->last = NULL;
        queue->first = NULL;
    }

    long val = node->val;
    node_deinit(node);
    return val;
}

void queue_push(Queue* queue, long nodeID){
    if(queue == NULL)
        return;

    QNode* node = node_init(nodeID);

    if(queue->length == 0){
        queue->first = node;
        queue->last = node;
    }else{
        QNode* last = queue->last;
        last->next = node;
        queue->last = node;
    }
    queue->length++;
}

long queue_first(Queue* queue){
    if(queue == NULL)
        return NULL;
    if(queue->first == NULL)
        return NULL;

    return queue->first->val;
}

long queue_last(Queue* queue){
    if(queue == NULL)
        return NULL;
    if(queue->last == NULL)
        return NULL;

    return queue->last->val;

}

void queue_print(Queue* queue){
    if(queue == NULL)
        return;
    if(queue->first == NULL)
        return;
    if(queue->last == NULL)
        return;

    QNode* node = queue->first;

    while(node != NULL){
        cout << node->val << " ";
        node = node->next;
    }

    cout << endl;
}

bool queue_is_empty(Queue* queue){
    if(queue == NULL)
        return true;

    if(queue->length == 0)
        return true;
    return false;
}
