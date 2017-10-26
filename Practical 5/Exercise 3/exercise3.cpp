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

double compute_degree_density(long n, long m);
double compute_edge_density(long n, long m);

void compute_densest_subgraph(double* scores, int iterations, long length);
void iterative_sort(Graph* graph, double* score, long* indexes, long length);
long max(long a, long b);
double* mkscore(Graph* graph, long iterations);
long* densest_prefix(Graph* graph, long* indexes);

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
    iterative_sort(graph, score, indexes, graph->number_nodes);

    //for (int i = 0; i < graph->number_nodes; ++i) {
    //    cout << "Node " << i + graph->offset << " has score " << score[indexes[i]] << endl;
    //}

    cout << "Computing dense subgraph..." << endl << endl;
    long* dens_prefixs = densest_prefix(graph, indexes);
    //compute_densest_subgraph(score, iterations, graph->number_nodes);

    long max_densest_graph = 0;
    double max_degree_density = 0.0;
    double max_edge_density = 0.0;
    double avg_degree_density = 0.0;
    double avg_edge_density = 0.0;

    cout << "Computing densities..." << endl;
    for (int i = 0; i < graph->number_nodes; ++i) {
        long subgraph_nodes = i+1;
        double degree_density =  compute_degree_density(subgraph_nodes, dens_prefixs[i]);
        double edge_density = compute_edge_density(subgraph_nodes, dens_prefixs[i]);
        //cout << "Dense subgraph of size = " << subgraph_nodes << " nodes" << endl;
        //cout << "\t Degree Density = " << degree_density << endl;
        //cout << "\t Edge Density = " << edge_density << endl;

        avg_degree_density += degree_density;
        avg_edge_density += edge_density;

        if(max_degree_density < degree_density){
            max_degree_density = degree_density;
            max_densest_graph = i;
            max_edge_density = edge_density;
        }
    }

    avg_degree_density /= graph->number_nodes;
    avg_edge_density /= graph->number_nodes;

    cout << endl;
    cout << "Maximum size = " << max_densest_graph+1 << endl;
    cout << "\twith degree density = " << max_degree_density << endl;
    cout << "\twith edge density = " << max_edge_density << endl;
    cout << "Avg degree density = " << avg_degree_density << endl;
    cout << "Avg edge density = " << avg_edge_density << endl;

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
void iterative_sort(Graph* graph, double* score, long* indexes, long length){
    Heap* heap;
    //cout << "Initializing..." << endl;
    heap = heap_init(graph, score);
    //cout << "Sorting..." << endl;
    heap_sort(heap);

    //cout << "Copying..." << endl;
    //cout << "Heap length = " << heap->length << endl;
    for (int i = 0; i < graph->number_nodes ; ++i) {
        //cout << "Node " << heap->heap[i].node + graph->offset << " with score = " << heap->heap[i].score  << endl;
        score[i] = heap->heap[i].score;
        indexes[heap->heap[i].node] = i;
    }

    heap_deinit(heap);
}

long* densest_prefix(Graph* graph, long* indexes){
    long* distributed = new long[graph->number_nodes];
    long* cumulative = new long[graph->number_nodes];

    memset(distributed, 0, sizeof(long)*graph->number_nodes);
    memset(cumulative, 0, sizeof(long)*graph->number_nodes);

    for (int i = 0; i < graph->number_nodes; ++i) {
        for (int j = 0; j < graph->graph_degree[i]; ++j) {
            long neighbour = graph->neighbours[graph->graph[i] + j];

            if(i < neighbour){
                //cout << "Studying "<<i+graph->offset<<" adding neighbour " << neighbour+graph->offset << " in position " << core_decomposition[neighbour] << " ";

                if(indexes[i] < indexes[neighbour]){
                    distributed[indexes[neighbour]]++;
                }else{
                    distributed[indexes[i]]++;
                }

                // cout << "distributed = " << distributed[core_decomposition[neighbour]] << endl;
            }
        }
    }

    //cout << "0 0" << endl;
    for (int i = 1; i < graph->number_nodes; ++i) {
        //cout << i << " " << distributed[i] << endl;
        cumulative[i] = cumulative[i-1] + distributed[i];
    }

    delete[](distributed);

    return cumulative;
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

double compute_degree_density(long n, long m){
    return (double)m/(double)n;
}

double compute_edge_density(long n, long m){
    if(n > 1)
        return (2.0 * (double) m) / (n * (n-1));
    return 0.0;
}