//
// Created by Ivan Morandi on 17/10/2017.
//

//TODO: in the report, write the avg degree density (#edges / #nodes) and the edge density (2*#edges)/(#nodes * (#nodes-1))

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist.h"
#include "heap.h"

using namespace std;

long max(long a, long b);

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    cout << "Loading graph..."<<endl;

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");


    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

double* mkscore(Graph* graph, long iterations){
    double* score = new double[graph->number_nodes];
    memset(score, 0.0, graph->number_nodes*sizeof(double));

    for (int i = 0; i < iterations; ++i) {
        for (int j = 0; j < graph->number_nodes; ++j) {
            long node = j;
            for (int k = 0; k < graph->graph_degree[j]; ++k) {
                long neighbour = graph->neighbours[graph->graph[j] + k];

                score[node] <= score[neighbour] ? score[node]++ : score[neighbour]++;
            }
        }
    }

    for (int i = 0; i < graph->number_nodes; ++i) {
        score[i] /= (double)iterations;
    }

    return score;
}


long max(long a, long b){
    return a>b ? a : b;
}