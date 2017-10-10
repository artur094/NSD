#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <vector>
#include "directed_adjacencylist.h"

using namespace std;

void save_matrix(char* filename, Graph* graph, double** matrix, long* matrix_nodes, int nrows, int ncols);
void print_matrix(Graph* graph, double** matrix, long* matrix_nodes, int nrows, int ncols);

void print_array(double *P, int length);

double* page_rank(Graph* graph, double alpha, int iterations, long* matrix_nodes, double** matrix_convergence, int matrix_length);
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

    int number_nodes = 12;

    long matrix_nodes[] = {907722, 2421026, 4778009, 7196384, 10329063, 305, 6104545, 2031512, 6622645, 8285726, 3434750, 632};

    double** matrix_convergence = new double*[number_nodes];

    for (int i = 0; i < number_nodes; ++i) {
        matrix_convergence[i] = new double[iteration];
        matrix_nodes[i] -= graph->offset;
    }

    double* P = page_rank(graph, alpha, iteration, matrix_nodes, matrix_convergence, number_nodes);

    save_matrix("output.dat", graph, matrix_convergence, matrix_nodes, number_nodes, iteration);

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

double* page_rank(Graph* graph, double alpha, int iterations, long* matrix_nodes, double** matrix_convergence, int matrix_length){
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

        for (int l = 0; l < matrix_length; ++l) {
            matrix_convergence[l][k] = P[matrix_nodes[l]];
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

void print_matrix(Graph* graph, double** matrix, long* matrix_nodes, int nrows, int ncols){
    for (int i = 0; i < nrows; ++i) {
        cout << matrix_nodes[i] + graph->offset <<": ";
        for (int j = 0; j < ncols; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void save_matrix(char* filename, Graph* graph, double** matrix, long* matrix_nodes, int nrows, int ncols){
    ofstream output;
    output.open(filename, ios::out);

    for (int i = 0; i < ncols; ++i) {
        //output << matrix_nodes[i] + graph->offset <<": ";
        output << i+1 << " ";
        for (int j = 0; j < nrows; ++j) {
            output << matrix[j][i] << " ";
        }
        output << endl;
    }

    output.close();
}

void write_file_corr1(char* filename, Graph* graph, double* P){
    for (int i = 0; i < graph->number_nodes; ++i) {

    }
}