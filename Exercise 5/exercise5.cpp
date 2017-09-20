#include <iostream>
#include <fstream>
#include <ctime>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

struct s_node{
    long id;
    long length;
    vector<long> neighbours;
};
typedef struct s_node s_node;

struct s_graph{
    long offset;
    long length;
    vector<s_node*> nodes;
};
typedef struct s_graph s_graph;

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

long *graph_degree = NULL;
s_graph graph;
fstream input_graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name);
long get_number_nodes(fstream &file);
long get_number_edges(fstream &file);
void set_degree_graph(fstream &file);
void get_node_index(long node);

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
    long max_id;
    long min_id;
    long index_offset;
    long number_nodes;

    get_number_nodes(input_graph, min_id, max_id);

    number_nodes = max_id - min_id + 1;
    index_offset = min_id;

    graph.offset = index_offset;

    initialize_graph(number_nodes);

    //Set file pointer to the beginning of the file
    input_graph.clear();
    input_graph.seekg(0, input_graph.beg);

    input_graph.close();
}

int insert_data_from_file(fstream &file){

}

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

int add_node(long node){
    if(node_exists(node) == TRUE)
        return FALSE;

    s_node* new_node = new s_node();
    new_node->id = node;
    //new_node->neighbours.resize(graph_degree[get_node_index(node)]);
    graph.nodes.at(new_node);

    return TRUE;
}

int add_edge(long node, long neighbour){
    s_node* tmp = get_node(node);

    if(edge_exists(tmp, neighbour) == TRUE)
        return FALSE;

    tmp->neighbours.push_back(neighbour);

    return TRUE;
}

int node_exists(int node){
    if(graph.nodes.at(get_node_index(node)) != NULL)
        return FALSE;
    return TRUE;
}

int edge_exists(s_node node, int neighbour){
    for(int i=0;i<node.length;i++)
        if(node.neighbours.at(i) == neighbour)
            return TRUE;

    return FALSE;
}

int is_self_loop(int node, int neighbour){
    return node == neighbour ? TRUE: FALSE;
}

void initialize_graph(long length){
    graph.length = length;
    graph.nodes.resize(length);
    for(int i=0;i<length;i++){
        graph.nodes.at(i) = NULL;
    }
}

void deinitialize_graph(){
    for(int i=0;i<graph.length;i++){
        deinitialize_node(graph.nodes.at(i)->id);
        graph.nodes.at(i) = NULL;
    }
}

void deinitialize_node(long node){
    s_node tmp = graph.nodes.at(get_node_index(node));

    for(int i=0;i<tmp.length;i++){
        delete tmp.neighbours.at(i);
        tmp.neighbours.at(i) = NULL;
    }

    delete tmp;
}

long get_node_index(long node){
    return (node-graph.offset);
}

s_node* get_node(long node){
    return graph.nodes.at(get_node_index(node));
}


