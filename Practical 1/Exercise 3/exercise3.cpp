#include <iostream>
#include <fstream>
#include <ctime>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

long *graph_degree = NULL;
long number_nodes = 0;
long offset;
fstream input_graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name);
void file_reset(fstream &file);
void graph_degree_print(long offset);
void graph_degree_init(long length);
void graph_degree_deinit();
void get_number_nodes(fstream &file, long &min_id, long &max_id);
void set_degree_graph(fstream &file, long offset);

/* - - - - - - - - - - - - - - - - - MAIN - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char** argv) {

    cout << "NSD Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        read_file(argv[1]);
    else
        read_file("../graph3.txt");

    graph_degree_print(offset);

    graph_degree_deinit();

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

/**
 * Initialize the array for counting the node degrees with the given length
 * @param length Number of nodes
 */
void graph_degree_init(long length){
    graph_degree = new long[length];
    memset(graph_degree, 0, sizeof(long)*length);
}

/**
 * Free the memory used by the array for counting the node degrees
 */
void graph_degree_deinit(){
    delete[] graph_degree;
}

/**
 * Print the degree of each node
 * @param offset Offset to calculate the nodeID from the index
 */
void graph_degree_print(long offset){
    if(graph_degree == NULL)
        return;

    for(int i=0;i<number_nodes;i++){
        long node = i + offset;
        cout << "Degree("<<node<<") = "<<graph_degree[i]<<endl;
    }
}

/**
 * Read the file and fill the array for the node degrees
 * @param name File name
 * @return TRUE if everything run correctly, FALSE otherwise
 */
int read_file(char* name){
    input_graph.open(name);

    if(!input_graph)
        return FALSE;

    //Get biggest ID number
    long max_id;
    long min_id;

    get_number_nodes(input_graph, min_id, max_id);

    number_nodes = max_id - min_id + 1;
    offset = min_id;

    cout<< "Supposed number of Nodes: "<<number_nodes<<endl;

    graph_degree_init(number_nodes);

    set_degree_graph(input_graph, offset);

    input_graph.close();

    return TRUE;
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
 * Fill the array for the node degrees by reading the given file. It automatically set the file's pointer to the beginning of the file
 * @param file file stream
 * @param offset offset to obtain the index from the node ID
 */
void set_degree_graph(fstream &file, long offset){
    if(graph_degree == NULL)
        return;

    file_reset(file);

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
 * Reset file pointer to the beginning
 * @param file
 */
void file_reset(fstream &file){
    file.clear();
    file.seekg(0, file.beg);
}