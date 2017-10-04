//
// Created by Ivan Morandi on 26/09/2017.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist.h"
#include "priority_queue.h"

using namespace std;

long* Dijkstra(Graph* graph, long root);

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    Dijkstra(graph, 0);

    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

long* Dijkstra(Graph* graph, long root){
    if(graph == NULL)
        return NULL;
    if(graph->number_nodes <= 0)
        return NULL;

    PriorityQueue* queue = priority_queue_init();
    long* parents = new long[graph->number_nodes];
    int* distance = new int[graph->number_nodes];
    bool* visited = new bool[graph->number_nodes];

    memset(parents, -1, sizeof(long)*graph->number_nodes);
    memset(visited, false, sizeof(bool)*graph->number_nodes);
    memset(distance, -1, sizeof(int)*graph->number_nodes);

    priority_queue_push(queue, root, 0);

    visited[root] = true;
    distance[root] = 0;

    while(! priority_queue_is_empty(queue)){

        long node = priority_queue_pop(queue);


        cout << node + graph->offset << " ";

        //For each neighbor
        for (int i = 0; i < graph->graph_degree[node]; ++i) {
            long neighbour = graph->neighbours[graph->graph[node] + i];
            long weight = graph->neighbours_weight[graph->graph[node]+i];

            //If not visited
            if(!visited[neighbour] && neighbour >= 0){

                distance[neighbour] = distance[node] + weight;

                priority_queue_push(queue, neighbour, distance[neighbour]);

                visited[neighbour] = true;
                parents[neighbour] = node;

            }//Will never re-introduce an already visited graph (since the visited ones have the shortest distance).
                // Therefore, it will only change the priorities of nodes inside the queue
            else if(visited[neighbour] && distance[neighbour] > (distance[node]+weight)){
                distance[neighbour] = distance[node] + weight;
                parents[neighbour] = node;
                priority_queue_update(queue, neighbour, distance[neighbour]);
            }
        }
    }

    priority_queue_deinit(queue);
    delete[] visited;

    cout << endl;

    return parents;
}
