//
// Created by Ivan Morandi on 04/01/18.
//

#include <iostream>
#include "graph.h"
#include "adjacencylist.h"

using namespace std;

void diameter_from_file(string src, string dst, long number_nodes, long offset);

int main(int argc, char** argv) {
    cout << "NSD Course:" << endl << endl;
    long start = time(NULL);

    long iterations = 100;
    long number_nodes = 100;
    double prob_creation = 0.5;
    double prob_deletion = 0.5;
    double deletion_mean = 5;
    double deletion_variance = 5;

    if(argc > 7){
        deletion_variance = atof(argv[7]);
    }
    if(argc > 6){
        deletion_mean = atof(argv[6]);
    }
    if(argc > 5){
        prob_deletion = atof(argv[5]);
    }
    if(argc > 4){
        prob_creation = atof(argv[4]);
    }
    if(argc > 3){
        number_nodes = atol(argv[3]);
    }
    if(argc > 2){
        iterations = atol(argv[2]);
    }
    if(argc < 2){
        cout << "Missing name!" << endl;
        cout << "Write ./a.out <graph_name>" << endl;
        exit(1);
    }

    cout << "argc = " << argc << endl;

    cout << "Running markovian with " << number_nodes << " nodes, " << iterations << " iterations. P(creation) = " << prob_creation << " and P(suppression) = " << prob_deletion << endl;
    markovian_to_file(argv[1], number_nodes, iterations, prob_creation, prob_deletion);

    cout << "Running proposed markovian with probability of creation="<<prob_creation << ", deletion mean=" << deletion_mean << " and deletion variance=" << deletion_variance << endl;
    markovian_proposal_to_file("markovian_proposal_1.txt", number_nodes, iterations, prob_creation, deletion_mean, deletion_variance);
    cout << "Done!" << endl;


    long end = time(NULL);
    cout<<"\nTime required: "<<end-start<<" seconds"<<endl;
    return 0;
}