#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "directed_adjacencylist.h"

using namespace std;

double* page_rank(Graph* graph, double alpha, int iterations);
void write_file(char* name,double* P,long length );
long find_max(double* P, long length);

//TODO: CHANGE GRAPH STRUCTURE TO HAVE LINK IN AND OUT

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    cout << "Loading the graph into the data structure"<<endl;
    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    //graph_print(graph);
    cout << "Done, required " << time(NULL) - start << " seconds" <<endl;


    cout<<"Starting page rank"<<endl;
    double* P = page_rank(graph, 0.2, 10);

    cout << "Node with highest rank: "<<find_max(P, graph->number_nodes) + graph->offset << endl;

    //cout<<"Writing on the file"<<endl;
    //write_file("output.txt", P, graph->number_nodes);
    //cout<<"Done!"<<endl;

    delete[](P);

    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

double* page_rank(Graph* graph, double alpha, int iterations){
    double* P = new double[graph->number_nodes];

    double probability_come_from_neighbour = 0;
    double probability_of_teletrasport = 0;

    //memset(P, 1.0/(double)graph->number_nodes, sizeof(double)*graph->number_nodes);

    for (int i = 0; i < graph->number_nodes; ++i) {
        P[i] = 1.0/(double)graph->number_nodes;
        //cout << "First P["<<i+graph->offset<<"] = " << P[i] << endl;
    }

    for (int k = 0; k < iterations; ++k) {
        for (long i = 0; i < graph->number_nodes; ++i) {
            probability_of_teletrasport = 1/(double)graph->number_nodes;
            probability_come_from_neighbour = 0;

            for (long j = graph->graph_in[i]; j < graph->graph_in[i] + graph->graph_degree_in[i]; ++j) {
                long neighbor = graph->neighbours_in[j];

                //cout << "Studying " << i+graph->offset << " for neighbour "<< neighbor << " with degree " << graph->graph_degree[neighbor] << endl;
                //TODO: CHECK IN LINK, NOT OUT
                //if(graph->graph_degree_out[neighbor] > 0)
                    //TODO: 1/nr_node is with OUT nodes, but I am adding the IN nodes!
                probability_come_from_neighbour += (1.0/(double)graph->graph_degree_out[neighbor]) * P[neighbor];
                //cout << "P["<<i<<"] = " << (1.0-alpha) << " * " << (1.0/(double)graph->graph_degree[neighbor]) << " * " << P[i] << " + " << alpha/(double)graph->number_nodes << endl;
            }
            //cout << "P["<<i+graph->offset<<"] = (1 - " << alpha << " ) * " << probability_come_from_neighbour << " + " << alpha << " * " << probability_of_teletrasport << endl;
            P[i] = (1.0-alpha) * probability_come_from_neighbour + alpha * probability_of_teletrasport;
        }
    }
    return P;
}

void write_file(char* name,double* P,long length ){
    fstream output;
    output.open(name, ios::out);

    for (int i = 0; i < length; ++i) {
        output << i << ": " << P[i] << endl;
    }

    output.close();
}

long find_max(double* P, long length){
    double max = P[0];
    long max_index = 0;

    for (int i = 0; i < length; ++i) {
        if(max < P[i]) {
            max = P[i];
            max_index = i;
        }
    }

    return max_index;
}