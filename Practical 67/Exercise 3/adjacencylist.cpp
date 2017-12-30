#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <utility>
#include "adjacencylist.h"
#include "community.h"

using namespace std;

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

Graph* graph_init(long number_nodes, long offset){
    Graph* graph = new Graph();

    graph->number_nodes = number_nodes;
    graph->number_edges = 0;
    graph->offset = offset;
    graph->number_communities = number_nodes;

    graph->nodes = new Node[graph->number_nodes];

    for (int i = 0; i < graph->number_nodes; ++i) {
        graph->nodes[i].id = i;
        graph->nodes[i].degree = 0;
        graph->nodes[i].number_triangles = 0;
        graph->nodes[i].community = community_init();
        community_insert(graph->nodes[i].community, i);
    }

    return graph;
}

void graph_deinit(Graph* graph){
    if(graph == NULL)
        return;

    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->nodes[i].id >= 0)
            delete graph->nodes[i].community;
    }

    delete[](graph->nodes);
    delete(graph);

    graph = NULL;
}

Graph* graph_load_file(char* name){
    Graph* graph;
    fstream input_graph;
    input_graph.open(name);

    if(!input_graph)
        return NULL;

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

    //graph_compute_degree(graph, edgelist);

    graph_load_data(graph, edgelist);

    return graph;
}

Graph* graph_load_edgelist(Edgelist* edgelist, long number_nodes, long offset){
    Graph* graph;

    //graph_compute_size(edgelist, number_nodes, number_edges, offset);

    graph = graph_init(number_nodes, offset);

    //graph_compute_degree(graph, edgelist);

    graph_load_data(graph, edgelist);

    return graph;
}

Edgelist* graph_file_to_edgelist(fstream &file){
    Edgelist* edgeslist = new Edgelist();

    long nodeA;
    long nodeB;

    while(file>>nodeA){
        file>>nodeB;

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

    // Count how many couples there are in the file
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
    //If array for the graph degree is not initialized, return false
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
        //graph->nodes[node].degree--;
        return false;
    }


    if(graph_contains_edge(graph, node, neighbour)){
        //graph->nodes[node].degree--;
        return false;
    }

    //cout << "Adding "<<neighbour<<" to "<<node<<endl;
    int i=0;
    //TODO: change using binary search
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

    while (start < end){
        m = (start+end) / 2;

        if(graph->nodes[src].neighbours[m] == dst)
            return true;
        else if(graph->nodes[src].neighbours[start] == dst)
            return true;
        else if(graph->nodes[src].neighbours[end] == dst)
            return true;
        else if(graph->nodes[src].neighbours[m] < dst){
            start = m+1;
        }
        else{
            end = m-1;
        }
    }

    return false;
}

long graph_get_edge(Graph* graph, long src, long dst){
    //cout << "[GRGETEDGE]: Prechecks" << endl;
    if(graph == NULL)
        return NULL;

    cout << "[GRGETEDGE]: Node " << src << " with neighbor " << dst << endl;
    //cout << "[GRGETEDGE]: Neighbors = ";
    //graph_print_array(graph, src);


    //cout << "[GRGETEDGE]: Defining variables" << endl;
    long start = 0;
    long end = graph->nodes[src].degree - 1;
    long m;

    //cout << "[GRGETEDGE]: Iterate..." << endl;
    while (start <= end){
        //cout << "[GRGETEDGE]: Start = " << start << " | End = " << end << endl;
        m = (start+end) / 2;
        //cout << "[GRGETEDGE]: m="<< graph->neighbours[m] << endl;

        //cout << "[GRGETEDGE]: Check if neighbor is on the left or right" << endl;
        //cout << "[GRGETEDGE]: " << graph->neighbours[m] << " < " << dst << endl;
        if(graph->nodes[src].neighbours[m] == dst) {
            cout << "[GRGETEDGE]: Return " << m << endl;
            return m;
        }
        else if(graph->nodes[src].neighbours[start] == dst){
            cout << "[GRGETEDGE]: Return " << start << endl;
            return start;
        }
        else if(graph->nodes[src].neighbours[end] == dst){
            cout << "[GRGETEDGE]: Return " << end << endl;
            return end;
        }
        else if(graph->nodes[src].neighbours[m] < dst){
            //cout << "[GRGETEDGE]: Go on right" << endl;
            start = m+1;
        }
        else{
            //cout << "[GRGETEDGE]: Go on left" << endl;
            end = m-1;
        }
    }

    cout << "[GRGETEDGE]: Not found" << endl;
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
    /*for(int i=0;i<graph->number_nodes;i++){
        Node node = graph->nodes[i];
        //cout << "Considering node "<<node.id<<" with degree "<<node.degree<<endl;
        for(int j=0;j<node.degree;j++){
            if(node.id < node.neighbours[j])
                cout << node.id << " " << node.neighbours.at(j) << endl;
        }
    }*/

    for(int i=0;i<graph->number_nodes;i++){
        Node node = graph->nodes[i];
        if(node.id < 0)
            continue;
        cout << node.id << ": ";
        //cout << "Considering node "<<node.id<<" with degree "<<node.degree<<endl;
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
        //cout << "Considering node "<<node.id<<" with degree "<<node.degree<<endl;
        for(int j=0;j<node.degree;j++){
            if(node.id < node.neighbours[j])
                output << node.id << " " << node.neighbours.at(j) << endl;
        }
    }

    output.close();
}




long graph_compute_triangles(Graph* graph){
    if(graph == NULL)
        return 0;

    long count = 0;

    for(int i=0;i<graph->number_nodes; i++){
        graph->nodes[i].number_triangles = graph_count_triangles_node(graph, i);
        count += graph->nodes[i].number_triangles;
    }

    return count;
}

long graph_count_triangles_node(Graph* graph, long node){
    if(graph == NULL || node < 0)
        return 0;

    long count = 0;

    for(int j=0; j < graph->nodes[node].degree; j++){
        long neighbor = graph->nodes[node].neighbours.at(j);

        long start_nodeV = graph_index_least_great_element(graph->nodes[node].neighbours, graph->nodes[node].degree, neighbor);
        long start_nodeU = graph_index_least_great_element(graph->nodes[neighbor].neighbours, graph->nodes[neighbor].degree, neighbor);

        //cout << "Node " << node << " neighbor " << neighbor << endl;
        //cout << "Node start: " << start_nodeV << " end: " << end_nodeV << endl;
        //cout << "Neigh start: " << start_nodeU << " end: " << end_nodeU << endl;

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
        // If equal --> intersection
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


long graph_index_least_great_element(vector<long> array, long array_length, long value){
    // Array is already ordered
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

void graph_communities_to_file(Graph* graph, char* filename){
    cout << "[GR-COMM-FILE] Initializing" << endl;
    ofstream input;
    cout << "[GR-COMM-FILE] Opening file" << endl;
    input.open(filename, ios::out);

    //COL1 = node_id
    //COL2 = cluster_id
    cout << "[GR-COMM-FILE] Iterating on all nodes. If node-id > 0 then it is still a community" << endl;
    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->nodes[i].id < 0)
            continue;

        cout << "[GR-COMM-FILE] Community " << i << " with number elements = " << graph->nodes[i].community->number_nodes << endl;
        for (int j = 0; j < graph->nodes[i].community->number_nodes; ++j) {
            input << graph->nodes[i].community->array[j] + graph->offset << " " << i << endl;
        }
    }

    cout << "[GR-COMM-FILE] Closing file" << endl;
    input.close();
}

long* graph_get_communities(Graph* graph){
    if(graph == NULL)
        return NULL;

    long* communities = new long[graph->number_communities];
    long comm_index = 0;

    cout << "#######################" << endl;
    cout << "Available communities: ";
    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->nodes[i].id >= 0)
        {
            communities[comm_index++] = i;
            cout << i << " ";
        }
    }
    cout << endl;
    cout << "#######################" << endl;

    return communities;
}

bool graph_substitute_neighbour(Graph* graph, long node, long old_node, long new_node){
    cout << "[GRSUB]: Prechecks: Node:" << node << ", Old Node: " << old_node << ", New Node: " << new_node << endl;
    if(graph == NULL)
        return false;

    if(node == new_node)
        return false;

    cout << "[GRSUB]: Getting indexes of neighbors" << endl;
    long index_old_node = graph_get_edge(graph, node, old_node);
    long index_new_node = graph_get_edge(graph, node, new_node);

    //cout << "[GRSUB]: Neighbors of " << node << ": ";
    //graph_print_array(graph, node);

    //new node is not contained in neighbours of node
    //cout << "[GRSUB]: Checking indexes" << endl;
    if(index_new_node < 0){
        //change link
        if(index_old_node >= 0) {
            cout << "[GRSUB]: The new node is not a neighbor of " << node << endl;
            //TODO: FIX PROBLEM --> the edge doesn't have !
            long weight = graph->nodes[node].weight.at(index_old_node);
            graph->nodes[node].neighbours.at(index_old_node) = new_node;
            graph_sort_new_neighbor(graph, node, index_old_node);
            graph_add_edge(graph, new_node, node, weight);
        }
    }
    else{
        cout << "[GRSUB]: There are both old and new node -> fix weight" << endl;
        //cout << "[GRSUB]: Neighbours of " << node << ": ";
        //graph_print_array(graph, node);
        //cout << "[GRSUB]: Neighbours of " << new_node << ": ";
        //graph_print_array(graph, new_node);
        //change weight and remove the old node
        cout << "[GRSUB]: Getting index for weight" << endl;
        long index_node_from_new_node = graph_get_edge(graph, new_node, node);
        //update weight from both directions
        cout << "[GRSUB]: Computing weight" << endl;
        graph->nodes[node].weight[index_new_node] += graph->nodes[node].weight[index_old_node];
        cout << "[GRSUB]: Weight " << node << " -> " << new_node << " = " << graph->nodes[node].weight[index_new_node] << endl;
        graph->nodes[new_node].weight[index_node_from_new_node] += graph->nodes[node].weight[index_old_node];
        cout << "[GRSUB]: Weight " << new_node << " -> " << node << " = " << graph->nodes[new_node].weight[index_node_from_new_node] << endl;
        cout << "[GRSUB]: Remove edge" << endl;
        graph_remove_edge(graph, node, old_node);
    }

    //cout << "[GRSUB]: Neighbors of " << node << " after modification: ";
    //graph_print_array(graph, node);
    cout << "[GRSUB]: Return" << endl;
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

bool graph_merge_communities(Graph* graph, long v, long u){
    cout << "[GRMERGE]: Prechecks. Communities: " << v << ", " << u << endl;
    if(graph == NULL)
        return false;

    //cout << "[GRMERGE]: Links of community " << v << ": ";
    //graph_print_array(graph, v);

    //cout << "[GRMERGE]: Links of community " << u << ": ";
    //graph_print_array(graph, u);

    //merge 2 communities
    cout << "[GRMERGE]: Get merged community" << endl;

    cout << "[COMMERGE]: Prechecks #1" << endl;
    if(graph->nodes[v].community == NULL || graph->nodes[u].community == NULL){
        cout << "--------- ERRORE ---------" << endl;
    }

    Community* final = community_merge(graph->nodes[v].community, graph->nodes[u].community);

    if(final == NULL)
        return false;

    cout << "[GRMERGE]: Free old communities" << endl;
    if(graph->nodes[v].community != NULL)
        community_deinit(graph->nodes[v].community);
    if(graph->nodes[u].community != NULL)
        community_deinit(graph->nodes[u].community);

    graph->nodes[v].community = NULL;
    graph->nodes[u].community = NULL;

    cout << "[GRMERGE]: Set new community" << endl;
    graph->nodes[v].community = final;

    //merge neighbours
    //for (int j = 0; j < graph->nodes[u].degree; ++j) {
    //    graph_add_edge(graph, v, graph->nodes[u].neighbours.at(j), graph->nodes[u].weight.at(j));
    //}

    //fix all neighbours of u such that they points against v
    cout << "[GRMERGE]: Fix all neighbors with the newest comm. ("<< graph->nodes[u].degree << ")" << endl;
    for (long i = 0; i < graph->nodes[u].degree; ++i) {
        long neighbours = graph->nodes[u].neighbours[i];

        if(neighbours == v)
            continue;

        cout << "[GRMERGE]: Substite the old comm with the new one (" << neighbours << ", old=" << u << ", new=" << v << ")" << endl;
        graph_substitute_neighbour(graph, neighbours, u, v);
    }

    //remove old link between the 2 communities
    //cout << "[GRMERGE]: Links before removement: ";
    //graph_print_array(graph, v);
    cout << "[GRMERGE]: Removing from " << v << " the neighbour " << u << endl;
    graph_remove_edge(graph, v, u);
    //cout << "[GRMERGE]: Links after removement: ";
    //graph_print_array(graph, v);

    cout << "[GRMERGE]: Decrease number of comm. and set one of the 2 comm as -1" << endl;
    graph->number_communities--;

    graph->nodes[u].id = -1;

    //cout << "[GRMERGE]: Final community = ";
    //community_print(graph->nodes[v].community);

    cout << "[GRMERGE]: Return" << endl;
    return true;
}

long graph_get_sum_weight(Graph* graph, long node){
    if(graph == NULL)
        return 0;

    long sum = 0;
    for (int i = 0; i < graph->nodes[node].weight.size(); ++i) {
        sum += graph->nodes[node].weight[i];
    }
    return sum;
}

long graph_get_max_weight(Graph* graph, long node){
    if(graph == NULL)
        return 0;

    long max = 0;
    long weight = 0;
    for (int i = 0; i < graph->nodes[node].weight.size(); ++i) {
        weight = graph->nodes[node].weight[i];
        weight > max ? max = weight : NULL;
    }
    return max;
}