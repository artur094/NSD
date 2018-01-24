//
// Created by Luca De Mori on 13/01/2018.
//

#ifndef LABLE_PROPAGATION_COMMUNITYDETECTION_H
#define LABLE_PROPAGATION_COMMUNITYDETECTION_H

#include "AdjacencyList.h"
#include <map>
#include <set>
#include <random>
#include "DynamicVector.h"

class CommunityDetection {

public:
    // compute communities detection with lablel propagation algorithm and give the number of communities detected
    static unsigned int lable_propagation(AdjacencyList &graph, string log_fn, string size_hist_fn, bool append, bool debug);
    static void generate_histogram();
    static void compute_ic_cdf(string distr_fn, string cdf_fn, bool debug);

    static unsigned int lblcalls;

private:


};


#endif //LABLE_PROPAGATION_COMMUNITYDETECTION_H
