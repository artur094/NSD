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
         "\tDuration = " << duration << endl <<
         "\tStart time = " << start_contact << endl;

    cout << "Computing intercontacts" << endl;
    InterContacts* interContacts = intercontacts_from_contacts(contacts);
    intercontacts_print(interContacts);

    intercontacts_deinit(interContacts);


    cout << "Storing normalized dataset" << endl;
    contacts_normalization(contacts, start_contact);

    //Creates 2 file: "normalized.txt" and "sorted_normalized.txt" where the second one is in the form: time, nodeA, nodeB, C/S
    cout << "Storing normalized dataset with contacts ordered and splitted" << endl;
    contacts_to_file_sorted(contacts, "normalized.txt");

    cout << "Computing average degrees" << endl;
    contacts_degree_to_file("avg_degree.txt", "sorted_normalized.txt", number_nodes);

    cout << "Computing fraction of created and deleted links" << endl;
    contacts_links_creation_deletion_to_file("links_fractions.txt", "sorted_normalized.txt", number_nodes);


    contacts_deinit(contacts);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}