//
// Created by Ivan Morandi on 24/12/2017.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <math.h>
#include "adjacencylist.h"
#include "community.h"
#include "jackcomesback.h"

#define FILENAME "jcb.txt"

using namespace std;

//started at 17.51 with amazon graph

int main(int argc, char** argv) {
    Graph *graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    long nr_comm = 3;
    int pagerank_iterations = 50;
    double alpha = ALPHA;

    cout << "Loading graph..." << endl;

    if (argc > 4)
        alpha = atof(argv[4]);

    if(alpha > 1)
        alpha = 1;
    if(alpha < 0)
        alpha = 0;

    if (argc > 3)
        pagerank_iterations = atoi(argv[3]);

    if(argc > 2)
        nr_comm = atol(argv[2]);

    if (argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    jackcomesback(graph, FILENAME, pagerank_iterations, alpha);

    cout << "Deinit graph..." << endl;
    graph_deinit(graph);
    cout << "Done! Closing the programme" << endl;

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;

}
