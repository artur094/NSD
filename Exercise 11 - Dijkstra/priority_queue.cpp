#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "priority_queue.h"

Node* node_init(long val, int priority){
    Node* node = new Node();
    node->next = NULL;
    node->val = val;
    node->priority = priority;
    return node;
}

void node_deinit(Node* node){
    delete node;
}

PriorityQueue* priority_queue_init(){
    PriorityQueue* q = new PriorityQueue();
    q->first = NULL;
    q->last = NULL;
    q->length = 0;
    return q;
}

void priority_queue_deinit(PriorityQueue* queue){
    if(queue == NULL)
        return;
    if(queue->first == NULL)
        return;

    Node* node = queue->first;
    Node* tmp;

    while(node != NULL)
    {
        tmp = node;
        node = node->next;
        node_deinit(tmp);
    }

    delete queue;
}

long priority_queue_pop(PriorityQueue* queue){
    if(queue == NULL)
        return NULL;
    if(queue->first == NULL)
        return NULL;
    if(queue->length <= 0)
        return NULL;

    Node* node = queue->first;
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

void priority_queue_update(PriorityQueue* queue, long nodeID, int new_priority){
    if(priority_queue_is_empty(queue))
        return;

    Node* tmp = queue->first;
    Node* last = NULL;

    while(tmp != NULL){
        if(tmp->val == nodeID){
            //If first element
            if(last == NULL){
                queue->first = tmp->next;
            }//Last element
            if (tmp->next == NULL){
                queue->last = last;
                last->next = NULL;
            }
            if (tmp->next != NULL && last != NULL){
                last->next = tmp->next;
            }

            node_deinit(tmp);
            queue->length--;

            priority_queue_push(queue, nodeID, new_priority);

            return;
        }
        last = tmp;
        tmp = tmp->next;
    }
    return;
}

//TODO: update priority when there is a new priority (or add a function update)
void priority_queue_push(PriorityQueue* queue, long nodeID, int priority){
    if(queue == NULL)
        return;

    Node* node = node_init(nodeID, priority);

    //Queue empty
    if(queue->length == 0){
        queue->first = node;
        queue->last = node;
    }//If first element has lower priority
    else if(queue->first->priority > priority){
        node->next = queue->first;
        queue->first = node;
    }
    else{
        Node* tmp = queue->first->next;
        Node* last = queue->first;

        //I have to find the correct place
        while(tmp != NULL && tmp->priority <= priority){
            last = tmp;
            tmp = tmp->next;
        }

        last->next = node;
        node->next = tmp;

        if(tmp == NULL)
            queue->last = node;
    }
    queue->length++;
}

long priority_queue_first(PriorityQueue* queue){
    if(queue == NULL)
        return NULL;
    if(queue->first == NULL)
        return NULL;

    return queue->first->val;
}

long priority_queue_last(PriorityQueue* queue){
    if(queue == NULL)
        return NULL;
    if(queue->last == NULL)
        return NULL;

    return queue->last->val;

}

void priority_queue_print(PriorityQueue* queue){
    if(queue == NULL)
        return;
    if(queue->first == NULL)
        return;
    if(queue->last == NULL)
        return;

    Node* node = queue->first;

    while(node != NULL){
        cout << node->val + 1<< " ";
        node = node->next;
    }

    cout << endl;
}

bool priority_queue_is_empty(PriorityQueue* queue){
    if(queue == NULL)
        return true;

    if(queue->length == 0)
        return true;
    return false;
}
