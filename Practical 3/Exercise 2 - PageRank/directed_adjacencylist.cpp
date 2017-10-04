#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "directed_adjacencylist.h"

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

//Graph graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

/**
 * Initialize the graph
 * @param number_nodes
 * @param number_edges
 */
Graph* graph_init(long number_nodes, long number_edges, long offset){
    Graph* graph = new Graph();
    //Initialize arrays and variables
    graph->graph_in = new long[number_nodes];
    graph->graph_out = new long[number_nodes];
    graph->graph_degree_in = new long[number_nodes];
    graph->graph_degree_out = new long[number_nodes];
    graph->neighbours_in = new long[number_edges];
    graph->neighbours_out = new long[number_edges];

    graph->last_neighbor_in = new long[number_nodes];
    graph->last_neighbor_out = new long[number_nodes];


    //Set default values for arrays
    memset(graph->graph_in, -1, sizeof(long)*number_nodes);
    memset(graph->graph_out, -1, sizeof(long)*number_nodes);
    memset(graph->neighbours_in, -1, sizeof(long)*number_edges);
    memset(graph->neighbours_out, -1, sizeof(long)*number_edges);
    memset(graph->graph_degree_in, 0, sizeof(long)*number_nodes);
    memset(graph->graph_degree_out, 0, sizeof(long)*number_nodes);

    memset(graph->last_neighbor_out, 0, sizeof(long)*number_nodes);
    memset(graph->last_neighbor_in, 0, sizeof(long)*number_nodes);

    //Set important data for interact with arrays
    graph->number_edges = number_edges;
    graph->number_nodes = number_nodes;
    graph->offset = offset;

    return graph;
}

/**
 * Free the memory used by the graph
 */
void graph_deinit(Graph* graph){
    //Free the memory used for arrays
    delete [] graph->graph_in;
    delete [] graph->graph_out;
    delete [] graph->graph_degree_in;
    delete [] graph->graph_degree_out;
    delete [] graph->neighbours_in;
    delete [] graph->neighbours_out;
    delete [] graph->last_neighbor_in;
    delete [] graph->last_neighbor_out;

    delete graph;
}

/**
 * Configure the graph and load data from file
 * @param name file name
 * @return
 */
Graph* graph_load_file(char* name){
    fstream input;
    Graph* graph;
    long number_nodes=0;
    long number_edges=0;
    long offset = 0;

    input.open(name);

    //If the file can't be opened, return false
    if(!input)
        return NULL;

    //Compute size of the graph and get the offset between the index for arrays and the ID of nodes
    graph_compute_size(input, number_nodes, number_edges, offset);

    //Initialize graph with the obtained size of the graph
    graph = graph_init(number_nodes, number_edges, offset);

    //Compute degree of each node
    graph_compute_degree_array(graph, input);

    //Set indexes of each node
    graph_set_nodes(graph);

    //Load data onto the data structure
    graph_load_data(graph, input);

    input.close();

    return graph;
}

/**
 * Add edge to the graph (in one directions A-->B, not B-->A)
 * @param node
 * @param neighbour
 * @return
 */
bool graph_add_edge_in(Graph* graph, long node, long neighbour){
    //If arrays are not initialized or node is the same as the given neighbour, then stop the function
    if(graph->graph_in == NULL || graph->neighbours_in == NULL || node == neighbour)
        return false;

    //Get correct index of the node
    long index = node;

    //Get first index of node's neighbours
    long first_neighbour = graph->graph_in[index];

    //Search the correct position of the given neighbour
    for(int i=first_neighbour;i<graph->graph_degree_in[index]+first_neighbour;i++){
        //If edge already exists, return false (not added)
        if(graph->neighbours_in[i] == neighbour){
            graph->graph_degree_in[node] -= 1; //Duplicate, then I reduce the degree
            return false;
        }


        //If empty spot, then assign the neighbour and stop the function
        if(graph->neighbours_in[i] < 0) {
            graph->neighbours_in[i] = neighbour;
            return true;
        }

        //Keep array ordered, therefore swap the given neighbour with the least bigger neighbour node (?)
        //After that, arrays elements must be fixed to their new places
        if(graph->neighbours_in[i] > neighbour){
            swap(graph->neighbours_in[i], neighbour);
        }
    }
    //Element not inserted into the array, is space finished?
    return false;
}

/**
 * Compute the array with the degree of each node
 * @param file
 * @return
 */
bool graph_compute_degree_array(Graph* graph, fstream &file){
    //If array for the graph degree is not initialized, return false
    if(graph->graph_degree_in == NULL && graph->graph_degree_out == NULL)
        return false;

    file_reset(file);

    long nodeA;
    long nodeB;

    while(file>>nodeA>>nodeB){
        nodeA -= graph->offset;
        nodeB -= graph->offset;

        //If nodeA is different from nodeB, then increase the degree of the nodeA
        if(nodeA != nodeB) {
            graph->graph_degree_out[nodeA]++;
            graph->graph_degree_in[nodeB]++;
        }
    }

    return true;
}

/**
 * Add edge to the graph (in one directions A-->B, not B-->A)
 * @param node
 * @param neighbour
 * @return
 */
bool graph_add_edge_out(Graph* graph, long node, long neighbour){
    //If arrays are not initialized or node is the same as the given neighbour, then stop the function
    if(graph->graph_out == NULL || graph->neighbours_out == NULL || node == neighbour)
        return false;

    //Get correct index of the node
    long index = node;

    //Get first index of node's neighbours
    long first_neighbour = graph->graph_out[index];

    //Search the correct position of the given neighbour
    for(int i=first_neighbour;i<graph->graph_degree_out[index]+first_neighbour;i++){
        //If edge already exists, return false (not added)
        if(graph->neighbours_out[i] == neighbour){
            graph->graph_degree_out[node] -= 1; //Duplicate, then I reduce the degree
            return false;
        }


        //If empty spot, then assign the neighbour and stop the function
        if(graph->neighbours_out[i] < 0) {
            graph->neighbours_out[i] = neighbour;
            return true;
        }

        //Keep array ordered, therefore swap the given neighbour with the least bigger neighbour node (?)
        //After that, arrays elements must be fixed to their new places
        if(graph->neighbours_out[i] > neighbour){
            swap(graph->neighbours_out[i], neighbour);
        }
    }
    //Element not inserted into the array, is space finished?
    return false;
}

/**
 * Compute the size of the graph by looking the ID of nodes and the number of rows (without considering self loops)
 * It may count more edges, in this case some space will be allocated but not used
 * @param file
 * @return
 */
bool graph_compute_size(fstream &file, long &number_nodes, long &number_edges, long &offset){
    if(!file)
        return false;

    file_reset(file);

    long nodeA;
    long nodeB;

    long min_id = LONG_MAX;
    long max_id = 0;

    // Count how many couples there are in the file
    while(file>>nodeA>>nodeB){

        if(nodeA!=nodeB)
            number_edges+=1;

        nodeA < min_id ? min_id = nodeA : NULL;
        nodeB < min_id ? min_id = nodeB : NULL;

        nodeA > max_id ? max_id = nodeA : NULL;
        nodeB > max_id ? max_id = nodeB : NULL;
    }

    number_nodes = max_id - min_id + 1;
    number_edges = number_edges;
    offset = min_id;

    return true;
}

/**
 * Print the edges of the graph without duplicates or self loops
 */
void graph_print(Graph* graph){
    for(int i=0;i<graph->number_nodes;i++){
        long node = i;
        long first_neighbour = graph->graph_out[i];

        for(int j=first_neighbour;j<graph->graph_degree_out[i]+first_neighbour;j++){
            if(node < graph->neighbours_out[j])
                cout << (node+graph->offset) << " " << (graph->neighbours_out[j]+graph->offset) << endl;
        }
    }
}

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

/**
 * Configure the indexes of each node in order to get the first neighbour on the contiguous array
 * @return
 */
bool graph_set_nodes(Graph* graph){
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

void swap(long &a, long &b){
    long tmp = a;
    a = b;
    b = tmp;
}

/**
 * Bring the file pointer to the beginning
 * @param file
 */
void file_reset(fstream &file){
    file.clear();
    file.seekg(0, file.beg);
}

/**
 * Load the whole file into the structure
 * @param file
 * @return
 */
bool graph_load_data(Graph* graph, fstream &file){
    file_reset(file);

    long nodeA;
    long nodeB;

    while(file>>nodeA>>nodeB){
        nodeA -= graph->offset;
        nodeB -= graph->offset;

        //cout << nodeA << " " << nodeB << endl;

        //Direct graph --> I have to add the edge to one node
        //graph_add_edge_out(graph, nodeA, nodeB);
        //graph_add_edge_in(graph, nodeB, nodeA);

        fast_graph_add_edge_out(graph, nodeA, nodeB);
        fast_graph_add_edge_in(graph, nodeB, nodeA);
    }

    return true;
}

bool fast_graph_add_edge_out(Graph* graph, long node, long neighbour){
    //If arrays are not initialized or node is the same as the given neighbour, then stop the function
    if(graph->graph_out == NULL || graph->neighbours_out == NULL || node == neighbour)
        return false;

    if(graph->last_neighbor_out[node] >= graph->graph_degree_out[node])
        return false;

    long first_available_spot = graph->last_neighbor_out[node] + graph->graph_out[node];

    graph->neighbours_out[first_available_spot] = neighbour;

    graph->last_neighbor_out[node]++;

    return true;
}

bool fast_graph_add_edge_in(Graph* graph, long node, long neighbour){
    //If arrays are not initialized or node is the same as the given neighbour, then stop the function
    if(graph->graph_in == NULL || graph->neighbours_in == NULL || node == neighbour)
        return false;

    if(graph->last_neighbor_in[node] >= graph->graph_degree_in[node])
        return false;

    long first_available_spot = graph->last_neighbor_in[node] + graph->graph_in[node];

    graph->neighbours_in[first_available_spot] = neighbour;

    graph->last_neighbor_in[node]++;

    return true;
}