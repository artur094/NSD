
#include <iostream>
#include "graph.h"
#include "adjacencylist.h"

using namespace std;

void diameter_from_file(string src, string dst, long number_nodes, long offset);

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
    long offset;

    contacts_size(contacts, number_nodes, number_contacts, duration, start_contact, offset);

    cout << "Size of contacts: " << endl <<
         "\tNumber of nodes = " << number_nodes << endl <<
         "\tNumber of contacts = " << number_contacts << endl <<
         "\tDuration = " << duration << endl;

    cout << "Normalizing" << endl;
    contacts_normalization(contacts, start_contact);

    cout << "Storing normalized file in 'normalized.txt' and sorted version in 'sorted_normalized.txt'" << endl;
    contacts_to_file_sorted(contacts, "normalized.txt");

    cout << "Storing intercontact distribution to 'intercontacts.txt'" << endl;
    contacts_intercontacts_distribution_to_file("intercontacts.txt", contacts, duration);

    cout << "Computing average degrees and storing them in 'avg_degree.txt'" << endl;
    contacts_degree_to_file("avg_degree.txt", "sorted_normalized.txt", number_nodes);

    cout << "Computing fraction of created and deleted links" << endl;
    contacts_links_creation_deletion_to_file("links_fractions.txt", "sorted_normalized.txt", number_nodes);

    cout << "Computing diameter and storing it in 'diameter.txt'" << endl;
    diameter_from_file("sorted_normalized.txt", "diameter.txt", number_nodes, offset);


    contacts_deinit(contacts);

    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}

void diameter_from_file(string src, string dst, long number_nodes, long offset){
    fstream in;
    fstream out;

    in.open(src);
    out.open(dst, ios::out);

    Graph* graph = graph_init(number_nodes, 0);

    long prev_time=0;
    long time;
    long nodeA;
    long nodeB;
    char c;

    while(in >> time >> nodeA >> nodeB >> c){
        nodeA -= offset;
        nodeB -= offset;

        if(prev_time != time){
            //compute diameter
            long diam = diameter(graph);
            out << prev_time << " " << diam << endl;
            prev_time = time;
        }

        if(c=='C'){
            graph_add_edge(graph, nodeA, nodeB);
            graph_add_edge(graph, nodeB, nodeA);
        }else{
            graph_remove_edge(graph, nodeA, nodeB);
            graph_remove_edge(graph, nodeB, nodeA);
        }

    }
}