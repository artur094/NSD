//
// Created by Ivan Morandi on 20/12/2017.
//

#include <iostream>
#include "community.h"

Community* community_init(){
    //cout << "Initializing community"<< endl;
    Community* comm = new Community();

    //cout << "Setting variables" << endl;
    comm->number_nodes = 0;
    comm->metric = 0.0;
    //cout << "Returning community" << endl;
    return comm;
}

void community_deinit(Community* community){
    delete community;
}

bool community_insert(Community* community, long node){
    //Pre checks
    if(community == NULL)
        return false;

    //If already inside the community
    //cout << "contains" << endl;
    if(community_contains(community, node))
        return false;

    //cout << "greater" << endl;
    //search the first node with greatest (or equal) id than the given one
    long tmp = node;
    long pos = community_first_greater_than(community, node);

    if(pos < 0)
        pos = community->number_nodes;

    //I'm going to add a new value, so I need more space
    community->number_nodes++;

    //cout << "Adding in position " << pos << endl;
    community->array.insert(community->array.begin()+pos, node);

    return true;
}

void community_merge(Community* community, Community*  community1){
    //cout << "[COMMERGE]: Prechecks #1" << endl;
    if(community == NULL || community1 == NULL)
        return;

    for (int i = 0; i < community1->array.size(); ++i) {
        community->array.push_back(community1->array[i]);
        community->number_nodes++;
    }
    //sort(community->array.begin(), community->array.begin() + community->number_nodes);
}

bool community_contains(Community* community, long node){
    //Pre checks
    if(community == NULL)
        return false;

    //Binary search
    long start = 0;
    long end = community->number_nodes-1;
    long m;

    while(start<=end){
        m = (start+end)/2;

        if(community->array[m] == node)
            return true;
        else if(community->array[start] == node)
            return true;
        else if(community->array[end] == node)
            return true;
        else if(community->array[m] < node)
            start = m+1;
        else
            end = m-1;
    }

    return false;
}

long community_first_greater_than(Community* community, long node){
    if(community == NULL)
        return -1;
    if(community->array.size() <= 0)
        return -1;

    //search the first node with greatest (or equal) id than the given one
    long start = 0;
    long end = community->number_nodes-1;
    long m;
    long pos;

    if(node < community->array[start])
        return 0; //I have to more the whole array

    if(node > community->array[end])
        return -1;  //No nodes greater than the input

    while(start <= end){
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

Community* community_merge_old(Community* community, Community*  community1){
    //cout << "[COMMERGE]: Prechecks #1" << endl;
    if(community == NULL || community1 == NULL)
        return NULL;

    long length = community->array.size() + community1->array.size();
    //cout << "[COMMERGE]: Initializing final community with lenth=" << length << " = " << community->array.size() << " + " << community1->array.size() << endl;
    Community* final_comm = community_init();
    //cout << "[COMMERGE]: Resizing of " << length << endl;
    //final_comm->array.resize(length);

    int i=0; //community
    int j=0; //community1
    int k=0; //new community

    //cout << "[COMMERGE]: Adding in ordered way all nodes" << endl;
    //cout << "[COMMERGE]: Comm1 -> #nodes = " << community->number_nodes << " and vector size=" << community->array.size() << endl;
    //cout << "[COMMERGE]: Comm2 -> #nodes = " << community1->number_nodes << " and vector size=" << community1->array.size() << endl;
    while(i < community->number_nodes && j < community1->number_nodes){
        //cout << community->array[i] << " == " << community1->array[j] << endl;
        if(community->array[i] == community1->array[j]){
            //cout << "Trying to add " << community->array[i] << endl;
            final_comm->array.push_back(community->array[i]);
            //cout << "Added " << final_comm->array[final_comm->array.size()-1] << endl;
            i++;
            j++;
        }
        else if(community->array[i] < community1->array[j]){
            //cout << "Trying to add " << community->array[i] << endl;
            final_comm->array.push_back(community->array[i]);
            //cout << "Added " << final_comm->array[final_comm->array.size()-1] << endl;
            i++;
        }
        else{
            //cout << "Trying to add " << community1->array[j] << endl;
            final_comm->array.push_back(community1->array[j]);
            //cout << "Added " << final_comm->array[final_comm->array.size()-1] << endl;
            j++;
        }
        k++;
    }

    //cout << "[COMMERGE]: Adding remaining " << community->number_nodes - i << " nodes, from " << i << " to " << community->number_nodes << " (vector size = " << community->array.size() << ")" << endl;
    for(; i < community->number_nodes; i++, k++){
        final_comm->array.push_back(community->array[i]);
    }
    //cout << "[COMMERGE]: Adding remaining " << community1->number_nodes - j << " nodes, from " << j << " to " << community1->number_nodes << " (vector size = " << community1->array.size() << ")" << endl;
    for(; j < community1->number_nodes; j++, k++){
        final_comm->array.push_back(community1->array[j]);
    }

    final_comm->number_nodes = k;

    //cout << "[COMMERGE]: Return" << endl;
    return final_comm;
}


