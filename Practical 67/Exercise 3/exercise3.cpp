//
// Created by Ivan Morandi on 24/12/2017.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <math.h>
#include "adjacencylist2.h"
#include "community.h"
#include "directed_adjacencylist.h"
#include "jackcomesback.h"

using namespace std;

//started at 17.51 with amazon graph

int main(int argc, char** argv) {
    Graph *graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    long nr_comm = 3;
    int pagerank_iterations = 50;
    double alpha = 0;

    cout << "Loading graph..." << endl;

    if (argc > 4)
        alpha = atof(argv[3]);

    if(alpha > 1)
        alpha = 1;
    if(alpha < 0)
        alpha = 0;

    if (argc > 3)
        pagerank_iterations = atoi(argv[2]);

    if(argc > 2)
        nr_comm = atol(argv[1]);

    if (argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    cout << "Graph loaded!" << endl;
    // graph_print(graph);
    cout << "Starting JCB Algorithm..."<<endl;
    jackcomesback(graph, nr_comm, pagerank_iterations, alpha);
    cout << "JCK Algorithm done!" << endl;

    cout << "Storing communities into the file \'communities.txt\'" << endl;
    graph_communities_to_file(graph, "communities.txt");

    cout << "Done!" << endl;

    cout << "Deinit graph..." << endl;
    graph_deinit(graph);
    cout << "Done! Closing the programme" << endl;

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;

}
