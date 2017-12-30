#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist.h"

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
    graph->graph = new long[number_nodes];
    graph->graph_degree = new long[number_nodes];
    graph->graph_triangles = new long[number_nodes];
    graph->neighbours = new long[number_edges];

    //Set default values for arrays
    memset(graph->graph, -1, sizeof(long)*number_nodes);
    memset(graph->neighbours, -1, sizeof(long)*number_edges);
    memset(graph->graph_degree, 0, sizeof(long)*number_nodes);
    memset(graph->graph_triangles, 0, sizeof(long)*number_nodes);

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
    delete [] graph->graph;
    delete [] graph->graph_degree;
    delete [] graph->neighbours;
    delete [] graph->graph_triangles;

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
 * Make a copy of the given graph without links
 * @param graph
 * @return
 */
Graph* graph_empty_copy(Graph* graph){
    Graph* copy = graph_init(graph->number_nodes, graph->number_edges, graph->offset);

    for (int i = 0; i < graph->number_nodes; ++i) {
        //copy->graph_degree[i] = graph->graph_degree[i];
        copy->graph[i] = graph->graph[i];
    }

    return copy;
}

/**
 * Add edge to the graph (in one directions A-->B, not B-->A)
 * @param node
 * @param neighbour
 * @return
 */
bool graph_add_edge(Graph* graph, long node, long neighbour){
    //If arrays are not initialized or node is the same as the given neighbour, then stop the function
    if(graph->graph == NULL || graph->neighbours == NULL || node == neighbour)
        return false;

    //Get correct index of the node
    long index = node;

    //Get first index of node's neighbours
    long first_neighbour = graph->graph[index];

    //Search the correct position of the given neighbour
    for(int i=first_neighbour;i<graph->graph_degree[index]+first_neighbour;i++){
        //If edge already exists, return false (not added)
        if(graph->neighbours[i] == neighbour){
            graph->graph_degree[node] -= 1; //Duplicate, then I reduce the degree
            return false;
        }


        //If empty spot, then assign the neighbour and stop the function
        if(graph->neighbours[i] < 0) {
            graph->neighbours[i] = neighbour;
            return true;
        }

        //Keep array ordered, therefore swap the given neighbour with the least bigger neighbour node (?)
        //After that, arrays elements must be fixed to their new places
        if(graph->neighbours[i] > neighbour){
            swap(graph->neighbours[i], neighbour);
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
    if(graph->graph_degree == NULL)
        return false;

    file_reset(file);

    long nodeA;
    long nodeB;

    while(file>>nodeA>>nodeB){
        nodeA -= graph->offset;
        nodeB -= graph->offset;

        //If nodeA is different from nodeB, then increase the degree of both nodes
        if(nodeA != nodeB) {
            graph->graph_degree[nodeA]++;
            graph->graph_degree[nodeB]++;
        }
    }

    return true;
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
            number_edges+=2;

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
        long first_neighbour = graph->graph[i];

        for(int j=first_neighbour;j<graph->graph_degree[i]+first_neighbour;j++){
            if(node < graph->neighbours[j])
                cout << (node+graph->offset) << " " << (graph->neighbours[j]+graph->offset) << endl;
        }
    }
}

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

/**
 * Configure the indexes of each node in order to get the first neighbour on the contiguous array
 * @return
 */
bool graph_set_nodes(Graph* graph){
    if(graph->graph == NULL || graph->neighbours == NULL || graph->graph_degree == NULL || graph->number_nodes<=0)
        return false;

    graph->graph[0] = 0;

    for(int i=1;i<graph->number_nodes;i++){
        graph->graph[i] = graph->graph[i-1] + graph->graph_degree[i-1];
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

        //Not direct graph --> I have to add the edge to both nodes
        graph_add_edge(graph, nodeA, nodeB);
        graph_add_edge(graph, nodeB, nodeA);
    }

    return true;
}

long graph_compute_triangles(Graph* graph){
    if(graph == NULL)
        return 0;
    if(graph->graph_triangles == NULL || graph->graph == NULL || graph->neighbours == NULL || graph->graph_degree == NULL)
        return 0;

    long count = 0;

    for(int i=0;i<graph->number_nodes; i++){
        graph->graph_triangles[i] = graph_count_triangles_node(graph, i);
        count += graph->graph_triangles[i];
    }
    
    return count;
}

long graph_count_triangles_node(Graph* graph, long node){
    if(graph == NULL || node < 0)
        return 0;

    long count = 0;

    for(int j=graph->graph[node]; j < graph->graph[node] + graph->graph_degree[node]; j++){

        long neighbor = graph->neighbours[j];

        //TODO: FIX INDEXES
        long start_nodeV = graph_index_least_great_element(graph->neighbours, graph->graph_degree[node], graph->graph[node], neighbor);
        long start_nodeU = graph_index_least_great_element(graph->neighbours, graph->graph_degree[neighbor], graph->graph[neighbor], neighbor);
        long end_nodeV = graph->graph[node] + graph->graph_degree[node];
        long end_nodeU = graph->graph[neighbor] + graph->graph_degree[neighbor];

        //cout << "Node " << node << " neighbor " << neighbor << endl;
        //cout << "Node start: " << start_nodeV << " end: " << end_nodeV << endl;
        //cout << "Neigh start: " << start_nodeU << " end: " << end_nodeU << endl;

        if(start_nodeU >= 0 && start_nodeV >= 0)
            count += graph_count_intersections(graph->neighbours, start_nodeV, end_nodeV, start_nodeU, end_nodeU);
    }
    return count;
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
long graph_count_intersections(long* array, long start1, long end1, long start2, long end2){
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


long graph_index_least_great_element(long* array, long array_length, long start, long value){
    // Array is already ordered
    for(int i=start;i<array_length+start; i++){
        if(array[i] > value)
            return i;
    }
    return -1;
}

bool graph_check_existence(Graph* original, long u, long v){
    if(original == NULL)
        return false;

    for (int i = 0; i < original->graph_degree[u]; ++i) {
        long neighbours = original->neighbours[original->graph[u] + i];

        if(neighbours == v)
            return true;
    }
    return false;
}

bool graph_sample_add_edge(Graph* original, Graph* sample, long u, long v){
    if(original == NULL || sample == NULL)
        return false;

    if(graph_check_existence(original, u, v))
    {
        graph_add_edge(sample, u, v);
        return true;
    }

    return false;
}