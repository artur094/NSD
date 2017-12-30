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

int main(int argc, char** argv) {
    Graph *graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);
    long iterations = 100;

    cout << "Loading graph..." << endl;

    if (argc > 2)
        iterations = atol(argv[2]);

    if (argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    cout << "Graph loaded!" << endl;
    // graph_print(graph);
    cout << "Starting JCB Algorithm..."<<endl;
    jackcomesback(graph, 3);
    cout << "JCK Algorithm done!" << endl;

    cout << "Storing communities into the file \'communities.txt\'" << endl;
    graph_communities_to_file(graph, "communities.txt");

    cout << "Done!" << endl;

    cout << "Deinit graph..." << endl;
    graph_deinit(graph);
    cout << "Done! Closing the programme" << endl;

}
