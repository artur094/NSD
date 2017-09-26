//
// Created by Ivan Morandi on 26/09/2017.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "adjacencylist.h"

using namespace std;

int main(int argc, char** argv) {

    cout << "NSD Course:" << endl;
    long start = time(NULL);

    if(argc > 1)
        graph_load_file(argv[1]);
    else
        graph_load_file("graph3.txt");

    //graph_print();

    graph_deinit();

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}
