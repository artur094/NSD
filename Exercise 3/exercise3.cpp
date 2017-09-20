#include <iostream>
#include <fstream>
#include <ctime>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

long *graph_degree = NULL;
fstream input_graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name);
void get_number_nodes(fstream &file, long &min_id, long &max_id);
void set_degree_graph(fstream &file, long offset);

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
    long index_offset;

    get_number_nodes(input_graph, min_id, max_id);

    number_nodes = max_id - min_id + 1;
    index_offset = min_id;

    //Set file pointer to the beginning of the file
    input_graph.clear();
    input_graph.seekg(0, input_graph.beg);

    cout<< "Supposed number of Nodes: "<<number_nodes<<endl;

    graph_degree = new long[number_nodes];

    memset(graph_degree, 0, sizeof(long)*number_nodes);

    /*for(int i=0;i<number_nodes;i++){
        graph_degree[i] = 0;
    }*/

    set_degree_graph(input_graph, index_offset);

    long max_degree = 0;

    for(int i=0;i<number_nodes;i++){
        max_degree < graph_degree[i] ? max_degree = graph_degree[i] : NULL;
        cout << "d( "<<i + index_offset<<" ) = "<<graph_degree[i]<<endl;
    }

    cout << "Max degree: "<<max_degree<<endl;

    delete(graph_degree);
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

void set_degree_graph(fstream &file,long offset){
    if(graph_degree == NULL)
        return;

    long nodeA;
    long nodeB;

    while(file>>nodeA){
        file>>nodeB;
        graph_degree[nodeA - offset]++;
        graph_degree[nodeB - offset]++;
    }
}
