//
// Created by Ivan Morandi on 04/01/18.
//

#include <iostream>
#include "graph.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);
    long iterations = 100;

    cout << "Loading graph..."<<endl;

    Contacts* contacts;

    if(argc < 2){
        cout << "Missing name!" << endl;
        cout << "Write ./a.out <graph_name>" << endl;
        exit(1);
    }


    contacts = contacts_load(argv[1]);

    long number_nodes;
    long number_contacts;
    long duration;
    long start_contact;

    contacts_size(contacts, number_nodes, number_contacts, duration, start_contact);

    cout << "Size of contacts: " << endl <<
            "\tNumber of nodes = " << number_nodes << endl <<
            "\tNumber of contacts = " << number_contacts << endl <<
            "\tDuration = " << duration << endl;


    contacts_deinit(contacts);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}