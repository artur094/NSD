#include <iostream>
#include <fstream>
#include <ctime>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

long *neighbours = NULL;
long *graph = NULL;
long *graph_degree = NULL;
fstream input_graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name);
void get_nodes_edges(fstream &file, long &min_id, long &max_id, long &edges);

/* - - - - - - - - - - - - - - - - - MAIN - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char** argv) {

    cout << "NSD Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        read_file(argv[1]);
    else
        read_file("../graph3.txt");

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */


/**
 * Read the file to get all the info required for the exercise
 * @param name File name
 * @return TRUE is function read the file correctly, FALSE otherwise
 */
int read_file(char* name){
    input_graph.open(name);

    if(!input_graph)
        return FALSE;

    //Get biggest ID number
    long max_id;
    long min_id;
    long number_nodes;
    long number_edges;

    get_nodes_edges(input_graph, min_id, max_id, number_edges);

    number_nodes = max_id - min_id + 1;

    cout<< "Supposed number of nodes: "<<number_nodes<<endl;
    cout<< "Supposed number of edges: "<<number_edges<<endl;

    input_graph.close();

    return FALSE;
}

/**
 * Compute the number of nodes and number of edges
 * To find the total number of nodes, the functions by searches the minimum and maximum ID in the file and it considers them as the first and the last one respectively
 * If nodes between min and max are not in any edges of the file, they are considered as nodes without any connections (single component)
 * Sadly, to keep this simple and fast, it count also the copies of an edge
 * @param file File to read data
 * @param min_id
 * @param max_id
 * @param edges
 */
void get_nodes_edges(fstream &file, long &min_id, long &max_id, long &edges){
    long nodeA, nodeB;
    edges = 0;

    min_id = LONG_MAX;
    max_id = 0;

    // Count how many couples there are in the file
    // And search the MIN and MAX ID of the nodes
    while(file>>nodeA){
        file>>nodeB;

        //If it is a self loop, then do not count it as link
        if(nodeA != nodeB)
            edges++;

        //Check if the ID of the 2 nodes is a MIN or MAX
        nodeA < min_id ? min_id = nodeA : NULL;
        nodeB < min_id ? min_id = nodeB : NULL;

        nodeA > max_id ? max_id = nodeA : NULL;
        nodeB > max_id ? max_id = nodeB : NULL;
    }
}