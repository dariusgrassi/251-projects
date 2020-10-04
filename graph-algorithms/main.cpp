/* main.cpp */

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <fstream>
#include <map>

#include "graph.h"
#include "util.h"

using namespace std;

//
// outputGraph:
//
// Outputs graph g to the console.
//
void outputGraph(graph& g)
{
  vector<char> vertices = g.vertices();  //collects all vertices in graph

  cout << "**Vertices: ";

  for (char c : vertices)
  {
    cout << c << " "; //outputs each vertex in graph
  }

  cout << endl;

  cout << "**Edges: ";

  for (char v : vertices)
  {
    vector<char> neighbors = g.neighbors(v);  //collects each adjacent vector for each vertex

    for (char n : neighbors)
    {
      int weight = g.getweight(v, n); //collects weight and prints out neighbors and weight for vertex
      cout << "(" << v << "," << n << "," << weight << ") ";
    }
  }

  cout << endl;
}

//function to print out each vertex in any vector
//creating this allows us to simplify our code by "minimizing" for-loops
void printVertices(string prefix, vector<char>& V){
	cout << prefix;

	for (char v : V)
		cout << v << " "; //just print out each individual character in vertex

	cout << endl;
}


int main()
{
	graph  g;
	string filename;
	char   startV;
	map<char, char> pred;
	map<char, int> dist;

	cout << "Enter filename containing graph data> ";
	cin >> filename;

	//inputs the graph information and then we output to see 
	//what's been stored in the graph
	buildGraph(filename, g);
	outputGraph(g);

	// ask user for starting vertex, and store it
	cout << endl;
	cout << "Enter a starting vertex or #> ";
	cin >> startV;

  while (startV != '#')
  {
    vector<char> visited;

    //As long as the vertex is valid, we are able to print out
    //information on how the different searches run
    if (g.isvertex(startV))
    {
		//first we print out the neighbors for the current vertex
		visited = g.neighbors(startV);
		printVertices("Neighbors: ", visited);
		
		//then we run a breadth-first search
		visited = BFS(g, startV);
		printVertices("BFS: ", visited);
		
		//then we run a depth-first search
		visited = DFS(g, startV);
		printVertices("DFS: ", visited);
		
		//finally, we run our Dijkstra algorithm to find the shortest path
		//for each vertex
		visited = DijkstraShortestPath(g, startV, pred, dist);
		printVertices("Dijkstra: ", visited);
		
		//then we print out the predecessors map populated in the Dijkstra function
		for(char v : visited){
			cout << v << ": ";
			printPredecessors(pred, dist, v, startV);
		}
		
    }
    else
    {
      cout << "Not a vertex in graph, ignored..." << endl;   //error-checking if the user puts in an invalid vertex
    }

    cout << endl;
    cout << "Enter a starting vertex or #> ";   //user input loop to get information on different vertices
    cin >> startV;
  }

  //
  // done:
  //
  return 0;
}