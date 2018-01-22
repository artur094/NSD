//
// Created by Ivan Morandi on 07/11/2017.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;

void generate_graph(char* file_name, char* file_comm, double p, double q);

int LIMIT = 400;

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char** argv){
    if(argc < 3){
        cout << "Error, wrong number of arguments" << endl;
        cout << "Usage: ./<name_exe> <probability_p> <probability_q>" << endl;
        return 1;
    }

    srand(time(NULL));

    double p = atof(argv[1]);
    double q = atof(argv[2]);

    cout << "NSD Course:" << endl;
    cout << "Practical 5, Exercise 1" << endl << endl;
    cout << "Generating graph ... " << endl;
    generate_graph("output.txt", "communities.txt", p, q);
    cout << "Done!" << endl;
}

void generate_graph(char* file_name, char* file_comm, double p, double q){
    ofstream out;
    ofstream comm;
    out.open(file_name, ios::out);
    comm.open(file_comm, ios::out);
    double prob;
    double res;

    cout << "Building graph with prob " << p << " to be inside the community and prob " << q << " to go out" << endl;

    for (int i = 0; i < LIMIT; ++i) {
        for (int j = i+1; j < LIMIT; ++j) {
            if(i/100 == j/100)
                prob = p;
            else
                prob = q;

            res = fRand(0.0, 1.0);
            //cout << res << " <= " << prob*100 << endl;
            if(res < prob){
                //cout << i << " " << j << " " << prob*100 << endl;
                out << i << " " << j << endl;
            }
        }
    }

    for (int i = 0; i < LIMIT/100; ++i) {
        comm << "100 ";
        for (int j = 0; j < 100; ++j) {
            comm << i*100+j << " ";
        }
        comm << endl;
    }

    out.close();
    comm.close();
}

