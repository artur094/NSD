//
// Created by Ivan Morandi on 20/12/2017.
//

#include <iostream>
#include "community.h"

Community* community_init(){
    return community_init(10);
}

Community* community_init(long length){
    Community* community = new Community();
    community->array = new long[length];
    community->length = length;
    community->number_nodes = 0;
    community->metric = 0.0;
    return community;
}

void community_deinit(Community* community){
    delete[] community->array;
    community->array = NULL;
    delete community;
}

//Called when the community doesn't have anymore space
//Double the space
void community_resize(Community* community){
    if(community == NULL)
        return;

    //popolate the new array
    long new_size = community->length*2;
    long *new_array = new long[new_size];
    for (int i = 0; i < community->number_nodes; ++i) {
        new_array[i] = community->array[i];
    }

    //free the old array
    delete[] community->array;

    //substitute the old array with the new one
    community->array = new_array;
    community->length = new_size;
}

bool community_insert(Community* community, long node){
    //Pre checks
    if(community == NULL)
        return false;

    if(community->array == NULL)
        return false;

    //If already inside the community
    if(community_contains(community, node))
        return false;

    //if there is not enough space
    if(community->number_nodes == community->length)
        community_resize(community);

    //search the first node with greatest (or equal) id than the given one
    long tmp = node;
    long pos = community_first_greater_than(community, node);

    if(pos < 0)
        pos = community->number_nodes;

    //I'm going to add a new value, so I need more space
    community->number_nodes++;

    for(; pos<community->number_nodes; pos++){
        tmp = node;
        node = community->array[pos];
        community->array[pos] = tmp;
    }

    return true;
}

Community* community_merge(Community* community, Community*  community1){
    cout << "[COMMERGE]: Prechecks #1" << endl;
    if(community == NULL || community1 == NULL)
        return NULL;

    cout << "[COMMERGE]: Prechecks #2" << endl;
    if(community->array == NULL || community1->array == NULL)
        return NULL;

    cout << "[COMMERGE]: Initializing final community" << endl;
    Community* final = community_init(community->length + community1->length);

    int i=0; //community
    int j=0; //community1
    int k=0; //new community

    cout << "[COMMERGE]: Adding in ordered way all nodes" << endl;
    while(i < community->number_nodes && j < community1->number_nodes){
        if(community->array[i] == community1->array[j]){
            final->array[k] = community->array[i];
            i++;
            j++;
        }
        else if(community->array[i] < community1->array[j]){
            final->array[k] = community->array[i];
            i++;
        }
        else{
            final->array[k] = community1->array[j];
            j++;
        }
        k++;
    }

    cout << "[COMMERGE]: Adding remaining nodes" << endl;
    for(; i < community->number_nodes; i++, k++){
        final->array[k] = community->array[i];
    }

    for(; j < community1->number_nodes; j++, k++){
        final->array[k] = community1->array[j];
    }

    final->number_nodes = k;

    cout << "[COMMERGE]: Return" << endl;
    return final;
}

bool community_contains(Community* community, long node){
    //Pre checks
    if(community == NULL)
        return false;
    if(community->array == NULL)
        return false;

    //Binary search
    int start = 0;
    int end = community->number_nodes-1;
    int m;

    if(community->array[start] == node)
        return true;
    if(community->array[end] == node)
        return false;

    while(start<end){
        m = (start+end)/2;

        if(community->array[m] == node)
            return true;
        else if(community->array[m] < node)
            start = m+1;
        else
            end = m-1;
    }

    return false;
}

long community_first_greater_than(Community* community, long node){
    //search the first node with greatest (or equal) id than the given one
    int start = 0;
    int end = community->number_nodes-1;
    int m;
    int pos;

    if(node < community->array[start])
        return 0; //I have to more the whole array

    if(node > community->array[end])
        return -1;  //No nodes greater than the input

    while(start < end){
        //if m-th node is bigger
        if(community->array[m] > node){
            //check if m-1th node is smaller -> if yes return m as the first node greater than the input
            if(community->array[m-1] < node)
                return m;
            else
                end = m-1;
        }
        else
            start = m+1;
    }
    //if here??? it shouldn't reach this position...
    //TODO: test function
    return -1;
}

void community_print(Community* community){
    for (int i = 0; i < community->number_nodes; ++i) {
        cout << community->array[i] << " ";
    }
    cout << endl;
}




