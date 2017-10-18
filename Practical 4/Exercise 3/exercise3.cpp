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

Graph* build_configuration_graph(long* degree, long n, long m);
void swap(long& a, long& b);

int main(int argc, char** argv){
    if(argc < 2){
        cout << "Error, wrong number of arguments" << endl;
        cout << "Usage: ./<name_exe> <graph_name>" << endl;
        return 1;
    }

    srand(time(NULL));


    cout << "NSD Course:" << endl;
    cout << "Practical 4, Exercise 1" << endl << endl;

    long start = time(NULL);
    long end = time(NULL);
    long diff = 0;
    long n, m;
    long *degree;
    char* file_name = argv[1];

    Graph* graph;

    cout << "Loading graph..."<<endl;
    graph = graph_load_file(file_name);
    n = graph->number_nodes;
    m = graph->number_edges;

    cout << "Getting the degree array..."<<endl;
    degree = graph_degree_array(graph);

    cout << "Deinitializing first graph..."<<endl;
    graph_deinit(graph);

    cout << "Building graph using the degree array..."<<endl;
    graph = build_configuration_graph(degree, n, m);

    delete[](degree);

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

    /*cout << "Storing graph in a file..."<<endl;
    graph_store_file("graph.txt", graph);

    end = time(NULL);
    cout<<"\tDone, required "<<(end-start)-diff<<" seconds. Total: "<<(end-start)<<endl;
    diff = end-start;*/

    graph_deinit(graph);


    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

Graph* build_configuration_graph(long* degree, long n, long m){

    Graph* graph = graph_init(n, 0);
    long i = m;
    long u, u_index;
    long v, v_index;

    while(i>0){
        u_index = rand()%(i);
        if(i-2 > 0)
            v_index = rand()%(i-1);
        else
            v_index = 0;

        u = degree[u_index];
        v = degree[v_index];

        //cout << "Considering " << u << " " << v << " with i = " << i << endl;

        if(graph_add_edge(graph, u, v)){
            graph_add_edge(graph, v, u);
            swap(degree[u_index], degree[i-1]);
            swap(degree[v_index], degree[i-2]);
            i-=2;
            cout << "i = " << i << endl;
        }
    }


    return graph;
}

void swap(long& a, long& b){
    long tmp = a;
    a = b;
    b = tmp;
}

