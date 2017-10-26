//
// Created by Ivan Morandi on 17/10/2017.
//

//TODO: in the report, write the avg degree density (#edges / #nodes) and the edge density (2*#edges)/(#nodes * (#nodes-1))

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist.h"
#include "heap.h"

using namespace std;

double compute_degree_density(long n, long m);
double compute_edge_density(long n, long m);


long* core_decomposition(Graph* graph);
long* densest_prefix(Graph* graph, long* core_decomposition);
long max(long a, long b);

// avg degree density = #edges_subgraph / #node_subgraph
// avg edge density = 2*#edges_subgraph / ( #node_subgraph * (#node_subgraph -1) )

// print size, avg degree density and avg edge density of the maximum

int main(int argc, char** argv) {
    Graph* graph;

    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    cout << "Loading graph..."<<endl;

    if(argc > 1)
        graph = graph_load_file(argv[1]);
    else
        graph = graph_load_file("graph3.txt");

    cout << "Computing core decomposition..."<<endl;
    long *core_dec = core_decomposition(graph);
    cout << "Computing prefix..." << endl;
    long *dens_prefixs = densest_prefix(graph, core_dec);

    long max_densest_graph = 0;
    double max_degree_density = 0.0;
    double max_edge_density = 0.0;
    double avg_degree_density = 0.0;
    double avg_edge_density = 0.0;

    cout << "Computing densities..." << endl;
    for (int i = 0; i < graph->number_nodes; ++i) {
        long subgraph_nodes = i+1;
        double degree_density =  compute_degree_density(subgraph_nodes, dens_prefixs[i]);
        double edge_density = compute_edge_density(subgraph_nodes, dens_prefixs[i]);
        //cout << "Dense subgraph of size = " << subgraph_nodes << " nodes" << endl;
        //cout << "\t Degree Density = " << degree_density << endl;
        //cout << "\t Edge Density = " << edge_density << endl;

        avg_degree_density += degree_density;
        avg_edge_density += edge_density;

        if(max_degree_density < degree_density){
            max_degree_density = degree_density;
            max_densest_graph = i;
            max_edge_density = edge_density;
        }
    }

    avg_degree_density /= graph->number_nodes;
    avg_edge_density /= graph->number_nodes;

    cout << endl;
    cout << "Maximum size = " << max_densest_graph+1 << endl;
    cout << "\twith degree density = " << max_degree_density << endl;
    cout << "\twith edge density = " << max_edge_density << endl;
    cout << "Avg degree density = " << avg_degree_density << endl;
    cout << "Avg edge density = " << avg_edge_density << endl;

    graph_deinit(graph);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

double compute_degree_density(long n, long m){
    return (double)m/(double)n;
}

double compute_edge_density(long n, long m){
    if(n > 1)
        return (2.0 * (double) m) / (n * (n-1));
    return 0.0;
}

long* core_decomposition(Graph* graph){
    Heap* heap = heap_init(graph);
    heap_restore(heap);

    long* cd = new long[graph->number_nodes];

    long n = graph->number_nodes-1;
    long c = 0;

    while(! heap_is_empty(heap)){
        long v = heap->heap[0].node;
        //cout << "Removing " << v + heap->graph->offset << " ..." <<endl;
        c = max(c, heap->heap[0].degree);

        //heap_print(heap);
        //cout << "After " << endl;

        heap_remove_node(heap, v);
        cd[v] = n;

        //heap_print(heap);

        //cout << v+graph->offset << " with c= " << c << " in " << cd[v] << endl;
        n--;
    }

    heap_deinit(heap);

    return cd;
}

long* densest_prefix(Graph* graph, long* core_decomposition){
    long* distributed = new long[graph->number_nodes];
    long* cumulative = new long[graph->number_nodes];

    memset(distributed, 0, sizeof(long)*graph->number_nodes);
    memset(cumulative, 0, sizeof(long)*graph->number_nodes);

    for (int i = 0; i < graph->number_nodes; ++i) {
        for (int j = 0; j < graph->graph_degree[i]; ++j) {
            long neighbour = graph->neighbours[graph->graph[i] + j];

            if(i < neighbour){
                //cout << "Studying "<<i+graph->offset<<" adding neighbour " << neighbour+graph->offset << " in position " << core_decomposition[neighbour] << " ";

                if(core_decomposition[i] < core_decomposition[neighbour]){
                    distributed[core_decomposition[neighbour]]++;
                }else{
                    distributed[core_decomposition[i]]++;
                }

               // cout << "distributed = " << distributed[core_decomposition[neighbour]] << endl;
            }
        }
    }

    //cout << "0 0" << endl;
    for (int i = 1; i < graph->number_nodes; ++i) {
        cout << i << " " << distributed[i] << endl;
        cumulative[i] = cumulative[i-1] + distributed[i];
    }

    delete[](distributed);

    return cumulative;
}

long max(long a, long b){
    return a>b ? a : b;
}


