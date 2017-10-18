#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <utility>
#include "adjacencylist.h"
#include "queue.h"

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

long* graph_degree_array(Graph* graph){
    //number edges is already double (it is increased for each add_edge, and since it is undirected we need to do 2 add_edge
    long *degree = new long[graph->number_edges];
    cout << "Number of nodes: "<<graph->number_nodes<<endl;
    cout << "Number of edges: "<<graph->number_edges<<endl;
    long index = 0;

    for (int i = 0; i < graph->number_nodes; ++i) {
        for (int j = 0; j < graph->nodes[i].degree; ++j) {
            cout << "Adding "<<i<<" to degree["<<index<<"]"<<endl;
            degree[index++] = i;
        }
    }
    return degree;
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
        graph_add_edge(graph, nodeA, nodeB);
        graph_add_edge(graph, nodeB, nodeA);
    }

    return true;
}

bool graph_add_edge(Graph* graph, long node, long neighbour){
    if(graph_is_self_loop(node, neighbour)) {
        //graph->nodes[node].degree--;
        return false;
    }

    if(graph_edge_exists(graph, node, neighbour)){
        //graph->nodes[node].degree--;
        return false;
    }

    //cout << "Adding "<<neighbour<<" to "<<node<<endl;
    int i=0;
    for (i = 0; i < graph->nodes[node].degree && graph->nodes[node].neighbours[i] <= neighbour; ++i);

    graph->nodes[node].neighbours.insert(graph->nodes[node].neighbours.begin()+i, neighbour);
    graph->nodes[node].degree++;
    graph->number_edges++;

    return true;
}



bool graph_edge_exists(Graph* graph, long node, long neighbour){
    Node n = graph->nodes[node];
    for(int i=0;i<n.degree;i++)
        if(n.neighbours[i] == neighbour)
            return true;
    return false;
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
        //cout << "Considering node "<<node.id<<" with degree "<<node.degree<<endl;
        for(int j=0;j<node.degree;j++){
            if(node.id < node.neighbours[j])
                cout << node.id << " " << node.neighbours.at(j) << endl;
        }
    }
}

void graph_store_degree_file(char* filename, Graph* graph){
    ofstream output;
    output.open(filename, ios::out);

    for(int i=0;i<graph->number_nodes;i++){
        Node node = graph->nodes[i];
        //cout << "Considering node "<<node.id<<" with degree "<<node.degree<<endl;
        output << node.id << " " << node.degree << endl;
    }

    output.close();
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

/* - - - - - - - - - - - - - - - - - ADDITIONAL FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

long diameter(Graph* graph){
    if(graph == NULL)
        return NULL;
    if(graph->number_nodes <= 0)
        return NULL;

    long* ids = new long[graph->number_nodes];
    memset(ids, -1, sizeof(long)*graph->number_nodes);
    long id = 0;
    long diameter = 0;
    long max_size = 0;
    double avg_distance = 0;

    for(int i=0;i<graph->number_nodes;i++){
        //If ID is not assigned to the node I, then run BFS
        long size = 0;
        long max_distance = 0;
        long last_node = i;
        long root = i;
        long tmp_max_distance = 0;

        if(ids[i] < 0){
            BFS_cc_diameter(graph, root, ids, id, size, max_distance, last_node);
            //cout << "CC of "<<i+graph->offset<<" with size "<<size<<" where the max distance is "<<max_distance<<" in node "<<last_node+graph->offset<<endl;

            long diff = 0;
            int count = 0;

            do{
                root = last_node;
                BFS_cc_diameter(graph, root, ids, id, size, tmp_max_distance, last_node);

                diff = tmp_max_distance - max_distance;

                max_distance = tmp_max_distance;

            }while(diff > 0 && count < 9);

            if(diameter < max_distance)
                diameter = max_distance;

            if(max_size < size)
                max_size = size;

            id++;

            avg_distance += max_distance;

        }
    }

    avg_distance /= (double)id;

    cout << endl << "Obtained diamater: " << diameter << endl;
    cout << "Maximum size: "<<max_size<<endl;
    cout << "Number of CC: "<<id<<endl;
    cout << "AVG Distance: "<<avg_distance<<endl;

    return diameter;
}

void BFS_cc_diameter(Graph* graph, long root, long* ids, long id, long &ids_length, long &max_distance, long &last_node){
    if(graph == NULL)
        return;
    if(graph->number_nodes <= 0)
        return;
    if(ids == NULL)
        return;

    Queue* queue = queue_init();

    bool* visited = new bool[graph->number_nodes];
    long* distance = new long[graph->number_nodes];

    memset(distance, 0, sizeof(long)*graph->number_nodes);
    memset(visited, false, sizeof(bool)*graph->number_nodes);

    queue_push(queue, root);
    visited[root] = true;
    ids[root] = id;
    ids_length = 1;
    last_node = root;
    max_distance = 0;

    while(! queue_is_empty(queue)){
        long node = queue_pop(queue);

        for (int i = 0; i < graph->nodes[node].degree; ++i) {
            long neighbour = graph->nodes[node].neighbours.at(i);

            if(!visited[neighbour] && neighbour >= 0){
                queue_push(queue, neighbour);
                visited[neighbour] = true;
                ids[neighbour] = id;
                ids_length++;

                distance[neighbour] = distance[node] + 1;

                if(distance[neighbour]>max_distance){
                    max_distance = distance[neighbour];
                    last_node = neighbour;
                }


            }
        }
    }

    queue_deinit(queue);

    delete[] visited;
    delete[] distance;
}

double tr(Graph* graph){
    double tr = 0.0;

    long count_triengles = 0;
    long count_pairs = 0;

    for(int i=0; i<graph->number_nodes; i++){
        if(graph->nodes[i].degree > 1) {
            count_triengles += graph->nodes[i].number_triangles;
            count_pairs += (graph->nodes[i].degree*(graph->nodes[i].degree-1))/2;
        }
    }

    tr = (double) count_triengles / (double) count_pairs;

    return tr;
}

double cc(Graph* graph){
    double cc = 0.0;
    double avg = 0.0;
    double possible_pairs = 0;
    long count = 0;

    for(int i=0; i<graph->number_nodes; i++){
        if(graph->nodes[i].degree > 1) {
            cc += cc_node(graph, i);
            count++;
        }
    }

    cc /= (double) count;

    return cc;
}

double cc_node(Graph* graph, long node){
    double cc = 0.0;
    double avg = 0.0;
    double possible_pairs = 0;
    long count = 0;

    if(graph->nodes[node].degree > 1) {
        possible_pairs = ((double)(graph->nodes[node].degree * (graph->nodes[node].degree - 1)))/2.0;
        cc = ((double) graph->nodes[node].number_triangles / possible_pairs);
    }

    return cc;
}