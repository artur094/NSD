//
// Created by Ivan Morandi on 28/11/2017.
//

#ifndef NSD_SIMULATION_H
#define NSD_SIMULATION_H

struct Simulation{
    Graph* original;
    Graph* sample;
    vector<long> time;
    vector<long> edge_u;
    vector<long> edge_v;
};
typedef struct Simulation Simulation;

#endif //NSD_SIMULATION_H
