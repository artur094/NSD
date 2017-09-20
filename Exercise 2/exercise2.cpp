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
void get_number_nodes(fstream &file, long &min_id, long &max_id);
long get_number_edges(fstream &file);
void get_nodes_edges(fstream &file, long &min_id, long &max_id, long &edges);

/* - - - - - - - - - - - - - - - - - MAIN - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char** argv) {

    cout << "NDS Course:" << endl;
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

int read_file(char* name){
    input_graph.open(name);

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
}

/**
 * It find the total number of nodes by searching the minimum and maximum ID in the file and it considers them as the first and the last one respectively
 * If nodes between min and max are not in any edges of the file, they are considered as nodes without any connections (single component)
 */
void get_number_nodes(fstream &file, long &min_id, long &max_id){
    long id;
    max_id=0;
    min_id=LONG_MAX;

    //Find biggest and the smallest ID
    //Then it consider the smallest one as the first node
    //And the biggest one as the last one
    //All nodes thar are not showed in the list are considered as a single node not connected
    while(file>>id){
        if(id>max_id)
            max_id = id;
        if(id<min_id)
            min_id = id;
    }
}

/**
 * It find the total number of edges in the file.
 * Sadly, to keep this simple and fast, it count also the copies of a single edge
 */
long get_number_edges(fstream &file){
    long count = 0;
    long nodeA, nodeB;
    // Count how many couples there are in the file
    while(file>>nodeA){
        file>>nodeB;
        count++;
    }
    return count;
}

void get_nodes_edges(fstream &file, long &min_id, long &max_id, long &edges){
    long nodeA, nodeB;
    edges = 0;

    min_id = LONG_MAX;
    max_id = 0;

    // Count how many couples there are in the file
    while(file>>nodeA){
        file>>nodeB;
        edges++;

        nodeA < min_id ? min_id = nodeA : NULL;
        nodeB < min_id ? min_id = nodeB : NULL;

        nodeA > max_id ? max_id = nodeA : NULL;
        nodeB > max_id ? max_id = nodeB : NULL;
    }
}