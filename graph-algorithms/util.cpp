/*util.cpp*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <string>
#include <fstream>
#include <map>

#include "util.h"
#include "minqueue.h"

using namespace std;

//
// buildGraph:
//
// Inputs the graph vertices and edges from the given file, building
// the graph g.  File format:
//   vertex vertex vertex ... #
//   src dest weight ... #
//
void buildGraph(string filename, graph& g)
{
  ifstream file(filename);
  char     v;

  //error checking to make sure the file input by the user is valid, otherwise the graph 
  //is not populated
  if (!file.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << filename << "'." << endl;
    cout << endl;
    return;
  }

  //
  // Input vertices as single uppercase letters:  A B C ... #
  //
  file >> v;

  while (v != '#')
  {
    g.addvertex(v); //add each valid vertex to the graph

    file >> v;
  }

  //
  // Now input edges:  Src Dest Weight ... #
  //
  char src, dest;
  int  weight;

  file >> src;

  while (src != '#')
  {
    file >> dest;
    file >> weight;

    g.addedge(src, dest, weight); //populates the graph with edges b/w vertices as well

    file >> src;
  }
}


//
// BFS:
//
// Performs a breadth-first search from the given starting vertex.  
// Returns the vertices in the order they were visited; startV will
// be first.
//
vector<char> BFS(graph& g, char startV)
{
  vector<char>  visited;
  queue<char>   frontierQueue;
  set<char>     discoveredSet;
  char currentV;
  vector<char> neighbors;
  
  
  frontierQueue.push(startV); //pushes vector being searched into queue
  discoveredSet.insert(startV); //marks vector being searched as discovered, since search starts at startV
  
  //loop to run the breadth first search on the graph
  while(!frontierQueue.empty()){
     currentV = frontierQueue.front();  //stores frontierQueue.pop()
     frontierQueue.pop();
     
     if(g.isvertex(currentV)){
        visited.push_back(currentV); //marks vertex as visited
     }
     
     neighbors = g.neighbors(currentV); //collects adjacent vertices 
     for(char nb : neighbors){
        if (discoveredSet.find(nb) == discoveredSet.end()){ //if adjacent vertex has not been discovered yet
           frontierQueue.push(nb); //stored in queue to be visited
           discoveredSet.insert(nb); //marked as discovered
        }
     }
  }
     
  
  return visited;
}

//
// DFS:
//
// Performs a depth-first search from the given starting vertex.  
// Returns the vertices in the order they were visited; startV will
// be first.
//
vector<char> DFS(graph& g, char startV)
{
  vector<char>  visited;
  stack<char>   frontierStack;
  set<char>     visitedSet;
  vector<char> neighbors;

  frontierStack.push(startV); //pushes first vector onto stack
  char currentV;
  
  while(!frontierStack.empty()){  //loop to perform depth-first search
	 currentV = frontierStack.top(); //stores frontierStack.pop()
	 frontierStack.pop();

	 if(visitedSet.find(currentV) == visitedSet.end()){ //if current vertex has not been visited yet
		visited.push_back(currentV);  // marks it as visited
		visitedSet.insert(currentV);  // stores it into visitedSet

		neighbors = g.neighbors(currentV); //collects adjacent vertices of current vertex

		 for(char nb : neighbors){
		   frontierStack.push(nb);  //pushes adjacent vertices onto stack
		}  

	 }
  }

  return visited;
}


//
// Dijkstra's Shortest Path:
//
// This algorithm's goal is to find the cheapest (lowest weight) between each vertex that is 
// connected by some path to startV. StartV is the vertex which the algorithm starts at, and from
// there the cheapest path to reach each vertex which can be reached is calculated and stored.
//
vector<char> DijkstraShortestPath(graph& g, char startV, map<char, char>& predecessors, map<char, int>& distance)
{
	minqueue<char, int> unvisited;
	vector<char> visited;
	vector<char> neighbors;
	vector<char> vertices = g.vertices();
	char currentV;
	int edgeWeight;
	int alternativeDistance;
	const int Infinity = numeric_limits<int>::max();  //Infinity is basically the max number C++ allows
	
	//Basically arranges both maps to have default values of either Infinity or -1. The -1 in predecessors is
	//marked as the index, and since no index has been found for any predecessors it is initialized as -1. Each
	//vertex in distance is marked as Infinity since the goal is to find a distance less than that number, not greater.
	//Since each vertex hasn't been visited yet, unvisited must store every vertex.
	for(char vert : vertices){
		distance[vert] = Infinity;
		predecessors[vert] = -1;
		unvisited.pushinorder(vert, Infinity);
	}
	
	distance[startV] = 0;             //distance is set as 0 since there is a distance of 0 between the same vector
	predecessors[startV] = startV;    //same basic rule for predecessors, since there is no predecessors for the starting vertex
	unvisited.pushinorder(startV, 0); //startV will be returned to at the end 
	
	//goes through each vertex stored in the queue of unvisited
	while(!unvisited.empty()){
		// the current vertex is the front of the queue, or unvisited.pop()
		currentV = unvisited.minfront();
		unvisited.minpop();
		
		// if the current vertex isn't reachable, then simply break out of the loop
		if(distance[currentV] == Infinity){
			break;
		}
		
		// store the current vertex as visited
		visited.push_back(currentV);
		// collect the adjacent vertices to the current one
		neighbors = g.neighbors(currentV);
		
		// loop through and collect weights from each adjacent vertex
		for(char nb: neighbors){
			// collect weight between neighbor and current vertex
			edgeWeight = g.getweight(currentV, nb);
			// calculate other distance to compare the two 
			alternativeDistance = distance[currentV] + edgeWeight;
			
			// calculate minimum distance between the current stored one and the new one
			if (alternativeDistance < distance[nb]){
				// if it passes, store the new smaller distance
				distance[nb] = alternativeDistance;
				// reset the predecessors as the current vertex since the weight is lower
				predecessors[nb] = currentV;
				// store in new smaller distance to update queue and resort itself so
				// smallest vertex is at front
				unvisited.pushinorder(nb, alternativeDistance); 
			}
		}
	}
	
	return visited; 
}


// 
// Helper function to print the predecessors for each vertex after running DijkstraShortestPath algorithm
// 
void printPredecessors(map<char, char>& pred, map<char, int>& dist, char currentV, char startV){
	vector<char> storePreds;
	
	cout << dist[currentV];  //prints out smallest distance, which has already been calculated
	cout << " via ";
	
	storePreds.push_back(currentV); //stores the current vertex first, since this is the last predecessors
	
	// while loop to store all predecessors for current vertex
	// this allows the vector to collect all the predecessors until it arrives
	// back at the starting vertex
	while(pred[currentV] != currentV){
		storePreds.push_back(pred[currentV]);  
		currentV = pred[currentV];  // resets currentV as its predecessors
	}
	
	// prints the vector out backwards since its stored in the wrong order
	for(int i = storePreds.size()-1; i > 0; i--){
		cout << storePreds.at(i) << " ";
	}
	
	// prints out the starting vertex at the end since it wasn't originally stored
	cout << storePreds.at(0);
	
	cout << endl;
}
