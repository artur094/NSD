//
// Created by Ivan Morandi on 28/09/2017.
//

#include <iostream>
#include <fstream>
#include <ctime>

int main(){
    int number_nodes = 6;
    int graph_degree[6] = {3, 3, 1, 3, 1, 1};
    int sum_degree = 12;

    int *graph = new int[number_nodes];
    int *neighbours = new int[sum_degree];

    graph[0] = 0;
    for (int i = 1; i < number_nodes; i++) {
        graph[i] = graph[i-1] + graph_degree[i-1];
    }

}

