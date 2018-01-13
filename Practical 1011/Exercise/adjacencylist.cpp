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

bool graph_add_edge(Graph* graph, long node, long neighbour){
    //Do not add if selp loop or existing edge
    if(graph_is_self_loop(node, neighbour)) {
        return false;
    }

    if(graph_contains_edge(graph, node, neighbour)){
        return false;
    }

    int i=0;
    //TODO: change using binary search
    for (i = 0; i < graph->nodes[node].degree && graph->nodes[node].neighbours[i] <= neighbour; ++i);

    graph->nodes[node].neighbours.insert(graph->nodes[node].neighbours.begin()+i, neighbour);
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
        cout << node.id << ": ";
        for(int j=0;j<node.degree;j++){
            cout << node.neighbours.at(j) << " ";
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
    graph->nodes[src].degree--;

    return true;
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

        ////cout << "Node " << node << " neighbor " << neighbor << endl;
        ////cout << "Node start: " << start_nodeV << " end: " << end_nodeV << endl;
        ////cout << "Neigh start: " << start_nodeU << " end: " << end_nodeU << endl;

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
        cout << graph->nodes[node].neighbours[i] << " ";
    }
    cout << endl;
}


void swap_vector(vector<long>* array, long index1, long index2){
    long tmp = array->at(index1);
    array->at(index1) = array->at(index2);
    array->at(index2) = tmp;
}


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

        }
    }

    //cout << endl << "Obtained diamater: " << diameter << endl;
    //cout << "Maximum size: "<<max_size<<endl;

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
            long neighbour = graph->nodes[node].neighbours[i];

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