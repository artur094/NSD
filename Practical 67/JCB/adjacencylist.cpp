#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <utility>
#include "community.h"
#include "adjacencylist.h"
#include "community_functions.h"


using namespace std;

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

Graph* graph_init(long number_nodes, long offset){
    Graph* graph = new Graph();

    graph->number_nodes = number_nodes;
    graph->number_edges = 0;
    graph->offset = offset;

    graph->nodes = new Node[graph->number_nodes];

    for (int i = 0; i < graph->number_nodes; ++i) {
        graph->nodes[i].id = i;
        graph->nodes[i].degree = 0;
        graph->nodes[i].number_triangles = 0;
        graph->nodes[i].community = community_init(graph, i);
    }

    return graph;
}

void graph_deinit(Graph* graph){
    if(graph == NULL)
        return;

    delete[](graph->nodes);
    delete(graph);

    graph = NULL;
}

Graph* graph_load_file(char* name){
    Graph* graph;
    fstream input_graph;
    input_graph.open(name);

    Edgelist* edgelist = graph_file_to_edgelist(input_graph);
    graph = graph_load_edgelist(edgelist);


    delete(edgelist);
    input_graph.close();

    return graph;
}

Graph* graph_load_edgelist(Edgelist* edgelist){
    Graph* graph;

    long number_nodes = 0;
    long number_edges = 0;
    long offset = 0;

    graph_compute_size(edgelist, number_nodes, number_edges, offset);
    graph = graph_init(number_nodes, offset);
    graph_load_data(graph, edgelist);

    for (int i = 0; i < graph->number_nodes; ++i) {
        community_set_node(graph, graph->nodes[i].community);
    }

    return graph;
}

Graph* graph_load_edgelist(Edgelist* edgelist, long number_nodes, long offset){
    Graph* graph;

    graph = graph_init(number_nodes, offset);
    graph_load_data(graph, edgelist);

    return graph;
}

Edgelist* graph_file_to_edgelist(fstream &file){
    Edgelist* edgeslist = new Edgelist();

    long nodeA;
    long nodeB;

    while(file>>nodeA>>nodeB){
        edgeslist->push_back(make_pair(nodeA, nodeB));
    }

    return edgeslist;
}


bool graph_compute_size(Edgelist* edgelist, long &number_nodes, long &number_edges, long &offset){
    if(!edgelist)
        return false;

    long nodeA;
    long nodeB;

    long min_id = LONG_MAX;
    long max_id = 0;

    for(int i=0;i<edgelist->size();i++){
        nodeA = edgelist->at(i).first;
        nodeB = edgelist->at(i).second;

        if(nodeA!=nodeB)
            number_edges+=2;

        nodeA < min_id ? min_id = nodeA : NULL;
        nodeB < min_id ? min_id = nodeB : NULL;

        nodeA > max_id ? max_id = nodeA : NULL;
        nodeB > max_id ? max_id = nodeB : NULL;
    }

    number_nodes = max_id - min_id + 1;
    number_edges = number_edges;
    offset = min_id;

    return true;
}

bool graph_compute_degree(Graph* graph, Edgelist* edgelist){
    if(graph == NULL)
        return false;

    long nodeA;
    long nodeB;

    for(int i=0; i<edgelist->size(); i++){
        nodeA = edgelist->at(i).first;
        nodeB = edgelist->at(i).second;

        nodeA -= graph->offset;
        nodeB -= graph->offset;

        graph->nodes[nodeA].degree++;
        graph->nodes[nodeB].degree++;
    }

    for (int i = 0; i < graph->number_nodes; ++i) {
        graph->nodes[i].neighbours.resize(graph->nodes[i].degree);
    }

    return true;
}

bool graph_load_data(Graph* graph, Edgelist* edgelist){
    if(edgelist == NULL || graph == NULL)
        return false;

    long nodeA;
    long nodeB;

    for(int i=0;i<edgelist->size();i++){
        nodeA = edgelist->at(i).first;
        nodeB = edgelist->at(i).second;

        nodeA -= graph->offset;
        nodeB -= graph->offset;

        //Not direct graph --> I have to add the edge to both nodes
        graph_add_edge(graph, nodeA, nodeB, 1);
        graph_add_edge(graph, nodeB, nodeA, 1);
    }

    return true;
}

bool graph_add_edge(Graph* graph, long node, long neighbour, long weight){
    //Do not add if selp loop or existing edge
    if(graph_is_self_loop(node, neighbour)) {
        return false;
    }


    if(graph_contains_edge(graph, node, neighbour)){
        return false;
    }

    int i=0;
    for (i = 0; i < graph->nodes[node].degree && graph->nodes[node].neighbours[i] <= neighbour; ++i);

    graph->nodes[node].neighbours.insert(graph->nodes[node].neighbours.begin()+i, neighbour);
    graph->nodes[node].weight.insert(graph->nodes[node].weight.begin()+i, weight);
    graph->nodes[node].degree++;
    graph->number_edges++;

    return true;
}



bool graph_contains_edge(Graph* graph, long src, long dst){
    if(graph == NULL)
        return false;

    long start = 0;
    long end = graph->nodes[src].degree - 1;
    long m;

    while (start <= end){
        m = (start+end) / 2;

        if(graph->nodes[src].neighbours[m] == dst)
            return true;

        if(graph->nodes[src].neighbours[m] < dst){
            start = m+1;
        }
        else{
            end = m-1;
        }
    }

    return false;
}

long graph_get_edge(Graph* graph, long src, long dst){
    if(graph == NULL)
        return NULL;

    long start = 0;
    long end = graph->nodes[src].degree - 1;
    long m;

    while (start <= end){
        m = (start+end) / 2;

        if(graph->nodes[src].neighbours[m] == dst) {
            return m;
        }
        else if(graph->nodes[src].neighbours[start] == dst){
            return start;
        }
        else if(graph->nodes[src].neighbours[end] == dst){
            return end;
        }
        else if(graph->nodes[src].neighbours[m] < dst){
            start = m+1;
        }
        else{
            end = m-1;
        }
    }

    return -1;
}

bool graph_is_self_loop(int node, int neighbour){
    return node == neighbour;
}

void graph_print(Graph* graph){
    cout << "Graph:"<<endl;
    cout << "Number nodes: "<<graph->number_nodes<<endl;
    cout << "Number edges: "<<graph->number_edges<<endl;
    cout << "Offset: "<<graph->offset<<endl;

    for(int i=0;i<graph->number_nodes;i++){
        Node node = graph->nodes[i];
        if(node.id < 0)
            continue;
        cout << node.id << ": ";
        for(int j=0;j<node.degree;j++){
            cout << node.neighbours.at(j) << " (" << node.weight.at(j) << "), ";
        }
        cout << endl;
    }
}

bool graph_remove_edge(Graph* graph, long src, long dst){
    if(graph == NULL)
        return false;

    long index = graph_get_edge(graph, src, dst);

    if(index < 0)
        return false;


    graph->nodes[src].neighbours.erase(graph->nodes[src].neighbours.begin()+index);
    graph->nodes[src].weight.erase(graph->nodes[src].weight.begin()+index);
    graph->nodes[src].degree--;

    return true;
}

void graph_store_degree_file(char* filename, Graph* graph){
    long max = 0;
    long length;
    for (int i = 0; i < graph->number_nodes; ++i) {
        long degree = graph->nodes[i].degree;

        max = max < degree ? degree : max;
    }
    length = max+1;
    long* degree = new long[length];
    memset(degree, 0, (length)*sizeof(long));

    for (int i = 0; i < graph->number_nodes; ++i) {
        degree[graph->nodes[i].degree]++;
    }

    ofstream output;
    output.open(filename, ios::out);

    for(int i=0;i<length;i++){
        output << i << " " << degree[i] << endl;
    }

    output.close();
    delete[] degree;
}



void graph_store_file(char* filename, Graph* graph){
    ofstream output;
    output.open(filename, ios::out);

    for(int i=0;i<graph->number_nodes;i++){
        Node node = graph->nodes[i];
        for(int j=0;j<node.degree;j++){
            if(node.id < node.neighbours[j])
                output << node.id << " " << node.neighbours.at(j) << endl;
        }
    }

    output.close();
}


long graph_index_least_great_element(vector<long> array, long array_length, long value){
    for(int i=0;i<array_length; i++){
        if(array.at(i) > value)
            return i;
    }
    return -1;
}

void graph_print_array(Graph* graph, long node){
    if(graph == NULL)
        return;

    for (int i = 0; i < graph->nodes[node].degree; ++i) {
        cout << graph->nodes[node].neighbours[i] << " (" << graph->nodes[node].weight[i] << "), ";
    }
    cout << endl;
}

bool graph_substitute_neighbour(Graph* graph, long node, long old_node, long new_node){
    if(graph == NULL)
        return false;

    if(node == new_node)
        return false;

    long index_old_node = graph_get_edge(graph, node, old_node);
    long index_new_node = graph_get_edge(graph, node, new_node);

    if(index_new_node < 0){
        if(index_old_node >= 0) {
            long weight = graph->nodes[node].weight.at(index_old_node);
            graph->nodes[node].neighbours.at(index_old_node) = new_node;
            graph_sort_new_neighbor(graph, node, index_old_node);
            graph_add_edge(graph, new_node, node, weight);
        }
    }
    else{
        long index_node_from_new_node = graph_get_edge(graph, new_node, node);
        //update weight from both directions
        graph->nodes[node].weight[index_new_node] += graph->nodes[node].weight[index_old_node];
        graph->nodes[new_node].weight[index_node_from_new_node] += graph->nodes[node].weight[index_old_node];
        graph_remove_edge(graph, node, old_node);
    }

    return true;
}

void graph_sort_new_neighbor(Graph* graph, long node, long neighbor_index){
    if(graph == NULL)
        return;

    bool done = false;

    long prev, next;
    long first = 0;
    long last = graph->nodes[node].degree - 1;

    while (!done){
        prev = neighbor_index -1;
        next = neighbor_index +1;

        if(next > last)
            next = last;
        if(prev < first)
            prev = first;

        if(graph->nodes[node].neighbours[prev] <= graph->nodes[node].neighbours[neighbor_index] && graph->nodes[node].neighbours[neighbor_index] <= graph->nodes[node].neighbours[next])
            done = true;
        else if(graph->nodes[node].neighbours[prev] > graph->nodes[node].neighbours[neighbor_index]){
            swap_vector(&graph->nodes[node].neighbours, prev, neighbor_index);
            swap_vector(&graph->nodes[node].weight, prev, neighbor_index);
            neighbor_index = prev;
        }
        else if(graph->nodes[node].neighbours[neighbor_index] > graph->nodes[node].neighbours[next]){
            swap_vector(&graph->nodes[node].neighbours, next, neighbor_index);
            swap_vector(&graph->nodes[node].weight, next, neighbor_index);
            neighbor_index = next;
        }
    }
}

void swap_vector(vector<long>* array, long index1, long index2){
    long tmp = array->at(index1);
    array->at(index1) = array->at(index2);
    array->at(index2) = tmp;
}

void page_rank(Graph* graph, double alpha, int iterations){
    double* P = new double[graph->number_nodes];

    double probability_come_from_neighbour = 0;
    double probability_of_teletrasport = 0;

    for (int i = 0; i < graph->number_nodes; ++i) {
        P[i] = 1.0/((double)graph->number_nodes);
    }

    for (int k = 0; k < iterations; ++k) {
        for (long i = 0; i < graph->number_nodes; ++i) {
            probability_of_teletrasport = 1.0/((double)graph->number_nodes);
            probability_come_from_neighbour = 0;

            for (long j = 0; j < graph->nodes[i].degree; ++j) {
                long neighbor = graph->nodes[i].neighbours[j];

                probability_come_from_neighbour += (1.0 / ((double)graph->nodes[neighbor].degree)) * P[neighbor];
            }

            P[i] = (1.0-alpha) * probability_come_from_neighbour + alpha * probability_of_teletrasport;
        }
    }

    for (int i = 0; i < graph->number_nodes; ++i) {
        graph->nodes[i].community->pr_value = 0;
    }

    for (int i = 0; i < graph->number_nodes; ++i) {
        graph->nodes[i].community->pr_value += P[i];
    }

    delete [] P;
}


// ########### TRIANGLES ###############

long graph_compute_triangles(Graph* graph){
    if(graph == NULL)
        return 0;

    long count = 0;

    for(int i=0;i<graph->number_nodes; i++){
        //graph->nodes[i].number_triangles = graph_count_triangles_node(graph, i);
        graph_set_triangles_node(graph, i);
        count += graph->nodes[i].number_triangles;
    }

    return count;
}

void graph_set_triangles_node(Graph* graph, long node){
    if(graph == NULL || node < 0)
        return;

    for(int j=0; j < graph->nodes[node].degree; j++){
        long neighbor = graph->nodes[node].neighbours.at(j);

        for (int i = 0; i < graph->nodes[neighbor].degree; ++i) {
            long neighbor_d2 = graph->nodes[neighbor].neighbours.at(i);

            if(graph_contains_edge(graph, node, neighbor_d2)) {
                graph->nodes[node].triangles_nodes.push_back(neighbor_d2);
                graph->nodes[node].community->neighbours_triangle.push_back(neighbor_d2);
                graph->nodes[node].community->number_triangles++;
            }
        }
    }

    graph->nodes[node].number_triangles = graph->nodes[node].triangles_nodes.size();
}

long graph_count_triangles_node(Graph* graph, long node){
    if(graph == NULL || node < 0)
        return 0;

    long count = 0;

    for(int j=0; j < graph->nodes[node].degree; j++){
        long neighbor = graph->nodes[node].neighbours.at(j);

        long start_nodeV = graph_index_least_great_element(graph->nodes[node].neighbours, graph->nodes[node].degree, neighbor);
        long start_nodeU = graph_index_least_great_element(graph->nodes[neighbor].neighbours, graph->nodes[neighbor].degree, neighbor);

        if(start_nodeU >= 0 && start_nodeV >= 0)
            count += graph_count_intersections(graph->nodes[node].neighbours, graph->nodes[node].degree, start_nodeV, graph->nodes[neighbor].neighbours, graph->nodes[neighbor].degree, start_nodeU);
    }
    return count;
}

/**
 * Count how many intersections (same elements) there are in the 2 arrays
 * @param array1 Array of neighbors
 * @param start1 Starting point to count the intersections
 * @param end1 Last index to check
 * @param start2 Starting point to count the intersection
 * @param end2 Last index to check
 * @return
 */
long graph_count_intersections(vector<long> array1, long length1, long start1, vector<long> array2, long length2, long start2){
    long i = start1;
    long j = start2;
    long count = 0;

    while( i < length1 && j < length2 ){
        if(array1.at(i) == array2.at(j))
        {
            count++;
            i++;
            j++;
        }

        else if(array1.at(i) < array2.at(j))
            i++;

        else if(array2.at(j) < array1.at(i))
            j++;
    }

    return count;
}


void graph_print_communities(Graph* graph){
    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->nodes[i].community->ID == i){
            community_print(graph->nodes[i].community);
        }
    }
}

void graph_fix_communities(Graph* graph){
    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->nodes[i].community->ID == i){
            community_fix_duplicates_and_order(graph->nodes[i].community);
        }
    }
}

void graph_merge(Graph* graph, long nodeA, long nodeB){
    if(graph == NULL)
        return;

    Community* communityA = graph->nodes[nodeA].community;
    Community* communityB = graph->nodes[nodeB].community;

    //cout << "[GMERGE]: Changing communities to all neighbours" << endl;
    for (int i = 0; i < communityB->nodes.size(); ++i) {
        long n = communityB->nodes[i];

        graph->nodes[n].community = communityA;
    }

    //cout << "[GMERGE]: Merging communities " << communityA->ID << " and " << communityB->ID << endl;
    community_merge(graph, communityA, communityB);
    //cout << "[GMERGE]: Deleting old community" << endl;
    community_deinit(communityB);
}