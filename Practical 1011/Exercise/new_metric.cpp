//
// Created by Ivan Morandi on 04/01/18.
//

#include <iostream>
#include <fstream>
#include "graph.h"
#include "adjacencylist.h"

using namespace std;

void diameter_from_file(string src, string dst, long number_nodes, long offset);
void size(string src, long &number_nodes, long &offset);

int main(int argc, char** argv) {
    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);
    long iterations = 100;


    if(argc < 3){
        cout << "Missing name!" << endl;
        cout << "Write ./a.out <graph_name_normalized> <output>" << endl;
        exit(1);
    }

    long number_nodes;
    long number_contacts;
    long offset;
    long duration;
    long start_contact;

    cout << "Computing size" << endl;
    size(argv[1], number_nodes, offset);

    cout << "Computing diameters" << endl;
    diameter_from_file(argv[1], argv[2], number_nodes, offset);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

void size(string src, long &number_nodes, long &offset){
    fstream in;
    in.open(src);

    long time, nodeA, nodeB;
    char c;

    long max_id=LONG_MIN;
    long min_id=LONG_MAX;

    while(in >> time >> nodeA >> nodeB >> c) {
        if(nodeA > max_id)
            max_id = nodeA;
        if(nodeA < min_id)
            min_id = nodeA;
        if(nodeB > max_id)
            max_id = nodeB;
        if(nodeB < min_id)
            min_id = nodeB;
    }

    offset = min_id;
    number_nodes = max_id-min_id+1;
}