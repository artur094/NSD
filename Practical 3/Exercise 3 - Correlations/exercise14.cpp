#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <vector>
#include "directed_adjacencylist.h"

using namespace std;

void save_pr_pr(char* filename, Graph* graph, double* P1, double* P2);
void save_pr_outdegree(char* filename, Graph* graph, double* P);
void save_pr_indegree(char* filename, Graph* graph, double* P);

void print_array(double *P, int length);

double* page_rank(Graph* graph, double alpha, int iterations);
void write_file(char* name,double* P,long length );
void find_max_min(Graph *graph, double* P, long length, long &max_index, long &min_index);

//TODO: CHANGE GRAPH STRUCTURE TO HAVE LINK IN AND OUT

int main(int argc, char** argv) {
    Graph* graph;
    long iteration = 10;
    double alpha = 0.2;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    cout << "Loading the graph into the data structure"<<endl;

    if(argc > 3)
        iteration = atoi(argv[3]);
    if(argc > 2)
        alpha = atof(argv[2]);

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");


    //graph_print(graph);
    cout << "Done, required " << time(NULL) - start << " seconds" <<endl;

    cout<<"Starting page rank with alpha="<<alpha<<" and #iteration="<<iteration<<endl;

    alpha = 0.15;

    double* P = page_rank(graph, alpha, iteration);

    save_pr_indegree("indegree.dat", graph, P);
    save_pr_outdegree("outdegree.dat", graph, P);

    alpha = 0.1;
    cout<<"Starting page rank with alpha="<<alpha<<" and #iteration="<<iteration<<endl;
    double* P2 = page_rank(graph, alpha, iteration);
    save_pr_pr("pr01.dat", graph, P, P2);
    delete[](P2);

    alpha = 0.2;
    cout<<"Starting page rank with alpha="<<alpha<<" and #iteration="<<iteration<<endl;
    P2 = page_rank(graph, alpha, iteration);
    save_pr_pr("pr02.dat", graph, P, P2);
    delete[](P2);

    alpha = 0.1;
    cout<<"Starting page rank with alpha="<<alpha<<" and #iteration="<<iteration<<endl;
    P2 = page_rank(graph, alpha, iteration);
    save_pr_pr("pr05.dat", graph, P, P2);
    delete[](P2);

    alpha = 0.1;
    cout<<"Starting page rank with alpha="<<alpha<<" and #iteration="<<iteration<<endl;
    P2 = page_rank(graph, alpha, iteration);
    save_pr_pr("pr09.dat", graph, P, P2);
    delete[](P2);


    long max_index = 0;
    long min_index = 0;

    find_max_min(graph, P, graph->number_nodes, max_index, min_index);

    cout << "Node with highest rank: "<<max_index + graph->offset << " with " << P[max_index] << endl;
    cout << "Node with lowest rank: "<<min_index + graph->offset << " with " << P[min_index] <<endl;

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

void find_max_min(Graph* graph, double* P, long length, long &max_index, long &min_index){
    max_index = 0;
    min_index = -1;

    for (int i = 0; i < length; ++i) {
        if(min_index < 0 && ( graph->graph_degree_out[i] > 0 || graph->graph_degree_in[i] > 0))
            min_index = i;
        else{
            if(min_index >= 0 && P[min_index] > P[i] && ( graph->graph_degree_out[i] > 0 || graph->graph_degree_in[i] > 0))
                min_index = i;
        }
        if(P[max_index] < P[i]) {
            max_index = i;
        }
    }
}

void save_pr_indegree(char* filename, Graph* graph, double* P){
    ofstream output;
    output.open(filename, ios::out);

    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->graph_degree_in[i] > 0 || graph->graph_degree_out[i] > 0)
            output << P[i] << " " << graph->graph_degree_in[i] << endl;
    }

    output.close();
}

void save_pr_outdegree(char* filename, Graph* graph, double* P){
    ofstream output;
    output.open(filename, ios::out);

    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->graph_degree_in[i] > 0 || graph->graph_degree_out[i] > 0)
            output << P[i] << " " << graph->graph_degree_out[i] << endl;
    }

    output.close();
}

void save_pr_pr(char* filename, Graph* graph, double* P1, double* P2){
    ofstream output;
    output.open(filename, ios::out);

    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->graph_degree_in[i] > 0 || graph->graph_degree_out[i] > 0)
            output << P1[i] << " " << P2[i] << endl;
    }

    output.close();
}