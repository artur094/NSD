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

Graph* switch_edges_graph(long* edges, long iterations, long length, long n);
void fix_duplicates_graph(Graph* graph, long* edges, long m);
void store_array(char* filename, double* array, long length, long iteration_step);
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
    long *edges;
    char* file_name = argv[1];
    
    int iterations_step = 1000;
    int total_iterations = 0;
    int cluster_array_length = 500;
    double* cluster_array = new double[cluster_array_length];
    

    Graph* graph;

    cout << "Loading graph..."<<endl;
    graph = graph_load_file(file_name);
    n = graph->number_nodes;
    m = graph->number_edges;

    cout << "Getting the degree array..."<<endl;
    edges = graph_edges_array(graph);

    cout << "Deinitializing first graph..."<<endl;
    graph_deinit(graph);

    cout << "Start iterations ... " << endl;
    long* copy = new long[m];

    for (int i = 0; i < cluster_array_length; ++i) {
        total_iterations += iterations_step;

        cout << "Iteration #"<<i<<endl;


        cout << "Copying the array in the tmp... " << endl;
        memcpy(copy, edges, m*sizeof(long));

        cout << "Switching edges of graph..."<<endl;
        graph = switch_edges_graph(edges, total_iterations, m, n);

        end = time(NULL);
        cout<<"\tDone, required "<<(end-start)-diff<<" seconds. Total: "<<(end-start)<<endl;
        diff = end-start;

        cout << "Calculating number of triangles..."<<endl;
        long number_trianglesX3 = graph_compute_triangles(graph);

        end = time(NULL);
        cout<<"\tDone, required "<<(end-start)-diff<<" seconds. Total: "<<(end-start)<<endl;
        diff = end-start;

        cluster_array[i] = cc(graph);

        cout << "Cluster Coefficient: " << cluster_array[i] << endl;

        if(i < cluster_array_length-1){
            cout << "Deinit Graph..." << endl;
            graph_deinit(graph);
        }

        //graph_print(graph);
    }
    delete[](copy);
    delete[](edges);

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

    cout << "Storing iterations results in a file..."<<endl;
    store_array("iterations.dat", cluster_array, cluster_array_length, iterations_step);

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

void store_array(char* filename, double* array, long length, long iteration_step){
    fstream out;
    out.open(filename, ios::out);

    for (int i = 0; i < length; ++i) {
        out<<(i+1)*iteration_step<<" "<<array[i]<<endl;
    }

    out.close();
}

Graph* switch_edges_graph(long* edges, long iterations, long length, long n){

    Graph* graph = graph_init(n, 0);
    long u, u_index;
    long v, v_index;

    while(iterations>0){
        u_index = rand()%(length);
        v_index = rand()%(length-1);

        u = edges[u_index];
        v = edges[v_index];

        //cout << "Considering " << u << " " << v << " with i = " << i << endl;

        if(u != v){
            swap(edges[u_index], edges[v_index]);
            iterations--;
        }
    }

    long j = length-1;

    for (int i = length-1; i > 0 ; i-=2) {
        u = edges[i];
        v = edges[i-1];

        if(graph_add_edge(graph, u, v)){
            graph_add_edge(graph, v, u);

            swap(edges[i], edges[j--]);
            swap(edges[i-1], edges[j--]);
        }
    }

    fix_duplicates_graph(graph, edges, j+1);


    return graph;
}

void fix_duplicates_graph(Graph* graph, long* edges, long m){
    long i = m;
    long u, u_index;
    long v, v_index;
    int counter = 0;

    while(i>0 && counter < 10000){
        u_index = rand()%(i);
        if(i-2 > 0)
            v_index = rand()%(i-1);
        else
            v_index = 0;

        u = edges[u_index];
        v = edges[v_index];

        //cout << "Considering " << u << " " << v << " with i = " << i << endl;

        if(graph_add_edge(graph, u, v)){
            graph_add_edge(graph, v, u);
            swap(edges[u_index], edges[i-1]);
            swap(edges[v_index], edges[i-2]);
            i-=2;
            //cout << "i = " << i << endl;
            counter = 0;
        } else
            counter++;
    }
}

void swap(long& a, long& b){
    long tmp = a;
    a = b;
    b = tmp;
}

