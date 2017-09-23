#include <iostream>
#include <fstream>
#include <ctime>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

long node_deg_zero = 0;
double density = 0;
double avg_degree = 0.;
long min_degree = LONG_MAX;
long max_degree = 0;
long number_nodes = 0;
long number_edges = 0;

long *graph_degree = NULL;
fstream input_graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name);
void get_nodes_edges(fstream &file, long &min_id, long &max_id, long &edges);
void set_degree_graph(fstream &file, long offset);
void compute_data();
void graph_degree_init(long length);
void graph_degree_deinit();
void file_reset(fstream &file);


/* - - - - - - - - - - - - - - - - - MAIN - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char** argv) {

    cout << "NDS Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        read_file(argv[1]);
    else
        read_file("../graph3.txt");

    long end = time(NULL);

    cout << "Number of nodes with degree 0: "<<node_deg_zero<<endl;
    cout << "Density of the graph: "<<density<<endl;
    cout << "Minimum Degree: "<<min_degree<<endl;
    cout << "Maximum Degree: "<<max_degree<<endl;
    cout << "Average Degree: "<<avg_degree<<endl;

    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;

    return 0;
}

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

/**
 * Load all data and sets all variables in the correct way
 * @param name
 * @return
 */
int read_file(char* name){
    input_graph.open(name);

    if(!input_graph)
        return FALSE;

    long max_id;
    long min_id;
    long offset;

    get_nodes_edges(input_graph, min_id, max_id, number_edges);

    number_nodes = max_id - min_id + 1;
    offset = min_id;

    graph_degree_init(number_nodes);
    set_degree_graph(input_graph, offset);

    compute_data();

    input_graph.close();
    graph_degree_deinit();

    return TRUE;
}

/**
 * Get number of nodes and edges
 * @param file
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
    while(file>>nodeA){
        file>>nodeB;
        edges++;

        nodeA < min_id ? min_id = nodeA : NULL;
        nodeB < min_id ? min_id = nodeB : NULL;

        nodeA > max_id ? max_id = nodeA : NULL;
        nodeB > max_id ? max_id = nodeB : NULL;
    }
}

/**
 * Fill the array with node degrees
 * @param file
 * @param offset
 */
void set_degree_graph(fstream &file,long offset){
    file_reset(file);
    if(graph_degree == NULL)
        return;

    long nodeA;
    long nodeB;

    while(file>>nodeA){
        file>>nodeB;
        if(nodeA != nodeB) {
            graph_degree[nodeA - offset]++;
            graph_degree[nodeB - offset]++;
        }
    }
}

/**
 * Compute the required data: min/max/avg degree and density
 */
void compute_data(){
    long count = 0;
    for(int i = 0; i<number_nodes; i++){
        avg_degree += graph_degree[i];

        graph_degree[i] == 0 ? node_deg_zero++ : NULL;

        graph_degree[i] > max_degree ? max_degree = graph_degree[i] : NULL;
        graph_degree[i] < min_degree ? min_degree = graph_degree[i] : NULL;
    }
    avg_degree /= (double)number_nodes;
    density = (2*(double)number_edges) / ((double)number_nodes * ((double)number_nodes-1.));
}


/**
 * Initialize the array for the degree of each node
 * @param length
 */
void graph_degree_init(long length){
    graph_degree = new long[length];
    memset(graph_degree, 0, sizeof(long)*length);
}

/**
 * Free the memory used by the array for the degree of each node
 */
void graph_degree_deinit(){
    delete[] graph_degree;
}

/**
 * Restore the file pointer to the beginning
 * @param file
 */
void file_reset(fstream &file){
    file.clear();
    file.seekg(0, file.beg);
}