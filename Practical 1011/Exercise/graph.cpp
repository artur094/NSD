//
// Created by Ivan Morandi on 04/01/18.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <random>
#include "graph.h"

Contact* contact_init(){
    Contact* contact = new Contact();
    return contact;
}

void contact_deinit(Contact* contact){
    if(contact == NULL)
        return;
    delete(contact);
}

InterContact* intercontact_init(){
    InterContact* interContact = new InterContact();
    return interContact;
}

void intercontact_deinit(InterContact* interContact){
    if(interContact == NULL)
        return;
    delete(interContact);
}

InterContacts* intercontacts_init(){
    InterContacts* interContacts = new InterContacts();
    return interContacts;
}

void intercontacts_deinit(InterContacts* interContacts){
    if(interContacts == NULL)
        return;

    for (int i = 0; i < interContacts->size(); ++i) {
        intercontact_deinit(interContacts->at(i));
    }
    delete(interContacts);
}

Contacts* contacts_init(){
    Contacts* contacts = new Contacts();
    return contacts;
}

void contacts_deinit(Contacts* contacts){
    if(contacts == NULL)
        return;

    for (int i = 0; i < contacts->size(); ++i) {
       contact_deinit( contacts->at(i) );
    }
    delete contacts;
}

Contacts* contacts_load(char* filename){
    fstream in;
    in.open(filename);

    Contacts* contacts = contacts_init();

    long nodeA;
    long nodeB;
    long start;
    long end;

    while(in >> nodeA >> nodeB >> start >> end){
        Contact* contact = contact_init();
        contact->node1 = nodeA;
        contact->node2 = nodeB;
        contact->start = start;
        contact->end = end;

        contacts->push_back(contact);
    }

    in.close();

    return contacts;
}

void contacts_size(Contacts* contacts, long &number_nodes, long &number_contacts, long &duration, long &start_time, long &offset){
    if (contacts == NULL)
        return;

    number_nodes = 0;
    number_contacts = 0;
    duration = 0;

    if(contacts->empty()){
        return;
    }

    long max_id = 0;
    long min_id = LONG_MAX;
    long start = LONG_MAX;
    long end = LONG_MIN;

    for (int i = 0; i < contacts->size(); ++i) {
        Contact* contact = contacts->at(i);

        if(contact->node1 > max_id)
            max_id = contact->node1;
        if(contact->node1 < min_id)
            min_id = contact->node1;

        if(contact->node2 > max_id)
            max_id = contact->node2;
        if(contact->node2 < min_id)
            min_id = contact->node2;


        if(contact->start < start)
            start = contact->start;
        if(contact->end > end)
            end = contact->end;

        number_contacts++;

    }

    number_nodes = max_id - min_id + 1;
    duration = end - start + 1;
    start_time = start;
    offset = min_id;
}

InterContacts* intercontacts_from_contacts(Contacts* contacts){
    if(contacts == NULL)
        return NULL;

    if(contacts->size() <= 0)
        return NULL;

    InterContacts* interContacts = intercontacts_init();

    InterContact* interContact = intercontact_init();
    interContact->node1 = contacts->at(0)->node1;
    interContact->node2 = contacts->at(0)->node2;
    interContact->times = 1;
    interContacts->push_back(interContact);

    for (int i = 1; i < contacts->size(); ++i) {
        InterContact* last = interContacts->at(interContacts->size() - 1);
        if(last->node1 == contacts->at(i)->node1 && last->node2 == contacts->at(i)->node2){
            last->times++;
        }else{
            InterContact* interContact = intercontact_init();
            interContact->node1 = contacts->at(i)->node1;
            interContact->node2 = contacts->at(i)->node2;
            interContact->times = 1;
            interContacts->push_back(interContact);
        }
    }

    return interContacts;
}

void contacts_intercontacts_distribution_to_file(string filename, Contacts* contacts, long duration){
    string filename_cum = "cumulative_"+filename;
    fstream out;
    fstream out_cum;
    out.open(filename, ios::out);
    out_cum.open(filename_cum, ios::out);

    long *distribution = new long[duration];
    memset(distribution, 0, duration*sizeof(long));
    long max_intercontact = 0;
    long intercontact = 0;
    long total_sum = 0;
    long sum = 0;

    for (int i = 0; i < contacts->size()-1; ++i) {

        if(contacts->at(i)->node1 == contacts->at(i+1)->node1 && contacts->at(i)->node2 == contacts->at(i+1)->node2){
            intercontact = contacts->at(i+1)->start - contacts->at(i)->end - 1;

            //cout << "Intercontact " << contacts->at(i)->node1 << " " << contacts->at(i)->node2 << " with duration " << intercontact << endl;

            distribution[intercontact]++;
            total_sum++;

            if(max_intercontact < intercontact)
                max_intercontact = intercontact;
        }
    }

    for (int j = 0; j <= max_intercontact; ++j) {
        if(distribution[j] <= 0)
            continue;

        sum += distribution[j];
        out << j << " " << distribution[j] << endl;
        out_cum << j << " " << sum << endl;
    }


    out.close();
    delete[] distribution;
}

void intercontacts_print(InterContacts* interContacts){
    if(interContacts == NULL)
        return;

    cout << "NodeA NodeB: Number of contacts" << endl;
    for (int i = 0; i < interContacts->size(); ++i) {
        cout << interContacts->at(i)->node1 << " " << interContacts->at(i)->node2 << ": " << interContacts->at(i)->times << endl;
    }
}

void contacts_normalization(Contacts* contacts, long offset){
    if(contacts == NULL)
        return;

    for (int i = 0; i < contacts->size(); ++i) {
        contacts->at(i)->start -= offset;
        contacts->at(i)->end -= offset;
    }
}

void contacts_to_file(Contacts* contacts, char* filename){
    if(contacts == NULL)
        return;

    fstream out;
    out.open(filename, ios::out);

    for (int i = 0; i < contacts->size(); ++i) {
        out << contacts->at(i)->node1 << " " << contacts->at(i)->node2 << " " << contacts->at(i)->start << " " << contacts->at(i)->end << endl;
    }

    out.close();
}

void contacts_to_file_sorted(Contacts* contacts, string filename){
    if(contacts == NULL)
        return;

    fstream out;
    out.open(filename, ios::out);

    for (int i = 0; i < contacts->size(); ++i) {
        out <<  contacts->at(i)->start << " " << contacts->at(i)->node1 << " " << contacts->at(i)->node2 << " C" << endl;
        out << contacts->at(i)->end << " " << contacts->at(i)->node1 << " " << contacts->at(i)->node2 << " S" << endl;
    }

    out.close();

    string sorted_file = "sorted_" + filename;

    string command = "sort -n -k1 " + filename + " > " + sorted_file;

    system(command.c_str());
}

void contacts_degree_to_file(string filename, string filename_sorted, long number_nodes){
    fstream in, out;
    in.open(filename_sorted);
    out.open(filename, ios::out);

    long time;
    long nodeA;
    long nodeB;
    char c;

    long degree = 0;
    long last_time = 0;

    while(in >> time >> nodeA >> nodeB >> c){
        if(time != last_time){
            out << last_time << " " << (double)degree / (double)number_nodes << endl;
            last_time = time;
        }
        if(c == 'C'){
            degree++;
        }else{
            degree--;
        }
    }

    out << last_time << " " << degree / number_nodes << endl;

    in.close();
    out.close();
}

//TODO: fix fraction calculation when there are deletion of links created at the same time: what to do with changing degree?
void contacts_links_creation_deletion_to_file(string filename, string filename_sorted, long number_nodes){
    fstream in, out;
    in.open(filename_sorted);
    out.open(filename, ios::out);

    long time;
    long nodeA;
    long nodeB;
    char c;

    long degree = 0;
    long number_created = 0;
    long number_deleted = 0;
    long last_time = 0;
    long max_number_edges = number_nodes * (number_nodes - 1) / 2;

    double fraction_created = 0;
    double fraction_deleted = 0;

    long min_degree = 0;
    long max_degree = 0;

    while(in >> time >> nodeA >> nodeB >> c){
        if(time != last_time){
            if(min_degree == max_number_edges)
                fraction_created = -1;
            else
                fraction_created = (double)number_created / (double)(max_number_edges - min_degree);

            if(max_degree == 0)
                fraction_deleted = -1;
            else
                fraction_deleted = (double) number_deleted / (double) max_degree;
            //<time> <fraction created links> <fraction deleted links>
            out << last_time << " " << fraction_created << " " << fraction_deleted << endl;
            last_time = time;
            number_created=0;
            number_deleted=0;
            max_degree = degree;
            min_degree = degree;
        }
        if(c == 'C'){
            number_created++;
            degree++;
        }else{
            number_deleted++;
            degree--;
        }

        if(degree > max_degree)
            max_degree = degree;
        if(degree < min_degree)
            min_degree = degree;
    }

    if(min_degree == max_number_edges)
        fraction_created = -1;
    else
        fraction_created = (double)number_created / (double)(max_number_edges - min_degree);

    if(max_degree == 0)
        fraction_deleted = -1;
    else
        fraction_deleted = (double) number_deleted / (double) max_degree;

    out << last_time << " " << fraction_created << " " << fraction_deleted << endl;

    in.close();
    out.close();
}

//p and d can be seen as the fractions of created and deleted links
//what else can I use?
//gaussian based on time?
//deletion based on gaussian
//creation based on poisson

//about metrics: duration of created links
void markovian_to_file(string filename, long number_nodes, long iterations, double p, double d){
    //Initialization
    fstream out;
    out.open("tmp.txt", ios::out);

    InterContacts* created = intercontacts_init();
    InterContacts* non_created = intercontacts_init();

    srand(time(NULL));

    double prob = 0;

    for (int i = 0; i < number_nodes; ++i) {
        for (int j = i+1; j < number_nodes; ++j) {
            InterContact* interContact = intercontact_init();
            interContact->node1 = i;
            interContact->node2 = j;
            non_created->push_back(interContact);
        }
    }

    //Markovian
    for (int i = 0; i < iterations; ++i) {
        //creates new links
        for (int j = 0; j < non_created->size(); ++j) {
            prob = (double)(rand()%100)/100.0;
            //create links
            if(prob < p){
                //cout << i << " " << non_created->at(j)->node1 << " " << non_created->at(j)->node2 << " C" << endl;
                //out << i << " " << non_created->at(j)->node1 << " " << non_created->at(j)->node2 << " C" << endl;
                non_created->at(j)->times = i;
                created->push_back(non_created->at(j));
                non_created->erase(non_created->begin()+j);
            }
        }

        //delete links
        for (int j = 0; j < created->size(); ++j) {
            prob = (double)(rand()%100)/100.0;
            //create links
            if(prob < d){
                //cout << i << " " << created->at(j)->node1 << " " << created->at(j)->node2 << " S" << endl;
                out << created->at(j)->node1 << " " << created->at(j)->node2 << " " << created->at(j)->times << " " << i << endl;
                non_created->push_back(created->at(j));
                created->erase(created->begin()+j);
            }
        }
    }

    //TODO: do I have to suppress all existing nodes?

    intercontacts_deinit(created);
    intercontacts_deinit(non_created);

    out.close();

    string command = "sort -n -k1,2 tmp.txt > " + filename;

    system(command.c_str());
}

// creation: based on popularity? based on a function over time?
// deletion: increasing over time (function): D(t) or gaussian distribution
// creation: popularity --> weighted sum between given probability and popularity (popularity = avg degree between 2 nodes)

double markovian_creation_popularity(double probability, long degreeA, long degreeB, long number_nodes){
    double avg_degree = ((double)degreeA + (double)degreeB)/2.0;
    long total_edges = number_nodes*(number_nodes-1)*0.5;

    return (avg_degree/(double)total_edges)*probability;
}

void markovian_proposal_to_file(string filename, long number_nodes, long iterations, double creation_prob, double deletion_mean, double deletion_variance ){
    //Initialization
    fstream out;
    out.open("tmp.txt", ios::out);

    InterContacts* created = intercontacts_init();
    InterContacts* non_created = intercontacts_init();
    default_random_engine generator;

    srand(time(NULL));

    long* degree = new long[number_nodes];
    memset(degree, 0, number_nodes*sizeof(long));

    //normal_distribution<double> creation_distribution(creation_rate, creation_variance);
    normal_distribution<double> deletion_distribution(deletion_mean, deletion_variance);

    double prob = 0;
    double random_prob = 0;

    for (int i = 0; i < number_nodes; ++i) {
        for (int j = i+1; j < number_nodes; ++j) {
            InterContact* interContact = intercontact_init();
            interContact->node1 = i;
            interContact->node2 = j;
            non_created->push_back(interContact);
        }
    }

    //Markovian
    for (int i = 0; i < iterations; ++i) {
        //creates new links
        for (int j = 0; j < non_created->size(); ++j) {
            random_prob = (double)(rand()%100)/100.0;
            prob = markovian_creation_popularity(creation_prob, degree[non_created->at(j)->node1], degree[non_created->at(j)->node2], number_nodes);

            //create links
            if(random_prob < prob){
                //Increase ddegree of 2 nodes
                degree[non_created->at(j)->node1]++;
                degree[non_created->at(j)->node2]++;
                non_created->at(j)->times = i;
                non_created->at(j)->ttl = deletion_distribution(generator); //times used as START time
                //out << i << " " << non_created->at(j)->node1 << " " << non_created->at(j)->node2 << " C" << endl;
                created->push_back(non_created->at(j));
                non_created->erase(non_created->begin()+j);
            }
        }
        //delete links
        for (int j = 0; j < created->size(); ++j) {
            if(created->at(j)->ttl <= 0){
                //cout << i << " " << created->at(j)->node1 << " " << created->at(j)->node2 << " S" << endl;
                degree[created->at(j)->node1]--;
                degree[created->at(j)->node2]--;

                out << created->at(j)->node1 << " " << created->at(j)->node2 << " " << created->at(j)->times << " " << i << endl;
                non_created->push_back(created->at(j));
                created->erase(created->begin()+j);
            }else
                created->at(j)->ttl--;
        }
    }

    //TODO: do I have to suppress all existing nodes?

    intercontacts_deinit(created);
    intercontacts_deinit(non_created);

    out.close();

    string command = "sort -n -k1,2 tmp.txt > " + filename;
    system(command.c_str());
}

