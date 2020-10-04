#pragma once

#include <iostream>
#include <string>

#include "graph.h"

using namespace std;

void buildGraph(string filename, graph& g);
vector<char> DFS(graph& g, char startV);
vector<char> BFS(graph& g, char startV);
vector<char> DijkstraShortestPath(graph& g, char startV, map<char, char>& pred, map<char, int>& dist);
void printPredecessors(map<char, char>& pred, map<char, int>& dist, char currentV, char startV);