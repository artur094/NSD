//
// Created by Ivan Morandi on 28/11/2017.
//

#include <iostream>
#include <vector>
#include <fstream>
#include "adjacencylist.h"
#include "simulation.h"

using namespace std;

Simulation* simulation_init(Graph* original){
    if(original == NULL)
        return NULL;

    Simulation* sim = new Simulation();
    sim->original = original;
    sim->sample = graph_empty_copy(original);
    sim->time = new vector<long>(original->number_edges);
    sim->edge_u = new vector<long>(original->number_edges);
    sim->edge_v = new vector<long>(original->number_edges);
}

void simulation_print(Simulation* sim){
    for(int i=0; i < sim->time.size(); i++){
        cout << sim->time[i] << " " << sim->edge_u[i] << " " << sim->edge_v[i] << endl;
    }
}

void simulation_store_file(char* name, Simulation* sim){
    fstream out;
    out.open(name, ios::out);

    for(int i=0; i < sim->time.size(); i++){
        out << sim->time[i] << " " << sim->edge_u[i] << " " << sim->edge_v[i] << endl;
    }

    out.close();
}

void simulation_analyze(Simulation* sim){

}

void simulation
