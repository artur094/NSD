//
// Created by Ivan Morandi on 12/10/2017.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include "adjacencylist.h"
#include "queue.h"

using namespace std;

Graph* build_random_graph(long n, long m);

int main(int argc, char** argv){
    if(argc < 3){
        cout << "Error, wrong number of arguments" << endl;
        cout << "Usage: ./<name_exe> <number_nodes> <number_edges>" << endl;
        return 1;
    }

    srand(time(NULL));


    cout << "NSD Course:" << endl;
    cout << "Practical 4, Exercise 1" << endl << endl;

    long start = time(NULL);
    long end = time(NULL);
    long diff = 0;
    long number_nodes = atol(argv[1]);
    long number_edges = atol(argv[2]);

    Graph* graph;

    cout << "Building graph..."<<endl;
    graph = build_random_graph(number_nodes, number_edges);

    end = time(NULL);
    cout<<"\tDone, required "<<(end-start)-diff<<" seconds. Total: "<<(end-start)<<endl;
    diff = end-start;

    //graph_print(graph);

    cout << endl << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl << endl;

    cout << "Calculating diameter and cc..."<<endl;
    diameter(graph);

    end = time(NULL);
    cout<<"\tDone, required "<<(end-start)-diff<<" seconds. Total: "<<(end-start)<<endl;
    diff = end-start;

    cout << endl << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl << endl;

    cout << "Calculating number of triangles..."<<endl;
    long number_trianglesX3 = graph_compute_triangles(graph);

    end = time(NULL);
    cout<<"\tDone, required "<<(end-start)-diff<<" seconds. Total: "<<(end-start)<<endl;
    diff = end-start;

    cout << endl;
    cout << "Number of triangles: " << number_trianglesX3 / 3 << endl;
    cout << "Cluster Coefficient: " << cc(graph) << endl;
    cout << "Transitive Ratio: " << tr(graph) << endl;

    cout << endl << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl << endl;

    cout << "Storing degrees in a file..."<<endl;
    graph_store_degree_file("degree.dat", graph);

    end = time(NULL);
    cout<<"\tDone, required "<<(end-start)-diff<<" seconds. Total: "<<(end-start)<<endl;
    diff = end-start;

    cout << "Storing graph in a file..."<<endl;
    graph_store_file("graph.txt", graph);

    end = time(NULL);
    cout<<"\tDone, required "<<(end-start)-diff<<" seconds. Total: "<<(end-start)<<endl;
    diff = end-start;

    graph_deinit(graph);


    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

Graph* build_random_graph(long n, long m){
    Graph* graph = graph_init(n, 0);
    long nodeA;
    long nodeB;

    for (int i = 0; i < m; ) {
        nodeA = rand()%n;
        nodeB = rand()%n;

        if(graph_add_edge(graph, nodeA, nodeB)){
            graph_add_edge(graph, nodeB, nodeA);
            i++;
        }
    }

    return graph;
}

