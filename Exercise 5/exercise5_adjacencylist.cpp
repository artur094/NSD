#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

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

/*
 * List of important functions that must be implemented
 * Load data from a file                    DONE
 * Write data to a file
 * Print data on the standart output        DONE
 * Add edge to the graph                    DONE
 * Remove edge from the graph
 * Add node to the graph                    DONE
 * Remove node from the graph
 * Initialize graph                         DONE
 * Deinitialize graph                       DONE
 */

/* - - - - - - - - - - - - - - - - - NEW FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

void graph_init();
void graph_deinit();
int graph_load_file(char* name);
int graph_add_edge(long node, long neighbour);
int graph_add_node(long node);
void graph_print();

void node_init(long node);
void node_deinit(long node);
void node_add_edge(long neighbour);

void degree_init(long length);
void degree_deinit();


/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int load_file(char* name);
int add_edge(long node, long neighbour);
int add_node(long node);
void initialize_graph();
void deinitialize_node(long node);
void print();

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

void reset_file(fstream &file);
long get_number_nodes(fstream &file, long &offset);
long get_number_edges(fstream &file);
void set_degree_graph(fstream &file);
long get_node_index(long node);
int edge_exists(s_node* node, int neighbour);
s_node* get_node(long node);
int node_exists(int node);
int load_data(fstream &file);
int is_self_loop(int node, int neighbour);

/* - - - - - - - - - - - - - - - - - MAIN - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char** argv) {

    cout << "NDS Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        load_file(argv[1]);
    else
        load_file("graph3.txt");

    //print();

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

void reset_file(fstream &file){
    input_graph.clear();
    input_graph.seekg(0, input_graph.beg);
}

int load_file(char* name){
    input_graph.open(name);

    //Compute size of the graph and get the offset between the index for arrays and the ID of nodes
    graph.length = get_number_nodes(input_graph, graph.offset);

    //Initialize graph with the obtained size of the graph
    initialize_graph();

    //Reset file pointer
    reset_file(input_graph);

    //Compute degree of each node
    set_degree_graph(input_graph);

    //Reset file pointer
    reset_file(input_graph);

    //Load data onto the data structure
    load_data(input_graph);

    input_graph.close();
    //TODO: deinit the degree array of the graph
}

int load_data(fstream &file){
    long nodeA;
    long nodeB;

    while(file>>nodeA){
        file>>nodeB;

        //Not direct graph --> I have to add the edge to both nodes
        add_node(nodeA);
        add_node(nodeB);
        add_edge(nodeA, nodeB);
        add_edge(nodeB, nodeA);
    }

    return TRUE;
}

long get_number_nodes(fstream &file, long &offset){
    long id;
    long max_id=0;
    long min_id=LONG_MAX;

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

    offset = min_id;

    return (max_id - min_id + 1);
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
    graph_degree = new long[graph.length];
    memset(graph_degree, 0, sizeof(long)*graph.length);

    long nodeA;
    long nodeB;

    while(file>>nodeA){
        file>>nodeB;
        graph_degree[get_node_index(nodeA)]++;
        graph_degree[get_node_index(nodeB)]++;
    }
}

int add_node(long node){
    if(node_exists(node) == TRUE)
        return FALSE;

    s_node* new_node = new s_node();
    new_node->id = node;
    new_node->length = 0;
    //new_node->length = graph_degree[get_node_index(node)];
    //new_node->neighbours.resize(graph_degree[get_node_index(node)]);
    graph.nodes.at(get_node_index(node)) = new_node;

    return TRUE;
}

int add_edge(long node, long neighbour){
    s_node* tmp = get_node(node);

    if(is_self_loop(node, neighbour) == TRUE)
        return FALSE;

    if(edge_exists(tmp, neighbour) == TRUE)
        return FALSE;

    tmp->neighbours.push_back(neighbour);
    tmp->length++;


    return TRUE;
}

int node_exists(int node){
    if(graph.nodes.at(get_node_index(node)) == NULL)
        return FALSE;
    return TRUE;
}

int edge_exists(s_node *node, int neighbour){
    for(int i=0;i<node->length;i++)
        if(node->neighbours.at(i) == neighbour)
            return TRUE;

    return FALSE;
}

int is_self_loop(int node, int neighbour){
    return node == neighbour ? TRUE: FALSE;
}

void initialize_graph(){
    graph.nodes.resize(graph.length);
    for(int i=0;i<graph.length;i++){
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
    s_node* tmp = graph.nodes.at(get_node_index(node));

    tmp->neighbours.clear();

    delete tmp;
}

long get_node_index(long node){
    return (node-graph.offset);
}

s_node* get_node(long node){
    return graph.nodes.at(get_node_index(node));
}

void print(){
    cout << "Graph:"<<endl;
    for(int i=0;i<graph.length;i++){
        s_node* node = graph.nodes.at(i);
        //cout << "Considering node "<<node->id<<endl;
        for(int j=0;j<node->length;j++){
            if(node->id < node->neighbours.at(j))
                cout << node->id << " " << node->neighbours.at(j) << endl;
        }
    }
}


