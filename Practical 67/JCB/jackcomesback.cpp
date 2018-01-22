//
// Created by Ivan Morandi on 22/12/2017.
//

#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include "adjacencylist.h"
#include "directed_adjacencylist.h"
#include "heap.h"
#include "jackcomesback.h"



void jackcomesback(Graph* graph, long number_communities, int pagerank_iterations, double pagerank_alpha){
    //TODO: FIX REORDERING USING HEAP (#degree * #weight)
    Heap* heap = heap_init(graph);
    heap_restore(heap);
    srand(time(NULL));

    while (!heap_is_empty(heap) && number_communities < graph->number_communities){
        cout << "Number communities = " << graph->number_communities << endl;
        //out << "[JCB]: Getting best community from this heap:" << endl;
        //heap_print(heap);

        long community = heap_first_element(heap);
        //cout << "Best community = " << community << " with following nodes: ";
        //community_print(graph->nodes[community].community);

        //If the community is not connected to anyone else
        if(graph->nodes[community].degree <= 0){
            heap_remove_node(heap, graph, community);
            continue;
        }

        ////cout << "Selected community " << community << " with the following nodes: " ;
        //community_print(graph->nodes[community].community);

        //cout << "Neighbours of selected community: ";
        //graph_print_array(graph, community);

        //cout << "[JCB]: Starting JCB Iteration" << endl;
        jcb_iteration(graph, heap, community, pagerank_iterations, pagerank_alpha);

        //cout << "[JCB]: Done JCB Iteration" << endl;
        //cout << "[JCB]: New community " << community << ": ";
        //community_print(graph->nodes[community].community);
        //cout << "[JCB]: Neighbours = ";
        //graph_print_array(graph, community);

        //cout << "[JCB]: New graph:" << endl;
        //graph_print(graph);
    }
}

void jcb_iteration(Graph* graph, Heap* heap, long node, int pagerank_iterations, double pagerank_alpha){
    cout << "[JCBIT]: Remaining communities = " << heap->length << endl;
    cout << "[JCBIT]: Degree of " << node << " = " << graph->nodes[node].degree << endl;

    long best_neighbor;
    long start_time;

    start_time = time(NULL);
    DEdgelist* vgraph = jcb_neighbours_d2(graph, node);
    cout << "[JCBIT]: Computed triangles in " << time(NULL) - start_time << endl;

    if(vgraph != NULL && !vgraph->empty()){
        start_time = time(NULL);
        DGraph* dgraph = dgraph_load_graph(vgraph);
        cout << "[JCBIT]: Builded subgraph in " << time(NULL) - start_time << endl;

        edgelist_deinit(vgraph);

        jcb_fix_dgraph_node_weight(graph, dgraph, node);

        start_time = time(NULL);

        best_neighbor = jcb_node_max_page_rank(graph, dgraph, node, pagerank_iterations, pagerank_alpha);
        cout << "[JCBIT]: Computed Page Rank in " << time(NULL) - start_time << endl;

        dgraph_deinit(dgraph);
    }
    else{
        if(vgraph != NULL)
            delete vgraph;

        if(graph->nodes[node].degree <= 0)
        {
            //NO NEIGHBOURS
            heap_remove_node(heap, graph, node);
            //i can't merge it
            return;
        }

        best_neighbor = graph->nodes[node].neighbours[rand() % graph->nodes[node].degree];
    }

    //Optimization based on the degree
    if(graph->nodes[best_neighbor].degree > graph->nodes[node].degree){
        long tmp = best_neighbor;
        best_neighbor = node;
        node = tmp;
    }



    //now merge the node with the neighbor
    start_time = time(NULL);
    long pos = graph_get_edge(graph, node, best_neighbor);
    long weight = graph->nodes[node].weight[pos];

    cout << "[JCBIT] Merging " << node << " " << best_neighbor << endl;
    if(community_quality_function(graph->nodes[node].community, graph->nodes[best_neighbor].community, weight))
        graph_merge_communities(graph, node, best_neighbor, weight);

    //cout << "[JCBIT]: Removing from heap the best neighbor " << best_neighbor << endl;
    heap_remove_node(heap, graph, best_neighbor);
    cout << "[JCBIT]: JCB in " << time(NULL) - start_time << endl;
}

//another way to get triangles
DEdgelist* jcb_neighbours_d2(Graph* graph, long node){
    if(graph == NULL)
        return NULL;

    DEdgelist* edgelist = edgelist_init();

    for (int i = 0; i < graph->nodes[node].degree; ++i) {
        long neighbour = graph->nodes[node].neighbours.at(i);


        edgelist->push_back(dedge_init(node, neighbour,  graph->nodes[node].weight.at(i)));

        for (int j = 0; j < graph->nodes[neighbour].degree; ++j) {
            long neighbour_d2 = graph->nodes[neighbour].neighbours.at(j);
            if(neighbour < neighbour_d2 && graph_contains_edge(graph, node, neighbour) && neighbour_d2 != node)
                edgelist->push_back(dedge_init(neighbour, graph->nodes[neighbour].neighbours.at(j), graph->nodes[neighbour].weight.at(j)));
        }
    }

    return edgelist;
}

void jcb_fix_dgraph_node_weight(Graph* graph, DGraph* dgraph, long nodeTarget){
    if(graph == NULL || dgraph == NULL || nodeTarget < 0)
        return;

    long neighbor;
    long weight;
    for (int i = 0; i < graph->nodes[nodeTarget].neighbours.size(); ++i) {
        neighbor = graph->nodes[nodeTarget].neighbours[i];
        weight = graph->nodes[nodeTarget].weight[i];

        dgraph_set_node_weight(dgraph, neighbor, weight);
    }
}

long page_rank(DGraph* dgraph, long target, double alpha, int iterations){
    double* P = new double[dgraph->number_nodes];

    double probability_come_from_neighbour = 0;
    double probability_of_teletrasport = 0;
    double max_pagerank=-1;
    long max_pagerank_index = -1;

    target -= dgraph->offset;
    P[target] = 1;


    long sum_node_weight = 0;
    for (int i = 0; i < dgraph->number_nodes; ++i) {
        sum_node_weight += dgraph->node_weight[i];
    }

    for (int i = 0; i < dgraph->number_nodes; ++i) {
        P[i] = (double)dgraph->node_weight[i]/(double)sum_node_weight;

        if(P[i] > max_pagerank && i != target){
            max_pagerank = P[i];
            max_pagerank_index = i;
        }
    }

    for (int k = 0; k < iterations; ++k) {
        max_pagerank = 0;
        max_pagerank_index = -1;
        for (long i = 0; i < dgraph->number_nodes; ++i) {
            probability_of_teletrasport = 1/(double)dgraph->number_nodes;
            probability_come_from_neighbour = 0;



            for (long j = dgraph->graph_out[i]; j < dgraph->graph_out[i] + dgraph->graph_degree_out[i]; ++j) {
                long neighbor = dgraph->neighbours_out[j];

                if(dgraph->graph_degree_out[neighbor] > 0) {
                    probability_come_from_neighbour +=
                            ((double) dgraph->neighbours_out_weight[j] / (double) dgraph->weight_out_sum[neighbor]) * P[neighbor];
                }
            }

            P[i] = (1.0-alpha) * probability_come_from_neighbour + alpha * probability_of_teletrasport;

            if(P[i] > max_pagerank && i != target){
                max_pagerank = P[i];
                max_pagerank_index = i;
            }
        }
    }
    P[target] = 0;

    delete [] P;

    cout << "Best pagerank node: " << max_pagerank_index << endl;
    return max_pagerank_index;
}

long jcb_node_max_page_rank(Graph* graph, DGraph* dgraph, long target, int pagerank_iterations, double pagerank_alpha){
    if(dgraph == NULL)
        return -1;

    if(dgraph->number_nodes <= 0)
        return -1;

    return page_rank(dgraph, target, pagerank_alpha, pagerank_iterations) + dgraph->offset;
}

void jcb_print_edgelist(DEdgelist* edgelist){
    for (int i = 0; i < edgelist->size(); ++i) {
        cout << edgelist->at(i)->nodeA << " " << edgelist->at(i)->nodeB << " " << edgelist->at(i)->weight << endl;
    }
}

void jcb_print_vector(vector<long>* v){
    for (int i = 0; i < v->size(); ++i) {
        //cout << v->at(i) << ", " ;
    }
    //cout << endl;
}

void jcb_print_array(long* array, long start, long end){
    for (int i = start; i < end; ++i) {
        //cout << array[i] << ", ";
    }
    //cout << endl;
}
