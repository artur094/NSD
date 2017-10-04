#include <iostream>
#include <fstream>
#include <ctime>

#define TRUE 0
#define FALSE 1

using namespace std;

/* - - - - - - - - - - - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - */

long matrix_length;
long matrix_offset;
int** matrix;
fstream input_graph;

/* - - - - - - - - - - - - - - - - - FUNCTIONS DECLARATION - - - - - - - - - - - - - - - - - - - - */

int read_file(char* name);
void write_file(char* file_name);
void add_edge(long nodeA, long nodeB);
void initialize_matrix();
void deinitialize_matrix();
void print();

/* - - - - - - - - - - - - - - - - - AUXILIARY FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

void get_number_nodes(fstream &file, long &min_id, long &max_id);
void read_data(fstream &file);

/* - - - - - - - - - - - - - - - - - MAIN - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char** argv) {

    cout << "NSD Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        read_file(argv[1]);
    else
        read_file("../graph3.txt");

    cout << "Data inserted into the matrix" << endl;

    //print();

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

/* - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - - - - */

/**
 * Initialize and configure all the variables and matrix using data in the file
 * @param name
 * @return
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

    get_number_nodes(input_graph, min_id, max_id);

    matrix_length = max_id - min_id + 1;
    matrix_offset = min_id;

    initialize_matrix();

    //Set file pointer to the beginning of the file
    input_graph.clear();
    input_graph.seekg(0, input_graph.beg);

    read_data(input_graph);

    input_graph.close();

    return TRUE;
}

/**
 * Load data from the given file to the matrix
 * @param file
 */
void read_data(fstream &file){
    long nodeA;
    long nodeB;

    while(file>>nodeA){
        file>>nodeB;

        add_edge(nodeA, nodeB);
    }
}

/**
 * Write the edges contained in the matrix to the input file
 * @param file_name
 */
void write_file(char* file_name){
    fstream output_graph;
    long nodeA;
    long nodeB;

    output_graph.open(file_name, ios::out);
    cout << file_name << " file opened"<<endl;

    for(int i=0;i<matrix_length;i++){
        for(int j=i+1;j<matrix_length;j++){
            if(matrix[i][j] > 0) {
                nodeA = i+matrix_offset;
                nodeB = j+matrix_offset;

                output_graph<<nodeA<<" "<<nodeB<<endl;
                cout<<"Writing: "<<nodeA<<" "<<nodeB<<endl;
            }
        }
    }

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

/**
 * Add edge in both directions to the matrix (A-->B and B-->A)
 * @param nodeA
 * @param nodeB
 */
void add_edge(long nodeA, long nodeB){
    if(nodeA == nodeB)
        return;

    matrix[nodeA-matrix_offset][nodeB-matrix_offset] = 1;
    matrix[nodeB-matrix_offset][nodeA-matrix_offset] = 1;
}

/**
 * Initialize the matrix
 */
void initialize_matrix(){
    matrix = new int*[matrix_length];

    for(int i=0;i<matrix_length;i++) {
        matrix[i] = new int[matrix_length];
        memset(matrix[i], 0, sizeof(int)*matrix_length);
    }
}

/**
 * Free the memory used by the matrix
 */
void deinitialize_matrix(){
    for(int i=0;i<matrix_length;i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

/**
 * Print all the edges contained in the adjacency matrix
 */
void print(){
    long nodeA;
    long nodeB;

    for(int i=0;i<matrix_length;i++){
        for(int j=i+1;j<matrix_length;j++){
            if(matrix[i][j] > 0) {
                nodeA = i+matrix_offset;
                nodeB = j+matrix_offset;

                cout<<nodeA<<" "<<nodeB<<endl;
            }
        }
    }
}