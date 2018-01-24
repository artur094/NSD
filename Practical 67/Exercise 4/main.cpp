#include <iostream>
#include <fstream>

using namespace std;

string f_benchmark = "../communities.txt";
string f_name1 = "../jcb.txt";

// get the size of the communities in a given file
int* get_size (string filename, int comm_number, bool debug) {

    fstream infile;
    auto* sizes = new int [comm_number];

    infile.open(filename);

    if (debug)
        if (!infile.is_open())
            cout << "[ERROR] - getsize: infile not open" << endl;

    while (!infile.eof()) {

        for (int i = 0; i < comm_number; i ++){
            infile >> sizes[i];

            for (int j = 0; j < sizes[i]; j ++) {
                int tmp;
                infile >> tmp;
            }
        }
    }
    if (debug)
        cout << "Number of Communities: " << comm_number << endl;
    infile.close();

    if (debug)
        for (int  j = 0;  j < comm_number; ++ j) {
            cout << "size of community: " << j << " = " << sizes[j] << " " << endl;
        }

    return sizes;
}

int** community_loader(string filename, int comm_number, bool debug) {
    fstream infile;

    auto** communities = new int* [comm_number];
    int* comm_sizes = get_size(filename, comm_number, false);

    infile.open(filename);

    while (!infile.eof()) {

        for (int i = 0; i < comm_number; i ++){

            int tmp = 0;
            infile >> tmp;
            communities[i] = new int[tmp];

            //load the i-th community
            for (int j = 0; j < tmp; j ++) {
                infile >> communities[i][j];
            }
        }
    }

    infile.close();

    if (debug){
        cout << "File: " << filename << endl;
        for (int i = 0; i < comm_number; i ++) {
            cout << "community " << i << " = ";
            for (int j = 0; j < comm_sizes[i]; j ++) {
                cout << communities[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    delete [] comm_sizes;

    return communities;
}

float* compute_score(string benchmark_file, string compared_file, int comm_number, int file_numbers, bool debug) {
    auto* score = new float [comm_number*file_numbers];

    int found = 0;

    //bool alert_benchmark = false;
    //bool alert_file = false;


    int** benchmark = community_loader(benchmark_file, comm_number, debug);
    int* sizes_benchmark = get_size(benchmark_file, comm_number, false);

    int** communities_x = community_loader(compared_file, comm_number, debug);
    int* sizes_file_x = get_size(compared_file, comm_number, false);

    int row_x = 0;
    int row_b = 0;
    int col_x = 0;
    int col_b = 0;

    bool eof = false;

    for (row_b = 0; row_b < comm_number; row_b ++) { // new line

        // init the score
        found = 0;
        score[row_x] = 0;

        //init column x
        col_x = 0;

        // iterate over all the columns
        for (col_b = 0; col_b < sizes_benchmark[row_b]; col_b ++) {

            // F[rf][cf] < B[rb][cb]
            while (communities_x[row_x][col_x] > benchmark[row_b][col_b] && !eof) {
                if (col_b < sizes_benchmark[row_b]) {
                    col_b ++;
                } else {
                    // new line in both the files
                    col_b = 0;
                    col_x = 0;
                    if (row_x < comm_number && row_b < comm_number) {
                        row_b ++;
                        row_x ++;
                    } else {
                        cout << "no more rows" << endl;
                        eof = true;
                    }
                }
            }

            while (communities_x[row_x][col_x] < benchmark[row_b][col_b] && !eof) {
                if (col_x < sizes_file_x[row_x]) {
                    col_x ++;
                } else {
                    // new line in both the files
                    col_x = 0;
                    col_b = 0;
                    if (row_x < comm_number-1 && row_b < comm_number-1) {
                        row_b ++;
                        row_x ++;
                    } else {
                        cout << "no more rows" << endl;
                        eof = true;
                    }
                }
            }

            if (communities_x[row_x][col_x] == benchmark[row_b][col_b]) {
                found ++;

                if (col_x < sizes_file_x[row_x]) {
                    col_x ++;
                }
            }
        } // for on col_b

        // give the score for every row of the file_x
        if (found != 0)
            score[row_x] = (float)(found+1)/(sizes_benchmark[row_b]+1);
        else
            score = 0;

        row_x ++;
    } // for on row_b

    return score;
}






int main() {

    bool debug = true;
    int comm_number = 4;
    int file_numbers = 2; // number of files to compare: benchmark included

    float* points;

    cout << endl;

    points = compute_score(f_benchmark, f_name1, comm_number, file_numbers, debug);

    for (int i = 0; i < comm_number; ++i) {
        //cout << "score community " << i << ": " << points[i] << endl;
        cout << "score community " << i << ": " << points[i]*100 << " %" << endl;
    }
    return 0;
}