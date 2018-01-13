#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <utility>
#include "directed_adjacencylist.h"

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

//Graph graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

/**
 * Initialize the graph
 * @param number_nodes
 * @param number_edges
 */
DGraph* dgraph_init(long number_nodes, long number_edges, long offset){
    ////cout << "[DG-INIT]: Initializing all attributes of the graph" << endl;
    DGraph* graph = new DGraph();
    ////cout << "[DG-INIT]: Number nodes " << number_nodes << endl;
    //Initialize arrays and variables
    graph->graph_in = new long[number_nodes];
    graph->graph_out = new long[number_nodes];

    graph->graph_degree_in = new long[number_nodes];
    graph->graph_degree_out = new long[number_nodes];

    graph->number_neighbours = new long[number_nodes];

    graph->neighbours_in = new long[number_edges];
    graph->neighbours_in_weight = new long[number_edges];
    graph->neighbours_out = new long[number_edges];
    graph->neighbours_out_weight = new long[number_edges];

    graph->last_neighbor_in = new long[number_nodes];
    graph->last_neighbor_out = new long[number_nodes];
    graph->weight_out_sum = new long[number_nodes];
    graph->weight_in_sum = new long[number_nodes];
    graph->node_weight = new long[number_nodes];

    ////cout << "[DG-INIT]: Setting to default values the attributes of the graph" << endl;
    //Set default values for arrays
    memset(graph->graph_in, -1, sizeof(long)*number_nodes);
    memset(graph->graph_out, -1, sizeof(long)*number_nodes);

    memset(graph->neighbours_in, -1, sizeof(long)*number_edges);
    memset(graph->neighbours_out, -1, sizeof(long)*number_edges);
    memset(graph->neighbours_in_weight, 0, sizeof(long)*number_edges);
    memset(graph->neighbours_out_weight, 0, sizeof(long)*number_edges);

    memset(graph->number_neighbours, 0, sizeof(long)*number_nodes);

    memset(graph->graph_degree_in, 0, sizeof(long)*number_nodes);
    memset(graph->graph_degree_out, 0, sizeof(long)*number_nodes);

    memset(graph->last_neighbor_out, 0, sizeof(long)*number_nodes);
    memset(graph->last_neighbor_in, 0, sizeof(long)*number_nodes);

    memset(graph->weight_in_sum, 0, sizeof(long)*number_nodes);
    memset(graph->weight_out_sum, 0, sizeof(long)*number_nodes);
    memset(graph->node_weight, 0, sizeof(long)*number_nodes);

    //Set important data for interact with arrays
    graph->number_edges = number_edges;
    graph->number_nodes = number_nodes;
    graph->offset = offset;

    ////cout << "[DG-INIT]: Returning the graph" << endl;
    return graph;
}

void dgraph_set_node_weight(DGraph* dgraph, long graph_node, long weight){
    if(dgraph == NULL)
        return;

    //cout << "###############################" << endl;
    //cout << "Graph Target = " << graph_node << endl;
    graph_node -= dgraph->offset;
    //cout << "DGraph Target = " << graph_node << " with weight " << weight << endl;
    dgraph->node_weight[graph_node] = weight;

    //dgraph_print(dgraph);
    //cout << "###############################" << endl;
}

/**
 * Free the memory used by the graph
 */
void dgraph_deinit(DGraph* graph){
    //Free the memory used for arrays
    delete [] graph->graph_in;
    delete [] graph->graph_out;
    delete [] graph->graph_degree_in;
    delete [] graph->graph_degree_out;
    delete [] graph->neighbours_in;
    delete [] graph->neighbours_in_weight;
    delete [] graph->neighbours_out;
    delete [] graph->neighbours_out_weight;
    delete [] graph->last_neighbor_in;
    delete [] graph->last_neighbor_out;
    delete [] graph->number_neighbours;
    delete [] graph->weight_out_sum;
    delete [] graph->weight_in_sum;
    delete [] graph->node_weight;

    delete graph;
}

DGraph* dgraph_load_graph(DEdgelist* edgelist){
    //cout << "[DG-LOAD]: Defining values" << endl;
    DGraph* graph;
    long number_nodes=0;
    long number_edges=0;
    long offset = 0;

    //cout << "[DG-LOAD]: Pre check on input graph" << endl;
    //If the file can't be opened, return false
    if(edgelist == NULL)
        return NULL;

    if(edgelist->size() <= 0)
        return NULL;

    //cout << "[DG-LOAD]: Computing size of the graph" << endl;
    //Compute size of the graph and get the offset between the index for arrays and the ID of nodes
    dgraph_compute_size(edgelist, number_nodes, number_edges, offset);

    //cout << "[DG-LOAD]: Initializing the direct graph" << endl;
    //Initialize graph with the obtained size of the graph
    graph = dgraph_init(number_nodes, number_edges, offset);

    //cout << "[DG-LOAD]: Computing degree array" << endl;
    //Compute degree of each node
    dgraph_compute_degree_array(graph, edgelist);

    //cout << "[DG-LOAD]: Setting nodes of the direct graph" << endl;
    //Set indexes of each node
    dgraph_set_nodes(graph);

    //cout << "[DG-LOAD]: Loading data" << endl;
    //Load data onto the data structure
    dgraph_load_data(graph, edgelist);

    //cout << "[DG-LOAD]: Resulting graph: " << endl;
    //dgraph_print(graph);

    return graph;
}




/**
 * Compute the array with the degree of each node
 * @param file
 * @return
 */
bool dgraph_compute_degree_array(DGraph* graph, DEdgelist* edgelist){
    ////cout << "[DGRAPH-DEGREE]: Prechecks" << endl;
    //If array for the graph degree is not initialized, return false
    if(graph->graph_degree_in == NULL && graph->graph_degree_out == NULL)
        return false;

    long nodeA;
    long nodeB;

    ////cout << "[DGRAPH-DEGREE]: Running throw the edge list" << endl;
    for(int i=0; i<edgelist->size(); i++){
        nodeA = edgelist->at(i)->nodeA - graph->offset;
        nodeB = edgelist->at(i)->nodeB - graph->offset;

        ////cout << "[DGRAPH-DEGREE]: Nodes = " << nodeA << ", " << nodeB << endl;

        //If nodeA is different from nodeB, then increase the degree of the nodeA
        if(nodeA != nodeB) {
            graph->graph_degree_out[nodeA]++;
            graph->graph_degree_out[nodeB]++;
            graph->graph_degree_in[nodeB]++;
            graph->graph_degree_in[nodeA]++;
        }


        ////cout << "[DGRAPH-DEGREE]: Node " << nodeA << ": out=" << graph->graph_degree_out[nodeA] << " and in=" << graph->graph_degree_in[nodeA] << endl;
        ////cout << "[DGRAPH-DEGREE]: Node " << nodeB << ": out=" << graph->graph_degree_out[nodeB] << " and in=" << graph->graph_degree_in[nodeB] << endl;
    }

    return true;
}

/**
 * Compute the size of the graph by looking the ID of nodes and the number of rows (without considering self loops)
 * It may count more edges, in this case some space will be allocated but not used
 * @param file
 * @return
 */
bool dgraph_compute_size(DEdgelist* edgelist, long &number_nodes, long &number_edges, long &offset){
    if(edgelist == NULL)
        return false;

    ////cout << "[DG-SIZE]: Defining variables" << endl;

    long nodeA;
    long nodeB;

    long min_id = LONG_MAX;
    long max_id = 0;

    ////cout << "[DG-SIZE]: Foreach pair in the graph" << endl;
    for(int i=0; i<edgelist->size(); i++) {
        ////cout << "[DG-SIZE]: Select 2 nodes" << endl;
        nodeA = edgelist->at(i)->nodeA;
        nodeB = edgelist->at(i)->nodeB;
        ////cout << "[DG-SIZE]: NodeA="<<nodeA<<" NodeB="<< nodeB << endl;

        ////cout << "[DG-SIZE]: Check if they are different" << endl;
        if(nodeA!=nodeB)
            number_edges+=1;

        ////cout << "[DG-SIZE]: Select MIN and MAX" << endl;
        nodeA < min_id ? min_id = nodeA : NULL;
        nodeB < min_id ? min_id = nodeB : NULL;

        nodeA > max_id ? max_id = nodeA : NULL;
        nodeB > max_id ? max_id = nodeB : NULL;
    }

    ////cout << "[DG-SIZE]: Setting output data and return" << endl;
    number_nodes = max_id - min_id + 1;
    number_edges = 2*number_edges;
    offset = min_id;

    return true;
}


/**
 * Print the edges of the graph without duplicates or self loops
 */
void dgraph_print(DGraph* graph){
    cout << "Number nodes = " << graph->number_nodes << endl;
    cout << "Number edges = " << graph->number_edges << endl;
    cout << "Offset = " << graph->offset << endl;
    for(int i=0;i<graph->number_nodes;i++){
        long node = i;
        long first_neighbour = graph->graph_out[i];

        cout << "Node " << node << " [out:" << graph->graph_degree_out[node] << ", in:" << graph->graph_degree_in[node] << "]: ";

        for(int j=first_neighbour;j<graph->graph_degree_out[i]+first_neighbour;j++){
            cout << graph->neighbours_out[j] << " (" << graph->neighbours_out_weight[j] << "), ";
        }
        cout << endl;

        /*
        for(int j=first_neighbour;j<graph->graph_degree_out[i]+first_neighbour;j++){
            if(node < graph->neighbours_out[j])
                //cout << node << " " << graph->neighbours_out[j] << endl;
        }
         */
    }
}

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

/**
 * Configure the indexes of each node in order to get the first neighbour on the contiguous array
 * @return
 */
bool dgraph_set_nodes(DGraph* graph){
    if(graph->graph_in == NULL || graph->neighbours_in == NULL || graph->graph_degree_in == NULL || graph->number_nodes<=0)
        return false;

    if(graph->graph_out == NULL || graph->neighbours_out == NULL || graph->graph_degree_out == NULL)
        return false;

    graph->graph_in[0] = 0;
    graph->graph_out[0] = 0;

    for(int i=1;i<graph->number_nodes;i++){
        graph->graph_in[i] = graph->graph_in[i-1] + graph->graph_degree_in[i-1];
        graph->graph_out[i] = graph->graph_out[i-1] + graph->graph_degree_out[i-1];
    }

    return true;
}

void dgraph_swap(long &a, long &b){
    long tmp = a;
    a = b;
    b = tmp;
}


/**
 * Load the whole file into the structure
 * @param file
 * @return
 */
bool dgraph_load_data(DGraph* graph, DEdgelist* edgelist){
    long nodeA;
    long nodeB;
    long weight;
    for(int i=0; i<edgelist->size(); i++){
        nodeA = edgelist->at(i)->nodeA - graph->offset;
        nodeB = edgelist->at(i)->nodeB - graph->offset;
        weight = edgelist->at(i)->weight;

        fast_graph_add_edge_out(graph, nodeA, nodeB, weight);

        fast_graph_add_edge_out(graph, nodeB, nodeA, weight);
    }

    return true;
}


bool fast_graph_add_edge_out(DGraph* graph, long node, long neighbour, long weight){
    //If arrays are not initialized or node is the same as the given neighbour, then stop the function
    if(graph->graph_out == NULL || graph->neighbours_out == NULL || node == neighbour)
        return false;

    if(graph->last_neighbor_out[node] >= graph->graph_degree_out[node])
        return false;

    long first_available_spot = graph->last_neighbor_out[node] + graph->graph_out[node];

    graph->neighbours_out[first_available_spot] = neighbour;
    graph->neighbours_out_weight[first_available_spot] = weight;
    graph->weight_out_sum[node] += weight;

    graph->last_neighbor_out[node]++;

    return true;
}

// -------------

DEdgelist* edgelist_init(){
    return new DEdgelist();
}

void edgelist_deinit(DEdgelist* edgelist){
    if(edgelist == NULL)
        return;

    for (int i = 0; i < edgelist->size(); ++i) {
        dedge_deinit(edgelist->at(i));
    }

    delete(edgelist);
}

DEdge* dedge_init(long nodeA, long nodeB, long weight){
    DEdge* dEdge = new DEdge();
    dEdge->nodeA = nodeA;
    dEdge->nodeB = nodeB;
    dEdge->weight = weight;
    return dEdge;
}

void dedge_deinit(DEdge* dEdge){
    delete dEdge;
}