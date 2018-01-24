//
// Created by Andrea Ferlini on 17/10/2017.
//

#ifndef PRACTICAL5_LOADGRAPH_H
#define PRACTICAL5_LOADGRAPH_H

#include "Node.h"
#include <fstream>
#include <ctime>

using namespace std;

class AdjacencyList{
public:
    Node* nodes;                        // array of the nodes in the greaph (ID, degree, pointer to the beginning of list of neighbours)
    /// DEFAULT CONSTRUCTOR
    AdjacencyList();

    /// SPECIFIC CONSTRUCTOR (directly load graph)
    AdjacencyList(string filename, bool debug);

    /// DESTRUCTOR
    ~AdjacencyList();

    unsigned int getNumNodes();
    unsigned int getNumEdges();

    int loadFromFile(string filename, bool debug);

    /// PRINT METHOD
    void print(bool debug);

    Node* getNeighbour(unsigned int node_id, unsigned int neighbour_number);

protected:
    Node** neighbours_list;             // array of Node pointers to store contiguously all the neignbours
    unsigned int cumulative_degree;     // sum of all degree of the nodes (probabily edge x2). SIZE OF neighbour_list
    unsigned int num_edges;
    unsigned int num_nodes;
    //Node* nodes;                        // array of the nodes in the greaph (ID, degree, pointer to the beginning of list of neighbours)


private:
    void countDegree(fstream& graph, bool debug);           // count degree for each node from a graph file stream
    int measureGraph(fstream& graph, bool debug);           // count degree for each node from a graph file stream
    void storeNeighbours(fstream& graph, bool debug);
};

/// FUNCTIONS


#endif //PRACTICAL5_LOADGRAPH_H
