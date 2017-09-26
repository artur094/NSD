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

long* BFS(Graph* graph, long root);

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");


    BFS(graph, 0);

    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

long* BFS(Graph* graph, long root){
    Queue* queue = queue_init();
    long* parents = new long[graph->number_nodes];
    bool* visited = new bool[graph->number_nodes];

    memset(parents, -1, sizeof(long)*graph->number_nodes);
    memset(visited, false, sizeof(bool)*graph->number_nodes);

    queue_push(queue, node_init(root));
    visited[root] = true;

    while(! queue_is_empty(queue)){
        long node = queue_pop_val(queue);

        cout << "Visiting: "<<node+graph->offset<<" with degree "<<graph->graph_degree[node]<<endl;

        for (int i = 0; i < graph->graph_degree[node]; ++i) {
            long neighbour = graph->neighbours[graph->graph[node] + i];

            if(!visited[neighbour] && neighbour >= 0){
                //cout << "Adding neighbour: "<<neighbour + graph->offset<<endl;
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
