//
// Created by Luca De Mori on 13/01/2018.
//

#include "CommunityDetection.h"


unsigned int CommunityDetection::lable_propagation(AdjacencyList &graph, string log_fn, string size_hist_fn, bool append, bool debug) {

    int lbl_start = (int)time(nullptr);

    bool changed;
    unsigned int *order = new unsigned int[graph.getNumNodes() + 1]();
    unsigned int *label = new unsigned int[graph.getNumNodes() + 1]();
    unsigned int n;
    unsigned int l;
    unsigned int iterations;
    fstream logfile;
    fstream histfile;

    Node nbr;
    map<unsigned int, unsigned int> lbl_counter;

    map<unsigned int, set<unsigned int> > communities;

    unsigned int max_occurrencies;
    unsigned int domin_label;


    unsigned int temp;
    unsigned int j;


    DynamicVector<unsigned int> size_hist;

    // set default lable = ID
    for (n = 1; n <= graph.getNumNodes(); n++) {
        label[n] = graph.nodes[n].ID;
    }


/*
    if (debug) {
        for (n = 1; n <= graph.getNumNodes(); n++) {
            cout << order[n] << " ";
        }
        cout << endl;
    }*/




    iterations = 0;
    do {

        changed = false;

        ++iterations;
        //if (++iterations>7) break;
        if (debug) cout << time(nullptr)-lbl_start << "s: "  << "[DEBUG] CommunityDetection::lable_propagation(): ***Iteration #" << iterations << endl;


        // scrumble the array
        for (unsigned int k = graph.getNumNodes(); k > 0; k--) {

            j = rand()%k+1;
            temp = (order[k] != 0) ? order[k] : k;
            order[k] = (order[j] != 0) ? order[j] : j;
            order[j] = temp;
        }

        // cycle on every nodes
        for (unsigned int i = 1; i<=graph.getNumNodes(); i++) {

            /*// skip if the node is done
            if (completed[n]) continue;*/

            n = order[i];
            //++ lbl_counter[n];
            // find max occurring lable in the neighborhood
            max_occurrencies = 0;
            domin_label = 0;
            lbl_counter.clear();

            for (unsigned int nb_idx = 0; nb_idx < graph.nodes[n].degree; nb_idx++){


                nbr = *graph.getNeighbour(n, nb_idx);
                // read the lable
                //l = label[nbr.ID];
                // increment the counter in the corrisponding position
                //++ lbl_counter[l];
                if (++lbl_counter[label[nbr.ID]] > max_occurrencies) {
                    max_occurrencies = lbl_counter[label[nbr.ID]];
                    domin_label = label[nbr.ID];
                }

                //if (debug) cout << "[DEBUG] CommunityDetection::label_propagation(): n=" << n << " nb=" << nbr.ID
                 //               << " lbl=" << label[nbr.ID] << " [+1] -> #=" << lbl_counter[label[nbr.ID]] << endl;


            }

            //if (debug) cout << "[DEBUG] CommunityDetection::label_propagation(): >>> n=" << n << " dom_lbl="
             //               << domin_label << "(" << max_occurrencies << ")" ;

            if (label[n]!=domin_label){
                //if (debug) cout << " >>> lbl was: " << label[n] << " -> setting to: " << domin_label << endl;
                label[n] = domin_label;
                changed = true;

            }

        }


    } while (changed);

    for (n=1; n<=graph.getNumNodes(); n++){
        communities[label[n]].insert(n);
    }



    logfile.open(log_fn, ios::out);


    map<unsigned int, set<unsigned int> >::iterator c_it;
    set<unsigned int>::iterator n_it;
    float avg_size = 0;


    if (debug) cout << "[DEBUG] CommunityDetection::lable_propagation(): DETECTED COMMUNITIES: " << communities.size() << endl;
    //if (append) logfile << "# ----------------------------------------------------------------" << endl;

    logfile << "# nr communities: " << communities.size() << endl;
    logfile << "# size n1 n2 n3 ..." << endl;

    for (c_it = communities.begin(); c_it!=communities.end(); ++c_it){

        size_hist[c_it->second.size()]++;
        avg_size += (float) c_it->second.size()/communities.size();

        //if (debug) cout << "lbl: " << c_it->first << "(" << c_it->second.size() << ") N={";
        //logfile << c_it->first << " "
        logfile << c_it->second.size() << " ";
        for (n_it = c_it->second.begin(); n_it!=c_it->second.end(); ++n_it){
            //if (debug) cout << *n_it << " ";
            logfile << *n_it << " ";
        }

        //if (debug) cout << "}" << endl;
        logfile << endl;
    }

    histfile.open(size_hist_fn, ios::out);

    //histfile << "# average= " << avg_size << endl;
    cout << "# nr communities: " << communities.size() << endl;
    cout << "# average size:   " << avg_size << endl;
    logfile << "# average size:  " << avg_size << endl;
    size_hist.f_print_nz(histfile);

    logfile.close();
    histfile.close();



    delete[] order;
    delete[] label;



    return  communities.size();

    }


void CommunityDetection::compute_ic_cdf(string distr_fn, string cdf_fn, bool debug) {

    // Variables
    fstream      infile;
    fstream      outfile;
    unsigned int s;     // size
    unsigned int occ;   // occurencies
    unsigned int cum;   // cumulative occurrencies
    // ---------------------------------------------------------------------------------------------


    if (debug) cout << "[DEBUG] - CommunityDetection::compute_ic_cdf: starting the function..." << endl;

    if (debug)
        cout << "[DEBUG] - CommunityDetection::compute_ic_cdf: opening the file..." << endl;

    outfile.open(cdf_fn, ios::out);

    infile.open(distr_fn, ios::in);

    if(debug) cout <<  "[DEBUG] - CommunityDetection::compute_ic_cdf: graph opening succeed! computing cumulative distribution..." << endl;

    s = occ = -1;
    cum = 0;

    while (!infile.eof()) {
        // t occ
        infile >> s    // size
               >> occ; // occ

        if (s == -1 || occ == -1) {
            continue;
        }

        if (debug) cout << s << " " << occ << endl;

        // accumulate occurrences and write to file
        cum += occ;
        outfile << s << " " << cum << endl;


    }


    if (debug) cout << "[DEBUG] - CommunityDetection::compute_ic_cdf: done" << endl;

    infile.close();
    outfile.close();

}

