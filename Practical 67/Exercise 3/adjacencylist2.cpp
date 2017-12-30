#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist2.h"

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
    graph->communities = new Community*[number_nodes];
    graph->graph = new long[number_nodes];
    graph->graph_degree = new long[number_nodes];
    graph->graph_triangles = new long[number_nodes];
    graph->neighbours = new long[number_edges];
    graph->neighbours_weight = new long[number_edges];

    //Set default values for arrays
    memset(graph->graph, -1, sizeof(long)*number_nodes);
    memset(graph->neighbours, -1, sizeof(long)*number_edges);
    memset(graph->graph_degree, 0, sizeof(long)*number_nodes);
    memset(graph->graph_triangles, 0, sizeof(long)*number_nodes);
    memset(graph->neighbours_weight, (long)0, sizeof(long)*number_edges);

    //Initialize communities
    for (long i = 0; i < number_nodes; ++i) {
        graph->communities[i] = community_init();
        community_insert(graph->communities[i], i);
    }

    //Set important data for interact with arrays
    graph->number_edges = number_edges;
    graph->number_nodes = number_nodes;
    graph->number_communities = number_nodes;
    graph->offset = offset;

    return graph;
}

/**
 * Free the memory used by the graph
 */
void graph_deinit(Graph* graph){
    //Free remaining communities
    for (int i = 0; i < graph->number_communities; ++i) {
        community_deinit(graph->communities[i]);
    }

    //Free the memory used for arrays
    delete [] graph->graph;
    delete [] graph->graph_degree;
    delete [] graph->neighbours;
    delete [] graph->graph_triangles;
    delete [] graph->communities;

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
            graph->neighbours_weight[i] = 1;
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

// graph nodes = communities
// community v
// community u
bool graph_merge_communities(Graph* graph, long v, long u){
    cout << "[GRMERGE]: Prechecks. Communities: " << v << ", " << u << endl;
    if(graph == NULL)
        return false;

    cout << "[GRMERGE]: Links of community " << v << ": ";
    graph_print_array(graph, graph->graph[v], graph->graph[v] + graph->graph_degree[v]);

    cout << "[GRMERGE]: Links of community " << u << ": ";
    graph_print_array(graph, graph->graph[u], graph->graph[u] + graph->graph_degree[u]);

    //merge 2 communities
    cout << "[GRMERGE]: Get merged community" << endl;
    Community* final = community_merge(graph->communities[v], graph->communities[u]);
    cout << "[GRMERGE]: Final community = ";
    community_print(final);

    if(final == NULL)
        return false;

    cout << "[GRMERGE]: Free old communities" << endl;
    if(graph->communities[v] != NULL)
        community_deinit(graph->communities[v]);
    if(graph->communities[u] != NULL)
        community_deinit(graph->communities[u]);

    graph->communities[v] = NULL;
    graph->communities[u] = NULL;

    cout << "[GRMERGE]: Set new community" << endl;
    graph->communities[v] = final;

    //fix all neighbours of u such that they points against v
    cout << "[GRMERGE]: Fix all neighbors with the newest comm. ("<< graph->graph[u] + graph->graph_degree[u] << ")" << endl;
    for (long i = graph->graph[u]; i < graph->graph[u] + graph->graph_degree[u]; ++i) {
        long neighbours = graph->neighbours[i];

        if(neighbours == v)
            continue;

        cout << "[GRMERGE]: Substite the old comm with the new one (" << neighbours << ", old=" << u << ", new=" << v << ")" << endl;
        graph_substitute_neighbour(graph, neighbours, u, v);
    }

    //remove old link between the 2 communities
    cout << "[GRMERGE]: Links before removement: ";
    graph_print_array(graph, graph->graph[v], graph->graph[v] + graph->graph_degree[v]);
    graph_remove_edge(graph, v, u);
    cout << "[GRMERGE]: Links after removement: ";
    graph_print_array(graph, graph->graph[v], graph->graph[v] + graph->graph_degree[v]);

    cout << "[GRMERGE]: Decrease number of comm. and set one of the 2 comm as -1" << endl;
    graph->number_communities--;
    graph->graph[u] = -1;

    cout << "[GRMERGE]: Return" << endl;
    return true;
}

bool graph_substitute_neighbour(Graph* graph, long node, long old_node, long new_node){
    cout << "[GRSUB]: Prechecks: Node:" << node << ", Old Node: " << old_node << ", New Node: " << new_node << endl;
    if(graph == NULL)
        return false;

    if(node == new_node)
        return false;

    cout << "[GRSUB]: Getting indexes of neighbors" << endl;
    long index_old_node = graph_get_edge(graph, node, old_node);
    long index_new_node = graph_get_edge(graph, node, new_node);

    cout << "[GRSUB]: Neighbors of " << node << ": ";
    graph_print_array(graph, graph->graph[node], graph->graph[node] + graph->graph_degree[node]);

    //new node is not contained in neighbours of node
    //cout << "[GRSUB]: Checking indexes" << endl;
    if(index_new_node < 0){
        //change link
        if(index_old_node >= 0) {
            cout << "[GRSUB]: The new node is not a neighbor of " << node << endl;
            //TODO: sort array!
            graph->neighbours[index_old_node] = new_node;
            graph_sort_new_neighbor(graph, node, index_old_node);
            graph_add_edge(graph, new_node, node);
        }
    }
    else{
        cout << "[GRSUB]: There are both old and new node -> fix weight" << endl;
        cout << "[GRSUB]: Neighbours of " << node << ": ";
        graph_print_array(graph, graph->graph[node], graph->graph[node] + graph->graph_degree[node]);
        cout << "[GRSUB]: Neighbours of " << new_node << ": ";
        graph_print_array(graph, graph->graph[new_node], graph->graph[new_node] + graph->graph_degree[new_node]);
        //change weight and remove the old node
        cout << "[GRSUB]: Getting index for weight" << endl;
        long index_node_from_new_node = graph_get_edge(graph, new_node, node);
        //update weight from both directions
        cout << "[GRSUB]: Computing weight" << endl;
        graph->neighbours_weight[index_new_node] += graph->neighbours_weight[index_old_node];
        graph->neighbours_weight[index_node_from_new_node] += graph->neighbours_weight[index_old_node];
        cout << "[GRSUB]: Remove edge" << endl;
        graph_remove_edge(graph, node, old_node);
    }

    cout << "[GRSUB]: Neighbors of " << node << " after modification: ";
    graph_print_array(graph, graph->graph[node], graph->graph[node] + graph->graph_degree[node]);
    cout << "[GRSUB]: Return" << endl;
    return true;
}

bool graph_contains_edge(Graph* graph, long src, long dst){
    if(graph == NULL)
        return false;

    long start = graph->graph[src];
    long end = graph->graph[src] + graph->graph_degree[src];
    long m;

    if(graph->neighbours[start] == dst)
        return true;
    if(graph->neighbours[end] == dst)
        return true;

    while (start < end){
        m = (start+end) / 2;

        if(graph->neighbours[m] == dst)
            return true;
        else if(graph->neighbours[m] < dst){
            start = m+1;
        }
        else{
            end = m-1;
        }
    }

    return false;
}

//-1 if not in the neighborhood
//index otherwise
long graph_get_edge(Graph* graph, long src, long dst){
    //cout << "[GRGETEDGE]: Prechecks" << endl;
    if(graph == NULL)
        return NULL;

    cout << "[GRGETEDGE]: Node " << src << " with neighbor " << dst << endl;
    cout << "[GRGETEDGE]: Neighbors = ";
    graph_print_array(graph, graph->graph[src], graph->graph[src] + graph->graph_degree[src]);


    //cout << "[GRGETEDGE]: Defining variables" << endl;
    long start = graph->graph[src];
    long end = graph->graph[src] + graph->graph_degree[src];
    long m;

    //cout << "[GRGETEDGE]: Check if the wanted node is at the beginning or the end" << endl;
    if(graph->neighbours[start] == dst)
        return start;
    if(graph->neighbours[end] == dst)
        return end;

    //cout << "[GRGETEDGE]: Iterate..." << endl;
    while (start < end){
        //cout << "[GRGETEDGE]: Start = " << start << " | End = " << end << endl;
        m = (start+end) / 2;
        //cout << "[GRGETEDGE]: m="<< graph->neighbours[m] << endl;

        //cout << "[GRGETEDGE]: Check if neighbor is on the left or right" << endl;
        //cout << "[GRGETEDGE]: " << graph->neighbours[m] << " < " << dst << endl;
        if(graph->neighbours[m] == dst) {
            //cout << "[GRGETEDGE]: Found!" << endl;
            return m;
        }
        else if(graph->neighbours[start] == dst){
            //cout << "[GRGETEDGE]: Found!" << endl;
            return start;
        }
        else if(graph->neighbours[end] == dst){
            //cout << "[GRGETEDGE]: Found!" << endl;
            return end;
        }
        else if(graph->neighbours[m] < dst){
            //cout << "[GRGETEDGE]: Go on right" << endl;
            start = m+1;
        }
        else{
            //cout << "[GRGETEDGE]: Go on left" << endl;
            end = m-1;
        }
    }

    //cout << "[GRGETEDGE]: Not found" << endl;
    return -1;
}

bool graph_remove_edge(Graph* graph, long src, long dst){
    if(graph == NULL)
        return false;

    long index = graph_get_edge(graph, src, dst);
    long last_index = graph->graph[src] + graph->graph_degree[src] - 1;

    cout << "Start index: " << graph->graph[src] << " | End index: " << graph->graph[src] + graph->graph_degree[src] << " | Index: " << index << endl;

    if(index < 0)
        return false;

    //swap the node that must be deleted at the end
    for(int i=index; i<last_index;i++){
        cout << "[GRREMEDGE]: Swapping " << graph->neighbours[i] << " with " << graph->neighbours[i+1] << endl;
        swap(graph->neighbours[i], graph->neighbours[i+1]);
        swap(graph->neighbours_weight[i], graph->neighbours_weight[i+1]);
    }

    graph->graph_degree[src]--;

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

void graph_communities_to_file(Graph* graph, char* filename){
    cout << "[GR-COMM-FILE] Initializing" << endl;
    ofstream input;
    cout << "[GR-COMM-FILE] Opening file" << endl;
    input.open(filename, ios::out);

    //COL1 = node_id
    //COL2 = cluster_id
    cout << "[GR-COMM-FILE] Iterating on all nodes. If node-id > 0 then it is still a community" << endl;
    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->graph[i] < 0)
            continue;

        cout << "[GR-COMM-FILE] Community " << i << " with number elements = " << graph->communities[i]->length << endl;
        for (int j = 0; j < graph->communities[i]->length; ++j) {
            input << graph->communities[i]->array[j] << " " << i << endl;
        }
    }

    cout << "[GR-COMM-FILE] Closing file" << endl;
    input.close();
}

long* graph_get_communities(Graph* graph){
    if(graph == NULL)
        return NULL;

    long* communities = new long[graph->number_communities];
    long comm_index = 0;

    for (int i = 0; i < graph->number_nodes; ++i) {
        if(graph->graph[i] >= 0)
        {
            communities[comm_index++] = i;
        }
    }

    return communities;
}

void graph_print_array(Graph* graph, long start, long end){
    for (int i = start; i < end; ++i) {
        cout << graph->neighbours[i] << " (" << graph->neighbours_weight[i] << "), ";
    }
    cout << endl;
}

void graph_sort_new_neighbor(Graph* graph, long node, long neighbor_index){
    if(graph == NULL)
        return;

    bool done = false;

    long prev, next;
    long first = graph->graph[node];
    long last = graph->graph[node] + graph->graph_degree[node] - 1;

    while (!done){
        prev = neighbor_index -1;
        next = neighbor_index +1;

        if(next > last)
            next = last;
        if(prev < first)
            prev = first;

        if(graph->neighbours[prev] <= graph->neighbours[neighbor_index] && graph->neighbours[neighbor_index] <= graph->neighbours[next])
            done = true;
        else if(graph->neighbours[prev] > graph->neighbours[neighbor_index]){
            swap(graph->neighbours[prev], graph->neighbours[neighbor_index]);
            neighbor_index = prev;
        }
        else if(graph->neighbours[neighbor_index] > graph->neighbours[next]){
            swap(graph->neighbours[next], graph->neighbours[neighbor_index]);
            neighbor_index = next;
        }
    }
}