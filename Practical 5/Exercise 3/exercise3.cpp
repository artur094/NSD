//
// Created by Ivan Morandi on 17/10/2017.
//

//TODO: in the report, write the avg degree density (#edges / #nodes) and the edge density (2*#edges)/(#nodes * (#nodes-1))

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <math.h>
#include "adjacencylist.h"
#include "heap.h"

using namespace std;

void compute_densest_subgraph(double* scores, int iterations, long length);
void iteratire_sort(Graph* graph, double* score, long* indexes, long length);
long max(long a, long b);
double* mkscore(Graph* graph, long iterations);

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);
    long iterations = 100;

    cout << "Loading graph..."<<endl;

    if(argc > 2)
        iterations = atol(argv[2]);

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    cout << "Computing score..." << endl;
    double* score = mkscore(graph, iterations);

    cout << "Initializing an array for indexes..." << endl;
    long* indexes = new long[graph->number_nodes];
    for (int i = 0; i < graph->number_nodes; ++i) {
        indexes[i] = i;
    }

    //for (int i = 0; i < graph->number_nodes; ++i) {
    //   cout << "Score of node " << i + graph->offset << " = " << score[i] << endl;
    //}

    cout << "Sorting scores..." << endl;
    iteratire_sort(graph, score, indexes, graph->number_nodes);

    //for (int i = 0; i < graph->number_nodes; ++i) {
    //    cout << "Node " << i + graph->offset << " has score " << score[indexes[i]] << endl;
    //}

    cout << "Computing dense subgraph..." << endl << endl;
    compute_densest_subgraph(score, iterations, graph->number_nodes);

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
                long neighbour = graph->neighbours[graph->graph[node] + k];
                if(node < neighbour) {
                    score[node] <= score[neighbour] ? score[node]++ : score[neighbour]++;
                }
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

//TODO: change to O(n * logn)
void iteratire_sort(Graph* graph, double* score, long* indexes, long length){
    long max;

    long* tmp_indexes = new long[graph->number_nodes];
    for (int i = 0; i < graph->number_nodes; ++i) {
        tmp_indexes[i] = i;
    }

    for (int i = 0; i < length; ++i) {
        max = i;
        for (int j = i+1; j < length; ++j) {
            if(score[max] < score[j]){
                max = j;
            }
        }

        if(max != i){
            double tmp = score[i];
            score[i] = score[max];
            score[max] = tmp;

            long tmp_index = tmp_indexes[i];
            tmp_indexes[i] = tmp_indexes[max];
            tmp_indexes[max] = tmp_index;
        }
    }

    for (int i = 0; i < graph->number_nodes; ++i) {
        indexes[tmp_indexes[i]] = i;
    }

    delete [](tmp_indexes);
}

void compute_densest_subgraph(double* scores, int iterations, long length){
    long count = 0;
    long size = 0;
    double degree_density = 0.0;
    double edge_density = 0.0;
    long max_size = 0;
    double max_degree_density = 0.0;
    double max_edge_density = 0.0;
    double avg_degree_density = 0.0;
    double avg_edge_density = 0.0;

    for (int i = 0; i < length; ++i) {
        if(i == 0){
            size++;
            continue;
        }

        if(scores[i-1]-scores[i] <= 1.0/(double)iterations) {
            size++;
        }else{
            count ++;
            long edges = scores[i-1] * size;

            degree_density = (double)edges/(double)size;
            edge_density = 2.0 * (double)edges / (double)(size * (size-1));

            //cout << "Size of subgraph " << count << " = " << size << endl;
            //cout << "\tDegree Density = " << degree_density << endl;
            //cout << "\tEdge Density = " << edge_density << endl;

            avg_degree_density += degree_density;
            avg_edge_density += edge_density;

            if(degree_density > max_degree_density) {
                max_size = size;
                max_degree_density = degree_density;
                max_edge_density = edge_density;
            }

            size = 1;
        }
    }

    count ++;
    long edges = scores[length-1] * size;

    degree_density = (double)edges/(double)size;
    edge_density = 2.0 * (double)edges / (double)(size * (size-1));

    //cout << "Size of subgraph " << count << " = " << size << endl;
    //cout << "\tDegree Density = " << degree_density << endl;
    //cout << "\tEdge Density = " << edge_density << endl;

    avg_degree_density += degree_density;
    avg_edge_density += edge_density;

    if(degree_density > max_degree_density) {
        max_size = size;
        max_degree_density = degree_density;
        max_edge_density = edge_density;
    }

    avg_degree_density /= count;
    avg_edge_density /= count;

    cout << "Size of Maximum subgraph " << count << " = " << max_size << endl;
    cout << "\tDegree Density = " << max_degree_density << endl;
    cout << "\tEdge Density = " << max_edge_density << endl;
    cout << "Avg degree density = " << avg_degree_density << endl;
    cout << "Avg edge density = " << avg_edge_density << endl;

}

