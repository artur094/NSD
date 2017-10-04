//
// Created by Ivan Morandi on 26/09/2017.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist.h"
#include "queue.h"

using namespace std;

long diameter(Graph* graph);
long* cc(Graph* graph);
long* BFS(Graph* graph, long root);
void BFS_cc(Graph* graph, long root, long* ids, long id, long &ids_length);
void BFS_cc_diameter(Graph* graph, long root, long* ids, long id, long &ids_length, long &max_distance, long &last_node);


int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");


    diameter(graph);
    //cc(graph);
    //BFS(graph, 0);

    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
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
            cout << "CC of "<<i+graph->offset<<" with size "<<size<<" where the max distance is "<<max_distance<<" in node "<<last_node+graph->offset<<endl;

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

    cout << endl << "Obtained diamater: " << diameter << endl;
    cout << "Maximum size: "<<max_size<<endl;

    return diameter;
}

/**
 * Return an array which contains the Connected Components ID of each node (in which connected component is the node)
 * @param graph
 * @return
 */
long* cc(Graph* graph){
    if(graph == NULL)
        return NULL;
    if(graph->number_nodes <= 0)
        return NULL;

    long* ids = new long[graph->number_nodes];
    memset(ids, -1, sizeof(long)*graph->number_nodes);
    long id = 0;

    for(int i=0;i<graph->number_nodes;i++){
        //If ID is not assigned to the node I, then run BFS
        long size = 0;
        long max_distance = 0;
        long last_node = i;
        long root = i;
        long tmp_max_distance = 0;

        if(ids[i] < 0){
            BFS_cc(graph, root, ids, id, size);
            cout << "CC of "<<i+graph->offset<<" with size "<<size<<endl;
            id++;
        }
    }
    return ids;
}

long* BFS(Graph* graph, long root){
    if(graph == NULL)
        return NULL;
    if(graph->number_nodes <= 0)
        return NULL;

    Queue* queue = queue_init();
    long* parents = new long[graph->number_nodes];
    bool* visited = new bool[graph->number_nodes];

    memset(parents, -1, sizeof(long)*graph->number_nodes);
    memset(visited, false, sizeof(bool)*graph->number_nodes);

    queue_push(queue, node_init(root));
    visited[root] = true;

    while(! queue_is_empty(queue)){

        long node = queue_pop_val(queue);

        for (int i = 0; i < graph->graph_degree[node]; ++i) {
            long neighbour = graph->neighbours[graph->graph[node] + i];

            if(!visited[neighbour] && neighbour >= 0){
                queue_push(queue, node_init(neighbour));
                visited[neighbour] = true;
                parents[neighbour] = node;
            }
        }
    }

    queue_deinit(queue);
    delete[] visited;

    return parents;
}

void BFS_cc(Graph* graph, long root, long* ids, long id, long &ids_length){
    if(graph == NULL)
        return;
    if(graph->number_nodes <= 0)
        return;
    if(ids == NULL)
        return;

    Queue* queue = queue_init();
    bool* visited = new bool[graph->number_nodes];

    memset(visited, false, sizeof(bool)*graph->number_nodes);

    queue_push(queue, node_init(root));
    visited[root] = true;
    ids[root] = id;
    ids_length = 1;

    while(! queue_is_empty(queue)){
        long node = queue_pop_val(queue);

        for (int i = 0; i < graph->graph_degree[node]; ++i) {
            long neighbour = graph->neighbours[graph->graph[node] + i];

            if(!visited[neighbour] && neighbour >= 0){
                queue_push(queue, node_init(neighbour));
                visited[neighbour] = true;
                ids[neighbour] = id;
                ids_length++;
            }
        }
    }

    queue_deinit(queue);
    delete[] visited;
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

    queue_push(queue, node_init(root));
    visited[root] = true;
    ids[root] = id;
    ids_length = 1;
    last_node = root;
    max_distance = 0;

    while(! queue_is_empty(queue)){
        long node = queue_pop_val(queue);

        for (int i = 0; i < graph->graph_degree[node]; ++i) {
            long neighbour = graph->neighbours[graph->graph[node] + i];

            if(!visited[neighbour] && neighbour >= 0){
                queue_push(queue, node_init(neighbour));
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
}

