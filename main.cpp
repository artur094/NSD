//
//  main.cpp
//  NSD-Practical1
//
//  Created by Andrea Ferlini on 19/09/2017.
//  Copyright © 2017 Andrea Ferlini. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <set>

using namespace std;

// functions
int graphSizeDegree(string filename);
int cleaningData(string filename);

int main(int argc, const char * argv[]) {
    
    //string filename = "/Users/AndreaFerlini/Downloads/actor-movie/out.actor-movie";
    //string filename = "/Users/AndreaFerlini/Downloads/com-amazon/out.com-amazon";
    //string filename = "/Users/AndreaFerlini/Documents/Master EIT - ITA/UPMC/NSD/Practical/Practical1/NSD-Practical1/NSD-Practical1/graphs/sample.practical1.graph";
    string filename = "graph3.txt";
    graphSizeDegree(filename);
    cin.get();
    cleaningData(filename);
    cin.get();
    graphSizeDegree(filename);
    return 0;
}


//// Size and degree of the graph
int graphSizeDegree(string filename){
    fstream graph;
    unsigned long numNodes=0;
    unsigned long numEdges=0;
    map<int, pair<set<int>, unsigned int> > graphMap;
    
    cout << "CALCULATING SIZE OF THE GRAPH AND DEGREE OF EACH NODE" << endl;
    graph.open(filename, ios::in);
    if (graph.is_open()){
        cout << "opening the file ..." << endl;
    
        int x, y;

        while(!graph.eof()){
            x = 0;
            y = 0;

            graph >> x >> y;
            graphMap[x].first.insert(y);
            graphMap[y].first.insert(x);
        
            numNodes = graphMap.size();
            numEdges++;
        }
        for ( map<int, pair<set<int>, unsigned int> >::iterator it=graphMap.begin(); it!=graphMap.end(); ++it){
            it->second.second = (unsigned int)it->second.first.size();
            cout << it->first << " " << it->second.second << endl;
        }
    }else{
        cout << "unable to open the file" << endl;
        return -1;
    }
    cout << "Nodes: " << numNodes << endl;
    cout << "Edges: " << numEdges << endl << endl;
    graph.close();
    return 0;
}

//// Cleaning graph data
int cleaningData(string filename){
    fstream graph;
    set<pair<int, int> > uniqueEdge;
    graph.open(filename, ios::in);
    cout << "CLEANING GRAPH DATA" << endl;
    
    if (graph.is_open()){
        cout << "opening the file ..." << endl;
        pair<int, int> edge;
        
        while(!graph.eof()){
            edge.first = 0;
            edge.second = 0;
            
            graph >> edge.first >> edge.second;
        
            if(edge.first != edge.second){
                uniqueEdge.insert(edge);
            }
        }
        graph.close();
        
        graph.open(filename, ios::out);
        cout << "WRITING CLEANED DATA ON THE GRAPH" << endl;
        
        if (graph.is_open()){
            cout << "opening the file ..." << endl;
            for (set<pair<int, int> >::iterator it=uniqueEdge.begin(); it!=uniqueEdge.end(); ++it){
                if(it == uniqueEdge.begin()){
                    graph << it->first << " " << it->second;
                }else{
                    graph << endl << it->first << " " << it->second;
                }
            }
        }else{
            cout << "unable to open the file" << endl;
            return -1;
        }
    }else{
        cout << "unable to open the file" << endl;
        return -1;
    }
    graph.close();
    cout << "DONE" << endl;
    return 0;
}
