//
// Created by Ivan Morandi on 04/01/18.
//

#ifndef NSD_GRAPH_H
#define NSD_GRAPH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "graph.h"

using namespace std;

struct Contact{
    long node1;
    long node2;
    long start;
    long end;
};
typedef struct Contact Contact;

struct InterContact{
    long node1;
    long node2;
    long times;
    double ttl;
};
typedef struct InterContact InterContact;

typedef vector<Contact*> Contacts;

typedef vector<InterContact*> InterContacts;

InterContact* intercontact_init();
void intercontact_deinit(InterContact* interContact);
InterContacts* intercontacts_init();
void intercontacts_deinit(InterContacts* interContacts);
InterContacts* intercontacts_from_contacts(Contacts* contacts);
void intercontacts_print(InterContacts* interContacts);

Contact* contact_init();
void contact_deinit(Contact* contact);

Contacts* contacts_init();
void contacts_deinit(Contacts* contacts);
Contacts* contacts_load(char* filename);
void contacts_size(Contacts* contacts, long &number_nodes, long &number_contacts, long &duration, long &start, long &offset);
void contacts_normalization(Contacts* contacts, long offset);
void contacts_to_file(Contacts* contacts, char* filename);
void contacts_to_file_sorted(Contacts* contacts, string filename);
void contacts_degree_to_file(string filename, string filename_sorted, long number_nodes);
void contacts_links_creation_deletion_to_file(string filename, string filename_sorted, long number_nodes);
void contacts_intercontacts_distribution_to_file(string filename, Contacts* contacts, long duration);

void markovian_to_file(string filename, long number_nodes, long iterations, double p, double d);
void markovian_proposal_to_file(string filename, long number_nodes, long iterations, double creation_prob, double deletion_mean, double deletion_variance );

#endif //NSD_GRAPH_H
