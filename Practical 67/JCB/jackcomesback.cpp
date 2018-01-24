#include <vector>
#include <utility>
#include <cstdlib>
#include "jackcomesback.h"
#include "adjacencylist.h"
#include "community.h"
#include "community_functions.h"
#include "heap.h"


using namespace std;

//start from nodes with slowest degree
void jackcomesback(Graph* graph, string filename, long iterations, double alpha){
    //Initialization
    cout << "[JCB] Initializing the environment" << endl;
    page_rank(graph, alpha, iterations);
    graph_compute_triangles(graph);
    graph_fix_communities(graph);

    Heap* heap = heap_init(graph);
    heap_restore(heap);

    bool merged = true;
    long community;
    long jcb_it = 0;

    //Start to iterate
    while(merged){
        merged = false;
        cout << "[JCB] Iteration " << jcb_it++ << endl;
        while ((community = heap_pointer_next(heap)) >= 0){
            if(graph->nodes[community].community->ID != community)
                continue;
            if(graph->nodes[community].community->number_neighbour <= 0)
                continue;

            cout << "[JCB]: Checking community " << community << endl;

            if(jackcomesback_iteration(graph, heap, community))
                merged = true;
        }
        cout << "[JCB]: Fixing heap" << endl;
        heap_fix(heap, graph);
        //heap_print(heap);
    }

    jackcomesback_communities_to_file(filename, graph);
}

//True if there is a merge
//False otherwise
bool jackcomesback_iteration(Graph* graph, Heap* heap, long node){
    //recompute 1 iteration of pagerank only on this node O(n)
    jackcomesback_fix_pr(graph, node);

    //find the best neighbour using the function jackcomesback_best_neighbour
    long best_neighbour = jackcomesback_best_neighbour(graph, node);

    if(best_neighbour < 0){
        //no best neighbour
        //cout << "[JCB-IT] Not merged" << endl;
        return false;
    }
    if(graph->nodes[best_neighbour].community->ID == graph->nodes[node].community->ID){
        //cout << "[ERROR] Merging the same community" << endl;
        return false;
    }

    //cout << "[JCB-IT] Merging " << node << " " << best_neighbour << endl;
    graph_merge(graph, node, best_neighbour);

    return true;
}

// get PR of each neighbour and multiply it for the weight of links between communities and multiply it by:
// the ratio of intersection over total degree if merged
// -1 if can't be merged
long jackcomesback_best_neighbour(Graph* graph, long node){
    //PR and weights are already computed
    Community* community = graph->nodes[node].community;

    long* intersections = new long[community->number_neighbour];
    long* triangles_to_comm = new long[community->number_neighbour];

    long final_comm = -1;

    double max = 0.0;
    long index_max = 0;
    double val = 0.0;

    for (int i = 0; i < community->number_neighbour; ++i) {
        triangles_to_comm[i] = community_get_numbers_triangle_to_comm(graph, community, graph->nodes[community->neighbours[i]].community);
        intersections[i] = community_get_intersections(community, graph->nodes[community->neighbours[i]].community);
        val = community_get_merge_value(community,graph->nodes[community->neighbours[i]].community, intersections[i], triangles_to_comm[i]);

        if(val > max){
            max = val;
            index_max = i;
        }
    }

    //cout << "[JCB-BN]: Checking " << node << " with " << graph->nodes[community->neighbours[index_max]].id << endl;

    if(community_quality_function(community,graph->nodes[community->neighbours[index_max]].community, intersections[index_max], triangles_to_comm[index_max]) &&
            community_quality_function(graph->nodes[community->neighbours[index_max]].community, community, intersections[index_max], triangles_to_comm[index_max]))
        final_comm = community->neighbours[index_max];

    delete [] intersections;
    delete [] triangles_to_comm;

    return final_comm;
}

void jackcomesback_fix_pr(Graph* graph, long node){
    if(graph == NULL)
        return;

    Community* comm = graph->nodes[node].community;

    comm->pr_value = ALPHA * (1.0/(double)graph->number_nodes);

    double prob_from_neighbours = 0;

    for (int i = 0; i < comm->number_neighbour; ++i) {
        long neighbour = comm->neighbours[i];

        if(comm->ID != graph->nodes[neighbour].community->ID)
            prob_from_neighbours += graph->nodes[neighbour].community->pr_value * (double)comm->neighbours_weight[i]/((double)graph->nodes[neighbour].community->outEdges);
    }

    comm->pr_value += (1.0-ALPHA) * prob_from_neighbours;
}

void jackcomesback_communities_to_file(string name, Graph* graph){
    if(graph == NULL)
        return;

    fstream out;
    out.open(name, ios::out);

    cout << "[JCB-FILE]: Storing file with communities..." << endl;

    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->nodes[i].community->ID != i)
            continue;

        vector<long> copy = graph->nodes[i].community->nodes;
        sort(copy.begin(), copy.begin()+copy.size());

        //size
        cout << "Size = " << copy.size() << " with nodes: ";
        out << copy.size() << " ";

        //nodes
        for (int j = 0; j < copy.size(); ++j) {
            cout << copy[j] << ", ";
            out << copy[j] << " ";
        }
        cout << endl;
        out << endl;
    }

    out.close();
}