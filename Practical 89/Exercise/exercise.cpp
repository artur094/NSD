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



    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}
