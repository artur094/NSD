//
// Created by Ivan Morandi on 20/12/2017.
//

#include <iostream>
#include "community.h"

using namespace std;

Community* community_init(long node){
    //cout << "Initializing community"<< endl;
    Community* comm = new Community();

    //cout << "Setting variables" << endl;
    comm->degreeIn = 0.0;
    comm->degreeOut = 0;
    comm->node = node;
    comm->parent = NULL;
    comm->number_nodes = 1;
    comm->nodes = to_string(node);
    //cout << "Returning community" << endl;
    return comm;
}

Community* community_find_boss(Community* community){
    if(community == NULL)
        return NULL;

    Community* tmp = community;

    while (tmp->parent != NULL)
        tmp = tmp->parent;

    return tmp;
}

void community_deinit(Community* community){
    delete community;
}

void community_merge(Community* dst, Community*  source, long weight){
    if(dst == NULL || source == NULL)
        return;

    source->parent = dst;
    dst->degreeOut += source->degreeOut - 2; //remove the old degree out
    dst->degreeIn += source->degreeIn + weight;
    dst->number_nodes += source->number_nodes;
    dst->nodes += " " + source->nodes;
}

bool community_quality_function(Community* dst, Community* src, long weight){
    double actual_val = (double)dst->degreeIn / (double)(dst->degreeIn + dst->degreeOut);
    long new_degreeIn = dst->degreeIn + weight + src->degreeIn;
    long new_degreeOut = dst->degreeOut + src->degreeOut - 2;
    long new_number_nodes = dst->number_nodes + src->number_nodes;
    double new_val = (double)new_degreeIn / (double)(new_degreeIn + new_degreeOut);

    return new_val > actual_val;
}

bool community_quality_function_edit_distance(Community* dst, Community* src, long weight){
    double actual_val = (double)(dst->number_nodes * (dst->number_nodes-1)) / 2.0 - (double)dst->degreeIn + (double)dst->degreeOut;
    long new_degreeIn = dst->degreeIn + weight + src->degreeIn;
    long new_degreeOut = dst->degreeOut + src->degreeOut - 2;
    long new_number_nodes = dst->number_nodes + src->number_nodes;
    double new_val =  (double)(new_number_nodes * (new_number_nodes-1)) / 2.0 - (double)new_degreeIn + (double)new_degreeOut;

    return new_val < actual_val;
}