//
// Created by Ivan Morandi on 26/09/2017.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist.h"

using namespace std;

double tr(Graph* graph);
double cc(Graph* graph);
double cc_node(Graph* graph, long node);

long count_triangles(Graph* graph);
long count_triangles_between_2_nodes(Graph* graph, long nodeV, long nodeU);
long count_intersections(long* array, long start1, long end1, long start2, long end2);
long index_least_great_element(long* array, long array_length, long start, long value);

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    long number_trianglesX3 = graph_compute_triangles(graph);

    for(int i=0; i<graph->number_nodes; i++){
        cout << "Node ["<<i+graph->offset<<"] with " << graph->graph_triangles[i] << " triangles and CC[" << i+graph->offset << "] = " << cc_node(graph, i) << endl;
    }

    cout << endl;
    cout << "Number of triangles: " << number_trianglesX3 / 3 << endl;
    cout << "Cluster Coefficient: " << cc(graph) << endl;
    cout << "Transitive Ratio: " << tr(graph) << endl;




    /* OLD WAY TO GET NUMBER OF TRIANGLES
    long triangles = count_triangles(graph);
    cout << "Number of triangles: " << triangles << endl;
    */

    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

double tr(Graph* graph){
    double tr = 0.0;

    long count_triengles = 0;
    long count_pairs = 0;

    for(int i=0; i<graph->number_nodes; i++){
        if(graph->graph_degree[i] > 1) {
            count_triengles += graph->graph_triangles[i];
            count_pairs += (graph->graph_degree[i]*(graph->graph_degree[i]-1))/2;
        }
    }

    tr = (double) count_triengles / (double) count_pairs;

    return tr;
}

double cc(Graph* graph){
    double cc = 0.0;
    double avg = 0.0;
    double possible_pairs = 0;
    long count = 0;

    for(int i=0; i<graph->number_nodes; i++){
        if(graph->graph_degree[i] > 1) {
            cc += cc_node(graph, i);
            count++;
        }
    }

    cc /= (double) count;

    return cc;
}

double cc_node(Graph* graph, long node){
    double cc = 0.0;
    double avg = 0.0;
    double possible_pairs = 0;
    long count = 0;

    if(graph->graph_degree[node] > 1) {
        possible_pairs = ((double)(graph->graph_degree[node]*(graph->graph_degree[node] - 1)))/2.0;
        cc = ((double) graph->graph_triangles[node] / possible_pairs);
    }

    return cc;
}


long count_triangles(Graph* graph){
    long count = 0;
    for(int node=0; node<graph->number_nodes; node++){
        for(int j=graph->graph[node]; j < graph->graph[node] + graph->graph_degree[node]; j++){
            long neighbor = graph->neighbours[j];
            if(neighbor > node) {
                count += count_triangles_between_2_nodes(graph, node, neighbor);
            }
        }
    }
    return count;
}

/**
 * Count the number of triangles between 2 nodes V and U, where V < U
 * @param graph
 * @param nodeV node with ID < nodeU
 * @param nodeU node with ID > nodeV
 * @return
 */
long count_triangles_between_2_nodes(Graph* graph, long nodeV, long nodeU){
    if(graph == NULL)
        return 0;
    if(nodeV > nodeU)
        return 0;

    long start_nodeV = index_least_great_element(graph->neighbours, graph->graph_degree[nodeV], graph->graph[nodeV], nodeV);
    long start_nodeU = index_least_great_element(graph->neighbours, graph->graph_degree[nodeU], graph->graph[nodeU], nodeU);
    long end_nodeV = graph->graph[nodeV] + graph->graph_degree[nodeV];
    long end_nodeU = graph->graph[nodeU] + graph->graph_degree[nodeU];

    if(start_nodeU < 0 || start_nodeV < 0)
        return 0;

    return count_intersections(graph->neighbours, start_nodeV, end_nodeV, start_nodeU, end_nodeU);
}

/**
 * Count how many intersections (same elements) there are in the 2 arrays
 * @param array1 Array of neighbors
 * @param start1 Starting point to count the intersections
 * @param end1 Last index to check
 * @param start2 Starting point to count the intersection
 * @param end2 Last index to check
 * @return
 */
long count_intersections(long* array, long start1, long end1, long start2, long end2){
    long i = start1;
    long j = start2;
    long count = 0;

    while( i < end1 && j < end2 ){
        // If equal --> intersection
        if(array[i] == array[j])
        {
            count++;
            i++;
            j++;
        }

        else if(array[i] < array[j])
            i++;

        else if(array[j] < array[i])
            j++;

    }

    return count;
}


long index_least_great_element(long* array, long array_length, long start, long value){
    // Array is already ordered
    for(int i=start;i<array_length+start; i++){
        if(array[i] > value)
            return i;
    }
    return -1;
}