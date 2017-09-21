#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <utility>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

long array_length;
//long array_offset;
vector<pair<long,long> > edges;
fstream input_graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name);
void get_number_nodes(fstream &file, long &min_id, long &max_id);
void read_data(fstream &file);
void write_data(char* file_name);
void add_edge(long nodeA, long nodeB);
void print();

/* - - - - - - - - - - - - - - - - - MAIN - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char** argv) {

    cout << "NDS Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        read_file(argv[1]);
    else
        read_file("../graph3.txt");

    cout << "Edges inserted into the array" << endl;

    print();

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

    get_number_nodes(input_graph, min_id, max_id);

    //Set file pointer to the beginning of the file
    input_graph.clear();
    input_graph.seekg(0, input_graph.beg);

    read_data(input_graph);

    input_graph.close();
}

void read_data(fstream &file){
    long nodeA;
    long nodeB;

    while(file>>nodeA){
        file>>nodeB;

        edges.push_back(make_pair(nodeA, nodeB));
    }
}

void write_data(char* file_name){
    fstream output_graph;
    long nodeA;
    long nodeB;

    output_graph.open(file_name, ios::out);
    cout << file_name << " file opened"<<endl;

    //TODO: finish write_data

    output_graph.close();

    cout << file_name << " file closed"<<endl;
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

void print(){
    long nodeA;
    long nodeB;
    pair<long, long> edge;
    vector<pair<long,long> >::iterator it;

    for(it=edges.begin() ; it < edges.end(); it++ ) {
        edge = *it;
        cout << edge.first << " " << edge.second << endl;
    }
}