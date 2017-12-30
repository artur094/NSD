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



void jackcomesback(Graph* graph, long number_communities){
    //TODO: FIX REORDERING USING HEAP (#degree * #weight)
    Heap* heap = heap_init(graph);
    heap_restore(heap);
    srand(time(NULL));
    cout << "[JCB]: Starting loop... " << endl;
    while (graph->number_communities > number_communities){
        cout << "[JCB]: Getting best community " << endl;
        heap_print(heap);

        /*
        long community_index = rand() % graph->number_communities;

        cout << "[JCB]: Getting available communities" << endl;
        long* communities = graph_get_communities(graph);

        cout << "[JCB]: Getting the chosen community" << endl;
        long community = communities[community_index]; //Get indexes of existing communities
         */
        long community = heap_first_element(heap);
        cout << "Best community = " << community << endl;

        //cout << "Selected community " << community << " with the following nodes: " ;
        //community_print(graph->nodes[community].community);

        //cout << "Neighbours of selected community: ";
        //graph_print_array(graph, community);

        //cout<< "[JCB]: Freeing memory used for detecting the random community" << endl;
        //delete [] communities;

        cout << "[JCB]: Starting JCB Iteration" << endl;
        jcb_iteration(graph, heap, community);

        cout << "[JCB]: Done JCB Iteration" << endl;
        //cout << "[JCB]: New community " << community << ": ";
        //community_print(graph->nodes[community].community);
        //cout << "[JCB]: Neighbours = ";
        //graph_print_array(graph, community);

        //cout << "[JCB]: New graph:" << endl;
        //graph_print(graph);

        int debug;
        cin >> debug;
    }
}

void jcb_iteration(Graph* graph, Heap* heap, long node){
    //build graph from triangles
    long best_neighbor;

    cout << "[JCBIT]: Building the edge list graph based on triangles" << endl;
    vector<pair<long,long> >* vgraph = jcb_build_triangle_graph(graph, node);

    //cout << "[JCBIT]: Obtained graph: ";
    //jcb_print_vgraph(vgraph);

    if(vgraph != NULL && vgraph->size() > 0){
        cout << "[JCBIT]: Graph: " << endl;
        graph_print(graph);

        cout << "[JCBIT]: Loading the edge list graph into adjacency list graph" << endl;
        DGraph* dgraph = dgraph_load_graph(vgraph);

        cout << "[JCBIT]: Freeing memory used for edge list graph" << endl;
        delete vgraph;

        //TODO: check node IDs between graph (undirected) and dgrap (directed): if they corresponds or not
        //get best neighbor based on pagerank values
        cout << "[JCBIT]: Remaining communities = " << graph->number_communities << endl;
        cout << "[JCBIT]: Choosing the best neighbor of the selected community using pagerank" << endl;
        best_neighbor = jcb_node_max_page_rank(dgraph, node);
        cout << "[JCBIT]: Choosen community: " << best_neighbor << endl;

        cout << "[JCBIT]: Freeing memory used for the adjacency list" << endl;
        dgraph_deinit(dgraph);
    }
    else{
        if(vgraph != NULL)
            delete vgraph;

        //cout << "Empty graph, selecting a neighbour randomly. Possible neighbours = ";
        //graph_print_array(graph, node);
        best_neighbor = graph->nodes[node].neighbours[rand() % graph->nodes[node].degree];
        cout << "Chosen neighbor: " << best_neighbor << endl;
    }

    vgraph = NULL;

    //now merge the node with the neighbor
    cout << "[JCBIT]: Merging the 2 communities" << endl;
    graph_merge_communities(graph, node, best_neighbor);

    heap_remove_node(heap, graph, best_neighbor);
}

//build graph from triangles by generating a list of edges of links between common neighbours
vector<pair<long, long> >* jcb_build_triangle_graph(Graph* graph, long node)
{
    if(graph == NULL)
        return NULL;

    //cout << "[JCBB]: Computing useful data" << endl;
    long node_start = 0;
    long node_end = graph->nodes[node].degree;
    //cout << "[JCBB]: Node " << node << " neighbors = ";
    //graph_print_array(graph, node);

    //cout << "[JCBB]: Initializing edge list graph" << endl;
    vector<pair<long, long> >* edgelist = new vector<pair<long,long> >();

    //cout << "[JCBB]: Iterating for each neighbor" << endl;
    for (int i = node_start; i < node_end; ++i) {
        long neighbor = graph->nodes[node].neighbours[i];

        //cout << "[JCBB]: Computing intersection to find common neighbors" << endl;
        vector<long>* intersection = jcb_intersection(graph, node, neighbor);

        //cout << "[JCBB]: Checking if there are any common neighbors" << endl;
        if(intersection != NULL){
            //cout << "[JCBB]: Add each link between 2 neighbor of the target community in the edge list" << endl;
            for(int j=0; j<intersection->size(); j++){
                //cout << "[JCBB]: Adding to edge list: "<<neighbor<<", "<<intersection->at(j)<< endl;
                edgelist->push_back(make_pair(neighbor, intersection->at(j)));
            }

        }

        //add links also between target and neighbours (multiple times for the weight
        for (int k = 0; k < graph->nodes[node].weight[i]; ++k) {
            cout << "Adding " << node << " " << neighbor << endl;
            edgelist->push_back(make_pair(node, neighbor));
        }

        //cout << "[JCBB]: Free intersection" << endl;
        delete intersection;
    }



    cout << "[JCBB]: Returning edge list" << endl;
    jcb_print_vgraph(edgelist);
    return edgelist;
}

//get neighbours in common
//TODO: fix problem with intersection weight (wrong weight between common links). Between target and neighbors it's fine!
vector<long>* jcb_intersection(Graph* graph, long node, long neighbor){
    //cout << "[JCBIN]: Initial checks" << endl;
    if(graph == NULL)
        return NULL;

    //cout << "[JCBIN]: Defining start/end indexes" << endl;
    long node_start = 0;
    long node_end = graph->nodes[node].degree;
    long neighbor_start = graph_index_least_great_element(graph->nodes[neighbor].neighbours, graph->nodes[neighbor].degree, neighbor);
    long neighbor_end = graph->nodes[neighbor].degree;

    long node_index = node_start;
    long neighbor_index = neighbor_start;

    //cout <<"[JCBIN]: Node " << node << " neighbors = ";
    //graph_print_array(graph, node);
    //cout << "[JCBIN]: Neighbor " << neighbor << " neighbors (cut from " << neighbor_start << ") = ";
    //graph_print_array(graph, neighbor);

    //cout << "[JCBIN]: Initializing intersection" << endl;
    vector<long>* intersection = new vector<long>();

    //cout << "[JCBIN]: Iterate to find common nodes" << endl;
    while(node_index < node_end && neighbor_index < neighbor_end && neighbor_start>=0){
        //cout << "[JCBIN]: Checking " << graph->nodes[node].neighbours[node_index] << " and " << graph->nodes[neighbor].neighbours[neighbor_index] << endl;

        if(graph->nodes[node].neighbours[node_index] == graph->nodes[neighbor].neighbours[neighbor_index]){
            //cout << "[JCBIN]: Common node found ("<<graph->nodes[node].neighbours[node_index]<<"), add more time the neighbor based on the weight ("<<graph->nodes[neighbor].weight[neighbor_index]<<")" << endl;
            for (int i = 0; i < graph->nodes[node].weight[node_index]; ++i) {
                intersection->push_back(graph->nodes[node].neighbours[node_index]);
            }
            node_index++;
            neighbor_index++;
        }
        else if(graph->nodes[node].neighbours[node_index] < graph->nodes[neighbor].neighbours[neighbor_index]){
            //cout << "[JCBIN]: Increasing node index" << endl;
            node_index++;
        }
        else{
            //cout << "[JCBIN]: Increasing neighbor index" << endl;
            neighbor_index++;
        }
    }

    //cout << "[JCBIN]: Return intersection" << endl;
    return intersection;
}

long index_least_great_element(long* array, long array_length, long start, long value){
    // Array is already ordered
    for(int i=start;i<array_length+start; i++){
        if(array[i] > value)
            return i;
    }
    return start+array_length;
}

double* page_rank(DGraph* dgraph, long target, double alpha, int iterations){
    double* P = new double[dgraph->number_nodes];

    double probability_come_from_neighbour = 0;
    double probability_of_teletrasport = 0;

    target -= dgraph->offset;

    //memset(P, 1.0/(double)graph->number_nodes, sizeof(double)*graph->number_nodes);

    for (int i = 0; i < dgraph->number_nodes; ++i) {
        P[i] = 1.0/(double)dgraph->number_nodes;
        cout << "Degree of " << i+dgraph->offset << " = " <<  dgraph->graph_degree_out[i] << endl;
    }

    for (int k = 0; k < iterations; ++k) {
        for (long i = 0; i < dgraph->number_nodes; ++i) {
            probability_of_teletrasport = 1/(double)dgraph->number_nodes;
            probability_come_from_neighbour = 0;

            for (long j = dgraph->graph_in[i]; j < dgraph->graph_in[i] + dgraph->graph_degree_in[i]; ++j) {
                long neighbor = dgraph->neighbours_in[j];

                //cout << "Studying " << i+graph->offset << " for neighbour "<< neighbor << " with degree " << graph->graph_degree[neighbor] << endl;
                //TODO: CHECK IN LINK, NOT OUT
                if(dgraph->graph_degree_out[neighbor] > 0) {
                    //TODO: 1/nr_node is with OUT nodes, but I am adding the IN nodes!
                    //Considering the weight of the link / number of degree
                    probability_come_from_neighbour +=
                            ((double) dgraph->neighbours_in_weight[j] / (double) dgraph->graph_degree_out[neighbor]) *
                            P[neighbor];
                    //cout << "P["<<i<<"] += " << (double)dgraph->neighbours_in_weight[j] << " / ( " << (double)dgraph->graph_degree_out[neighbor] << " * " << P[neighbor] << ")" << endl;
                }
            }
            //cout << "P["<<i<<"] = (1 - " << alpha << " ) * " << probability_come_from_neighbour << " + " << alpha << " * " << probability_of_teletrasport << endl;
            P[i] = (1.0-alpha) * probability_come_from_neighbour + alpha * probability_of_teletrasport;
        }
    }

    P[target] = -1;
    cout << "################# Pagerank #################" << endl;
    for (int i = 0; i < dgraph->number_nodes; ++i) {
        cout << i+dgraph->offset << " -> " << P[i] << endl;
    }
    cout << "################# #################" << endl;
    return P;
}

long jcb_node_max_page_rank(DGraph* dgraph, long target){
    if(dgraph == NULL)
        return -1;

    if(dgraph->number_nodes <= 0)
        return -1;

    //get page rank values
    double* prank = page_rank(dgraph,target, ALPHA, ITERATIONS);

    long max_i = 0;
    double max_rank = prank[0];

    for (int i = 0; i < dgraph->number_nodes; ++i) {
        if(max_rank < prank[i]){
            max_rank = prank[i];
            max_i = i;
        }
    }

    return max_i + dgraph->offset;
}

void jcb_print_vgraph(vector<pair<long, long> >* vgraph){
    for (int i = 0; i < vgraph->size(); ++i) {
        cout << vgraph->at(i).first << " " << vgraph->at(i).second << ", ";
    }
    cout << endl;
}

void jcb_print_vector(vector<long>* v){
    for (int i = 0; i < v->size(); ++i) {
        cout << v->at(i) << ", " ;
    }
    cout << endl;
}

void jcb_print_array(long* array, long start, long end){
    for (int i = start; i < end; ++i) {
        cout << array[i] << ", ";
    }
    cout << endl;
}

/*
4: 5 (3), 8 (1),
5: 4 (2), 6 (2), 8 (3), 9 (2),
6: 5 (3),
8: 4 (1), 5 (3),
9: 5 (3),
 */




/*************** OLD *****************/
//build graph from triangles by generating a list of edges of links between common neighbours
vector<pair<long, long> >* jcb_build_triangle_graph_old(Graph* graph, long node)
{
    if(graph == NULL)
        return NULL;

    //cout << "[JCBB]: Computing useful data" << endl;
    long node_start = 0;
    long node_end = graph->nodes[node].degree;
    //cout << "[JCBB]: Node " << node << " neighbors = ";
    //graph_print_array(graph, node);

    //cout << "[JCBB]: Initializing edge list graph" << endl;
    vector<pair<long, long> >* edgelist = new vector<pair<long,long> >();

    //cout << "[JCBB]: Iterating for each neighbor" << endl;
    for (int i = node_start; i < node_end; ++i) {
        long neighbor = graph->nodes[node].neighbours[i];

        //cout << "[JCBB]: Computing intersection to find common neighbors" << endl;
        vector<long>* intersection = jcb_intersection(graph, node, neighbor);

        //cout << "[JCBB]: Checking if there are any common neighbors" << endl;
        if(intersection != NULL){
            //cout << "[JCBB]: Add each link between 2 neighbor of the target community in the edge list" << endl;
            for(int j=0; j<intersection->size(); j++){
                //cout << "[JCBB]: Adding to edge list: "<<neighbor<<", "<<intersection->at(j)<< endl;
                edgelist->push_back(make_pair(neighbor, intersection->at(j)));
            }

        }

        //cout << "[JCBB]: Free intersection" << endl;
        delete intersection;
    }

    //cout << "[JCBB]: Returning edge list" << endl;
    //jcb_print_vgraph(edgelist);
    return edgelist;
}

double* page_rank_old(DGraph* dgraph, double alpha, int iterations){
    double* P = new double[dgraph->number_nodes];

    double probability_come_from_neighbour = 0;
    double probability_of_teletrasport = 0;

    //memset(P, 1.0/(double)graph->number_nodes, sizeof(double)*graph->number_nodes);

    for (int i = 0; i < dgraph->number_nodes; ++i) {
        P[i] = 1.0/(double)dgraph->number_nodes;
        cout << "Degree of " << i+dgraph->offset << " = " <<  dgraph->graph_degree_out[i] << endl;
    }

    for (int k = 0; k < iterations; ++k) {
        for (long i = 0; i < dgraph->number_nodes; ++i) {
            probability_of_teletrasport = 1/(double)dgraph->number_nodes;
            probability_come_from_neighbour = 0;

            for (long j = dgraph->graph_in[i]; j < dgraph->graph_in[i] + dgraph->graph_degree_in[i]; ++j) {
                long neighbor = dgraph->neighbours_in[j];

                //cout << "Studying " << i+graph->offset << " for neighbour "<< neighbor << " with degree " << graph->graph_degree[neighbor] << endl;
                //TODO: CHECK IN LINK, NOT OUT
                if(dgraph->graph_degree_out[neighbor] > 0) {
                    //TODO: 1/nr_node is with OUT nodes, but I am adding the IN nodes!
                    //Considering the weight of the link / number of degree
                    probability_come_from_neighbour +=
                            ((double) dgraph->neighbours_in_weight[j] / (double) dgraph->graph_degree_out[neighbor]) *
                            P[neighbor];
                    //cout << "P["<<i<<"] += " << (double)dgraph->neighbours_in_weight[j] << " / ( " << (double)dgraph->graph_degree_out[neighbor] << " * " << P[neighbor] << ")" << endl;
                }
            }
            //cout << "P["<<i<<"] = (1 - " << alpha << " ) * " << probability_come_from_neighbour << " + " << alpha << " * " << probability_of_teletrasport << endl;
            P[i] = (1.0-alpha) * probability_come_from_neighbour + alpha * probability_of_teletrasport;
        }
    }

    cout << "################# Pagerank #################" << endl;
    for (int i = 0; i < dgraph->number_nodes; ++i) {
        cout << i+dgraph->offset << " -> " << P[i] << endl;
    }
    cout << "################# #################" << endl;
    return P;
}