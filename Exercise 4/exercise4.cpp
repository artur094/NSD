#include <iostream>
#include <fstream>
#include <ctime>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

struct node{
    long id;
    long first_neighbour;
};
typedef struct node node;

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

long *graph = NULL;
fstream input_graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name);
long get_number_nodes(fstream &file);
long get_number_edges(fstream &file);
void set_degree_graph(fstream &file);
node* initialize_node_array(int length);
void destroy_node_array(node* array);

/* - - - - - - - - - - - - - - - - - MAIN - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char** argv) {

    cout << "NDS Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        read_file(argv[1]);
    else
        read_file("graph3.txt");

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name){
    input_graph.open(name);

    //Get biggest ID number
    long max_id = get_number_nodes(input_graph);

    //Set file pointer to the beginning of the file
    input_graph.clear();
    input_graph.seekg(0, input_graph.beg);

    //Get number of edges
    long number_edges = get_number_edges(input_graph);

    input_graph.clear();
    input_graph.seekg(0, input_graph.beg);

    cout<< "Supposed number of nodes: "<<max_id<<endl;
    cout<< "Supposed number of edges: "<<number_edges<<endl;

    graph_degree = new long[max_id];

    set_degree_graph(input_graph);

    for(int i=0;i<max_id;i++){
        cout << "d( "<<i<<" ) = "<<graph_degree[i]<<endl;
    }

    delete(graph_degree);
    input_graph.close();
}

long get_number_nodes(fstream &file){
    long id;
    long max_id=0;

    //Find biggest number
    while(file>>id)
        if(id>max_id)
            max_id = id;

    return max_id;
}

long get_number_edges(fstream &file){
    long count = 0;
    long value;
    // Count how many couples there are in the file
    while(file>>value){
        file>>value;
        count++;
    }
    return count;
}

void set_degree_graph(fstream &file){
    if(graph_degree == NULL)
        return;

    long nodeA;
    long nodeB;

    while(file>>nodeA){
        file>>nodeB;

        graph_degree[nodeA]++;
        graph_degree[nodeB]++;
    }
}


int add_node(int node){

}

int add_edge(int node, int neighbour){

}

int check_node_existence(int node){

}

int check_edge_existence(int node, int neighbour){

}

int is_self_loop(int node, int neighbour){
    return node == neighbour ? TRUE: FALSE;
}

node* initialize_node_array(int length){
    node* array = new node[length];
    return array;
}

void destroy_node_array(node* array){
    delete array;
}

