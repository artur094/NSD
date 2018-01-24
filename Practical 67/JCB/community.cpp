//
// Created by Ivan Morandi on 20/12/2017.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "community.h"
#include "adjacencylist.h"
#include "community_functions.h"


using namespace std;

Community* community_init(Graph* graph, long id){
    Community* comm = new Community();

    comm->ID = id;
    comm->pr_value = 0.0;
    comm->inEdges = 0;
    comm->outEdges = 0;
    comm->nodes.resize(1);
    comm->nodes[0] = id;
    comm->number_triangles = 0;
    comm->number_neighbour = 0;

    return comm;
}

//called only at the beginning
void community_set_node(Graph* graph, Community* community){
    long id = community->ID;
    community->outEdges = graph->nodes[id].degree;

    for (int i = 0; i < graph->nodes[id].degree; ++i) {
        community->neighbours.push_back(graph->nodes[id].neighbours[i]);
        community->neighbours_comm.push_back(graph->nodes[id].neighbours[i]);
        community->neighbours_weight.push_back(1);
        community->number_neighbour++;
    }
}

/// RUN ONLY AT THE BEGINNING
/// \param community
void community_fix_duplicates_and_order(Community* community){
    sort(community->neighbours_triangle.begin(), community->neighbours_triangle.begin() + community->neighbours_triangle.size());

    for (int i = 1; i < community->number_triangles; ++i) {
        if(community->neighbours_triangle[i-1] == community->neighbours_triangle[i]){
            community->neighbours_triangle.erase(community->neighbours_triangle.begin()+i);
            community->number_triangles--;
            i--;
        }
    }
}

void community_deinit(Community* community){
    delete community;
}

// O(4n) + O(n^2) due to notification
void community_merge(Graph* graph, Community* dst, Community*  src){
    if(dst == NULL || src == NULL)
        return;

    //cout << "[COM-MERGE]: Merging " << dst->ID << " and " << src->ID << endl;
    long weight = dst->neighbours_weight[community_get_neighbour_index(dst, src->ID)];

    dst->pr_value += src->pr_value;
    dst->inEdges += weight + src->inEdges;
    dst->outEdges += src->outEdges - 2*weight;

    //community_print(dst);
    //community_print(src);

    //add new nodes to the comm
    //cout << "[COM-MERGE]: Adding nodes" << endl;
    for (int i = 0; i < src->nodes.size(); ++i) {
        dst->nodes.push_back(src->nodes[i]);
    }

    //cout << "[COM-MERGE]: Intersecting neighbours" << endl;
    community_vector_intersect_neighbours(graph, dst, src);
    //cout << "[COM-MERGE]: Intersecting triangles" << endl;
    community_vector_intersect_triangles(graph, dst, src);

    //community_print(dst);

    //cout << "[COM-MERGE]: Fix neighbours" << endl;
    //notify neighbours of the change
    for (int i = 0; i < dst->number_neighbour; ++i) {
        community_fix_when_neighbor_change(graph, graph->nodes[dst->neighbours[i]].community, dst, src);
    }
}

//if node represents the community or is the community (nodeID == communityID), the result of this function is the same
void community_vector_intersect_neighbours(Graph* graph, Community* dst, Community* src){
    //dst->neighbours.resize(dst->neighbours.size() + src->neighbours.size());
    //dst->neighbours_weight.resize(dst->neighbours_weight.size() + src->neighbours_weight.size());
    int i, j;
    for (i = 0, j=0; i < dst->number_neighbour && j < src->number_neighbour; ) {
        //if node is actually in one of 2 comm --> remove it or go ahead
        if(graph->nodes[dst->neighbours[i]].community->ID == dst->ID || graph->nodes[dst->neighbours[i]].community->ID == src->ID){
            dst->neighbours.erase(dst->neighbours.begin()+i);
            dst->neighbours_weight.erase(dst->neighbours_weight.begin()+i);
            dst->neighbours_comm.erase(dst->neighbours_comm.begin()+i);
            dst->number_neighbour--;
            //i++;
            continue;
        }
        if(graph->nodes[src->neighbours[j]].community->ID == dst->ID || graph->nodes[src->neighbours[j]].community->ID == src->ID){
            j++;
            continue;
        }

        if(dst->neighbours[i] == src->neighbours[j]){
            dst->neighbours_weight[i] += src->neighbours_weight[j];
            i++;
            j++;
        }
        else if(dst->neighbours[i] < src->neighbours[j]){
            i++;
        }
        else{
            dst->neighbours.insert(dst->neighbours.begin()+i, src->neighbours[j]);
            dst->neighbours_weight.insert(dst->neighbours_weight.begin()+i, src->neighbours_weight[j]);
            dst->neighbours_comm.insert(dst->neighbours_comm.begin()+i, src->neighbours_comm[j]);
            dst->number_neighbour++;
            i++;
            j++;
        }
    }
    //check last dst neighbours and remove if they are inside the new community
    for(; i < dst->number_neighbour; ){
        if(graph->nodes[dst->neighbours[i]].community->ID == dst->ID || graph->nodes[dst->neighbours[i]].community->ID == src->ID){
            dst->neighbours.erase(dst->neighbours.begin()+i);
            dst->neighbours_weight.erase(dst->neighbours_weight.begin()+i);
            dst->neighbours_comm.erase(dst->neighbours_comm.begin()+i);
            dst->number_neighbour--;
            //i++;
            continue;
        }
        else
            i++;
    }

    //check last src neighbours and add them if missing
    for (; j < src->number_neighbour; j++) {
        if(graph->nodes[src->neighbours[j]].community->ID == dst->ID || graph->nodes[src->neighbours[j]].community->ID == src->ID){
            continue;
        }

        dst->neighbours.push_back(src->neighbours[j]);
        dst->neighbours_weight.push_back(src->neighbours_weight[j]);
        dst->neighbours_comm.push_back(src->neighbours_comm[j]);
        dst->number_neighbour++;
        i++;
    }
}

void community_vector_intersect_triangles(Graph* graph, Community* dst, Community* src){
    //dst->neighbours.resize(dst->neighbours_triangle.size() + src->neighbours_triangle.size());
    int i,j;
    for (i = 0, j=0; i < dst->number_triangles && j < src->number_triangles;) {
        //if node is actually in one of 2 comm --> remove it or go ahead
        if(graph->nodes[dst->neighbours_triangle[i]].community->ID == dst->ID || graph->nodes[dst->neighbours_triangle[i]].community->ID == src->ID){
            dst->neighbours_triangle.erase(dst->neighbours_triangle.begin()+i);
            dst->number_triangles--;
            //i++;
            continue;
        }
        if(graph->nodes[src->neighbours_triangle[j]].community->ID == dst->ID || graph->nodes[src->neighbours_triangle[j]].community->ID == src->ID){
            j++;
            continue;
        }

        if(dst->neighbours_triangle[i] == src->neighbours_triangle[j]){
            i++;
            j++;
        }
        else if(dst->neighbours_triangle[i] < src->neighbours_triangle[j]){
            i++;
        }
        else{
            dst->neighbours_triangle.insert(dst->neighbours_triangle.begin()+i, src->neighbours_triangle[j]);
            dst->number_triangles++;
            i++;
            j++;
        }
    }

    //check last dst neighbours and remove if they are inside the new community
    for(; i < dst->number_triangles; ){
        if(graph->nodes[dst->neighbours_triangle[i]].community->ID == dst->ID || graph->nodes[dst->neighbours_triangle[i]].community->ID == src->ID){
            dst->neighbours_triangle.erase(dst->neighbours_triangle.begin()+i);
            dst->number_triangles--;
        }
        else
            i++;
    }

    //check last src neighbours and add them if missing
    for (; j < src->number_triangles; j++) {
        if(graph->nodes[src->neighbours_triangle[j]].community->ID == dst->ID || graph->nodes[src->neighbours_triangle[j]].community->ID == src->ID){
            continue;
        }

        dst->neighbours_triangle.push_back(src->neighbours_triangle[j]);
        dst->number_triangles++;
    }
}

bool community_contains_neighbour(Community* community, long node){
    //Pre checks
    if(community == NULL)
        return false;

    //Binary search
    long start = 0;
    long end = community->number_neighbour-1;
    long m;

    while(start<=end){
        m = (start+end)/2;

        if(community->neighbours[m] == node)
            return true;
        else if(community->neighbours[start] == node)
            return true;
        else if(community->neighbours[end] == node)
            return true;
        else if(community->neighbours[m] < node)
            start = m+1;
        else
            end = m-1;
    }

    return false;
}

bool community_get_neighbour_index(Community* community, long node){
    //Pre checks
    if(community == NULL)
        return -1;

    //Binary search
    long start = 0;
    long end = community->number_neighbour-1;
    long m;

    while(start<=end){
        m = (start+end)/2;

        if(community->neighbours[m] == node)
            return m;
        else if(community->neighbours[start] == node)
            return start;
        else if(community->neighbours[end] == node)
            return end;
        else if(community->neighbours[m] < node)
            start = m+1;
        else
            end = m-1;
    }

    return -1;
}

long community_first_greater_than(Community* community, long node){
    if(community == NULL)
        return -1;
    if(community->nodes.size() <= 0)
        return -1;

    //search the first node with greatest (or equal) id than the given one
    long start = 0;
    long end = community->nodes.size()-1;
    long m;
    long pos;

    if(node < community->nodes[start])
        return 0; //I have to more the whole array

    if(node > community->nodes[end])
        return -1;  //No nodes greater than the input

    while(start <= end){
        //if m-th node is bigger
        if(community->nodes[m] > node){
            //check if m-1th node is smaller -> if yes return m as the first node greater than the input
            if(community->nodes[m-1] < node)
                return m;
            else
                end = m-1;
        }
        else
            start = m+1;
    }
    return -1;
}

//Called when a neighbor (COMMUNITY) change community
//We may have 2 neighbour with the same community
//or just 1
//because we see only communities, therefore for each merge, only 2 communities are merged
void community_fix_when_neighbor_change(Graph* graph, Community* community, Community* new_community, Community* old_community){
    if(community == NULL || new_community == NULL || old_community == NULL)
        return;

    //search the communityID in our neighbourhood and count how many neighbours we have with the same ID of the new community
    long count = 0;
    long old_comm_pos = -1;
    long new_comm_pos = -1;

    //cout << "[COMM-FIX]: Fixing community " << community->ID << " due to merge of " << new_community->ID << " and " << old_community->ID << endl;
//    cout << "[COMM-FIX]: Neighbours of community " << community->ID << ": ";
//    for (int i = 0; i < community->number_neighbour; ++i) {
//        cout << community->neighbours[i] << " (" << community->neighbours_weight[i] << ") ";
//    }
//    cout << endl;

    //cout << "[COMM-FIX]: Counting how many times there is the same community in the neighborhood" << endl;
    for (int i = 0; i < community->number_neighbour; ++i) {
        if(community->neighbours[i] == old_community->ID){
            //community->neighbours[i] = new_community->ID;
            old_comm_pos = i;
            count++;
        }
        else if(community->neighbours[i] == new_community->ID){
            new_comm_pos = i;
            count++;
        }
    }

    if(count > 2)
        cout << "[ERROR] - Community Fix: count > 2" << endl;
    if(count <= 0)
        cout << "[ERROR] - There are no neighbours with that community" << endl;

    //no changes in the neighborhood
    if(old_comm_pos < 0){
        return;
    }

    //if there are more (no more than 2!) --> remove the old which changed, since it is in the wrong position (the other one is already in a ordered position)
    //and fix weights
    if(count>1){
        //add weights to the new community
        //cout << "[COMM-FIX]: More than 1 --> fix weight and remove the duplicate" << endl;
        community->neighbours_weight[new_comm_pos] += community->neighbours_weight[old_comm_pos];

        //erase old comm
        community->neighbours.erase(community->neighbours.begin()+old_comm_pos);
        community->neighbours_weight.erase(community->neighbours_weight.begin()+old_comm_pos);
        community->neighbours_comm.erase(community->neighbours_comm.begin()+old_comm_pos);
        community->number_neighbour--;
    }else{
        //at least ONE MUST EXIST
        //change ID and fix the position
        //cout << "[COMM-FIX]: Only 1 ref, change value from " << community->neighbours[old_comm_pos] << " -> " << new_community->ID << endl;
        community->neighbours[old_comm_pos] = new_community->ID;
        community->neighbours_comm[old_comm_pos] = new_community->ID;

        //fix position
        //cout << "[COMM-FIX]: Sort" << endl;
        community_sort_neighbour(community, old_comm_pos);
    }

//    cout << "[COMM-FIX]: Final Neighbours: ";
//    for (int i = 0; i < community->number_neighbour; ++i) {
//        cout << community->neighbours[i] << " (" << community->neighbours_weight[i] << ") ";
//    }
//    cout << endl;
}

void community_sort_neighbour(Community* community, long neighbour_index){
    if(community == NULL)
        return;

    //check to move it to the right (if nexts are smaller
    while (neighbour_index < (community->number_neighbour-1) && community->neighbours[neighbour_index] > community->neighbours[neighbour_index+1]){
        //cout << neighbour_index << " < " << community->neighbours.size()-1 << endl;
        community_swap_vector(community->neighbours, neighbour_index, neighbour_index+1);
        community_swap_vector(community->neighbours_weight, neighbour_index, neighbour_index+1);
        community_swap_vector(community->neighbours_comm, neighbour_index, neighbour_index+1);
        neighbour_index++;
    }
    while (neighbour_index > 0 && community->neighbours[neighbour_index] < community->neighbours[neighbour_index-1]){
        //cout << neighbour_index << " > 0 " << endl;
        community_swap_vector(community->neighbours, neighbour_index, neighbour_index-1);
        community_swap_vector(community->neighbours_weight, neighbour_index, neighbour_index-1);
        community_swap_vector(community->neighbours_comm, neighbour_index, neighbour_index-1);
        neighbour_index--;
    }
}

void community_swap_vector(vector<long> &array, long index1, long index2){
    long tmp = array.at(index1);
    array.at(index1) = array.at(index2);
    array.at(index2) = tmp;
}

void community_print(Community* community){
    cout << endl;
    cout << "Community " << community->ID << endl;
    cout << "\tPagerank Value: " << community->pr_value << endl;
    cout << "\t#Inside Links: " << community->inEdges << endl;
    cout << "\t#Outside Links: " << community->outEdges << endl;
    cout << "\t#Nodes: " << community->nodes.size() << endl;
    cout << "\t#Neighbours: " << community->number_neighbour << endl;
    cout << "\t#Triangles: " << community->number_triangles << endl;

    cout << "\tNodes: ";
    for (int i = 0; i < community->nodes.size(); ++i) {
        cout << community->nodes[i] << " ";
    }
    cout << endl;

    cout << "\tNeighbour: ";
    for (int i = 0; i < community->number_neighbour; ++i) {
        cout << community->neighbours[i] << " (" << community->neighbours_weight[i] << ") ";
    }
    cout << endl;

    cout << "\tTriangles with: ";
    for (int i = 0; i < community->number_triangles; ++i) {
        cout << community->neighbours_triangle[i] << " ";
    }
    cout << endl;

    cout << endl << endl;
}

/*
 * Conductance (Yang and Leskovec, 2012):
 * f(S) = c/(2m+c) measures the fraction of total edge volume that points outside the cluster,
 * S is the set of nodes, m the number of edges in S, m = |{(u, v) ∈ E : u ∈ S, v ∈ S}|;
 * and c, the number of edges on the boundary of S, c = |{(u, v) ∈ E : u ∈ S, v not ∈ S}|.
 *  ==> Probability to exit from the community
 */

// Keep intersection in an array to compute it once for node for iteration
//O(log n)
//the smaller the better
bool community_quality_function_v0(Community* dst, Community* src, long intersections, long triangles){
    long src_pos = community_get_neighbour_index(dst, src->ID);
    long weight = dst->neighbours_weight[src_pos];

    if(triangles <= 0 && intersections<=0 && weight<=log(dst->nodes.size()))
        return false;

    double before_val = (double) dst->outEdges / (double)(2*dst->inEdges + dst->outEdges);
    double after_val = (double) (dst->outEdges + src->outEdges - 2*weight - intersections) / (double) (2*(dst->inEdges + weight + src->inEdges) + dst->outEdges + src->outEdges - 2*weight - intersections);
    cout << "\t" << before_val << " > " << after_val << endl;

    if(before_val > after_val)
        return true;
    return false;
}

bool community_quality_function(Community* dst, Community* src, long intersections, long triangles){
    long src_pos = community_get_neighbour_index(dst, src->ID);
    long weight = dst->neighbours_weight[src_pos];

    double before_scaled_density = (double) (2 * dst->inEdges) / ((double)(dst->nodes.size() - 1));

    if(dst->nodes.size() <= 1)
        before_scaled_density = 0;

    double after_scaled_density = 2.0 * (double)(dst->inEdges + src->inEdges + weight) / ((double) dst->nodes.size() + src->nodes.size() - 1);

    cout << "\t" << after_scaled_density << " > " << before_scaled_density << endl;

    if(before_scaled_density < after_scaled_density)
        return true;
    return false;
}

//Internal density
// number of internal links out of the number of total possible internal links
// the greater the better
bool community_quality_function_v2(Community* dst, Community* src, long intersections, long triangles){
    long src_pos = community_get_neighbour_index(dst, src->ID);
    long weight = dst->neighbours_weight[src_pos];

    //maybe consider intersections
    double before_density = (double) (2.0*dst->inEdges) / (double)(dst->nodes.size() * (dst->nodes.size() - 1.0));
    double after_density = (double) (2.0*(dst->inEdges + weight + src->inEdges)) / (double) ((dst->nodes.size() + weight + src->nodes.size()) * (dst->nodes.size() + weight + src->nodes.size() - 1.0));

    if(dst->nodes.size() <= 1)
        before_density = 0;


    cout << "\t" << 0.5*before_density << " < " << after_density << endl;

    if(after_density >= 0.1*before_density)
        return true;
    return false;
}

bool community_quality_function_v3(Community* dst, Community* src, long intersections, long triangles){
    long src_pos = community_get_neighbour_index(dst, src->ID);
    long weight = dst->neighbours_weight[src_pos];

    double before_prob_out = (double) dst->outEdges / (double)(2*dst->inEdges + dst->outEdges);
    double after_prob_out = (double) (dst->outEdges + src->outEdges - 2*weight - intersections) / (double) (2*(dst->inEdges + weight + src->inEdges) + dst->outEdges + src->outEdges - 2*weight - intersections);


    double before_prob_in = 1.0 - before_prob_out;
    double after_prob_in = 1.0 - after_prob_out;

    double before_density = (double) (2.0*dst->inEdges) / (double)(dst->nodes.size() * (dst->nodes.size() - 1.0));
    double after_density = (double) (2.0*(dst->inEdges + weight + src->inEdges)) / (double) ((dst->nodes.size() + weight + src->nodes.size()) * (dst->nodes.size() + weight + src->nodes.size() - 1.0));

    if(dst->nodes.size() <= 1) {
        before_density = 0.0;
    }

    cout << "\t" << before_density << " < " << after_density << endl;

    double before_final = 0.973 * before_prob_in + 0.027 * before_density;
    double after_final = 0.973 * after_prob_in + 0.027 * after_density;
    cout << after_density << " > " << before_final << endl;
    if(after_prob_in > before_prob_in)
        return true;
    return false;
}

//O(n)
//TODO: CHECK COMMUNITY ID ?
long community_get_intersections(Community* dst, Community* src){
    long count = 0;
    for (long i = 0, j=0; i < dst->number_triangles && j < src->number_triangles;) {
        if(dst->neighbours_triangle[i] == src->neighbours_triangle[j])
        {
            count++;
            i++;
            j++;
        }
        else if(dst->neighbours_triangle[i] < src->neighbours_triangle[j]){
            i++;
        }else{
            j++;
        }
    }
    return count;
}

long community_get_numbers_triangle_to_comm(Graph* graph, Community* dst, Community* src){
    long count = 0;
    for (long  j=0; j < src->number_triangles; ++j) {
        if(graph->nodes[src->neighbours_triangle[j]].community->ID == dst->ID)
            count++;
    }
    return count;
}

//bool community_quality_function_old(Community* dst, Community* src, long weight, long intersections){
//    double before_val = (double) dst->outEdges / (double)(2*dst->inEdges + dst->outEdges);
//    double after_val = (double) (dst->outEdges + src->outEdges - 2*weight - intersections) / (double) (2*(dst->inEdges + weight + src->inEdges) + dst->outEdges + src->outEdges - 2*weight - intersections);
//
//    if(before_val < after_val)
//        return true;
//    return false;
//}

double community_get_merge_value(Community* dst, Community* src, long intersection, long triangles){
    if(dst == NULL || src == NULL)
        return -1;

    long src_pos = community_get_neighbour_index(dst, src->ID);
    long weight = dst->neighbours_weight[src_pos];
    // final degree considering all weights ?
    // or final degree as the number of neighbour?
    //I go with the second one
    long final_out_degree = dst->outEdges + src->outEdges - 2*weight - intersection;
    //long final_out_degree = dst->neighbours.size() + src->neighbours.size() - 2 - intersection;
    double val = dst->pr_value * (double)triangles * (double)intersection / (double)final_out_degree ;
    return val;
}

double community_get_merge_value_v1(Community* dst, Community* src, long intersections, long triangles){
    long src_pos = community_get_neighbour_index(dst, src->ID);
    long weight = dst->neighbours_weight[src_pos];

    return intersections;
}

double community_get_merge_value_v2(Community* dst, Community* src, long intersection, long triangles){
    if(dst == NULL || src == NULL)
        return -1;

    long src_pos = community_get_neighbour_index(dst, src->ID);
    long weight = dst->neighbours_weight[src_pos];

    double after_scaled_density = 2.0 * (double)(dst->inEdges + src->inEdges + weight) / ((double) dst->nodes.size() + src->nodes.size() - 1);

    return after_scaled_density;
}