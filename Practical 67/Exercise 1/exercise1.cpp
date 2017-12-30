//
// Created by Ivan Morandi on 07/11/2017.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;

void generate_graph(char* file_name, double p);

int LIMIT = 400;

int main(int argc, char** argv){
    if(argc < 2){
        cout << "Error, wrong number of arguments" << endl;
        cout << "Usage: ./<name_exe> <probability_p>" << endl;
        return 1;
    }

    srand(time(NULL));

    double p = atof(argv[1]);

    cout << "NSD Course:" << endl;
    cout << "Practical 5, Exercise 1" << endl << endl;
    cout << "Generating graph ... " << endl;
    generate_graph("output.dat", p);
    cout << "Done!" << endl;
}

void generate_graph(char* file_name, double p){
    ofstream out;
    out.open(file_name, ios::out);
    double prob;
    double res;

    for (int i = 0; i < LIMIT; ++i) {
        for (int j = i+1; j < LIMIT; ++j) {
            if(i/100 == j/100)
                prob = p;
            else
                prob = 1-p;

            res = rand()%100;
            //cout << i << " " << j << " " << prob << endl;
            //cout << res << " <= " << prob*100 << endl;
            if(res <= prob*100){
                out << i << " " << j << endl;
            }
        }
    }

    out.close();
}

