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

Graph* build_scale_free_graph(long n, long m);
Graph* build_path_graph(long n, long m);

int main(int argc, char** argv){
    if(argc < 3){
        cout << "Error, wrong number of arguments" << endl;
        cout << "Usage: ./<name_exe> <number_nodes> <number_edges>" << endl;
        return 1;
    }

    srand(time(NULL));


    cout << "NSD Course:" << endl;
    cout << "Practical 4, Exercise 2" << endl << endl;

    long start = time(NULL);
    long end = time(NULL);
    long diff = 0;
    long number_nodes = atol(argv[1]);
    long number_edges = atol(argv[2]);

    Graph* graph;

    cout << "Building graph..."<<endl;
    graph = build_scale_free_graph(number_nodes, number_edges);

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

Graph* build_scale_free_graph(long n, long m){
    long alpha = m / n;

    Graph* graph = build_path_graph(n, alpha);

    long *P = new long[2*m];
    long P_length = 0;

    for (int i = 0; i < graph->number_nodes; ++i) {
        for (int j = 0; j < graph->nodes[i].degree; ++j) {
            P[P_length++] = i;
        }
    }

    for (long i = alpha; i < n; ++i) {
        long k=0;
        while (k<alpha){
            long neighbor = rand()%P_length;

            if(graph_add_edge(graph, i, P[neighbor])) {
                graph_add_edge(graph, P[neighbor], i);
                k++;
                P[P_length++] = i;
                P[P_length++] = P[neighbor];
            }
        }
    }

    return graph;
}

Graph* build_path_graph(long n, long m){
    Graph* graph = graph_init(n, 0);
    long nodeA;
    long nodeB;

    for (int i = 0; i < m-1; i++) {
        //cout << "Adding " << i << " " << i+1 << endl;
        graph_add_edge(graph, i, i+1);
        graph_add_edge(graph, i+1, i);
    }

    return graph;
}
