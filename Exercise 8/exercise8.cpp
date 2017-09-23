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
int graph_degree_write_file(char *name, long offset);
void graph_degree_init(long length);
void graph_degree_deinit();
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

    graph_degree_write_file("output.txt", offset);

    graph_degree_deinit();

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

void graph_degree_init(long length){
    graph_degree = new long[length];
    memset(graph_degree, 0, sizeof(long)*length);
}

void graph_degree_deinit(){
    delete[] graph_degree;
}

int graph_degree_write_file(char *name, long offset){
    if(graph_degree == NULL)
        return FALSE;

    fstream output;
    output.open(name, ios::out);

    if(!output) {
        cerr << "Error while opening the output file"<<endl;
        return FALSE;
    }

    for(int i=0;i<number_nodes;i++){
        long node = i + offset;
        output << graph_degree[i] << " " << node << endl;
    }

    output.close();
    return TRUE;
}


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

void file_reset(fstream &file){
    file.clear();
    file.seekg(0, file.beg);
}