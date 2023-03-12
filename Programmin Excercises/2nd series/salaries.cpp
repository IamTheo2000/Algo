#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#define BSIZE 1<<15

using namespace std;

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

long readLong() 
{
	long d = 0L, x = 0L;
	char c;

	while (1)  {
		if (bpos >= bsize) {
			bpos = 0;
			if (feof(stdin)) return x;
			bsize = fread(buffer, 1, BSIZE, stdin);
		}
		c = buffer[bpos++];
		if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
		else if (d == 1) return x;
	}
	return -1;
}


// A C++ program to find single source longest distances
// in a DAG
#include <iostream>
#include <limits.h>
#include <list>
#include <stack>
#define NINF INT_MIN
using namespace std;

class AdjListNode {
	int v;

public:
	AdjListNode(int _v)
	{
		v = _v;
	}
	int getV() { return v; }
};

// Class to represent a graph using adjacency list
// representation
class Graph {
	int V; // No. of vertices'

	// Pointer to an array containing adjacency lists
	list<AdjListNode>* adj;

	// A function used by longestPath
	void topologicalSortUtil(int v, bool visited[],
							stack<int>& Stack);

public:
	Graph(int V); // Constructor
	~Graph(); // Destructor

	// function to add an edge to graph
	void addEdge(int u, int v);

	// Finds longest distances from given source vertex
	int longestPath(int s);
};

Graph::Graph(int V) // Constructor
{
	this->V = V;
	adj = new list<AdjListNode>[V];
}

Graph::~Graph() // Destructor
{
	delete [] adj;
}


void Graph::addEdge(int u, int v)
{
	AdjListNode node(v);
	adj[u].push_back(node); // Add v to u's list
}

// A recursive function used by longestPath. See below
// link for details
// https:// www.geeksforgeeks.org/topological-sorting/
void Graph::topologicalSortUtil(int v, bool visited[],
								stack<int>& Stack)
{
	// Mark the current node as visited
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	list<AdjListNode>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i) {
		AdjListNode node = *i;
		if (!visited[node.getV()])
			topologicalSortUtil(node.getV(), visited, Stack);
	}

	// Push current vertex to stack which stores topological
	// sort
	Stack.push(v);
}

// The function to find longest distances from a given vertex.
// It uses recursive topologicalSortUtil() to get topological
// sorting.
int Graph::longestPath(int s)
{
	stack<int> Stack;
	vector<int> dist(V);

	// Mark all the vertices as not visited
	bool* visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Call the recursive helper function to store Topological
	// Sort starting from all vertices one by one
	for (int i = 0; i < V; i++)
		if (visited[i] == false)
			topologicalSortUtil(i, visited, Stack);

	// Initialize distances to all vertices as infinite and
	// distance to source as 0
	for (int i = 0; i < V; i++)
		dist[i] = NINF;
	dist[s] = 0;
	// Process vertices in topological order
	while (Stack.empty() == false) {
		// Get the next vertex from topological order
		int u = Stack.top();
		Stack.pop();

		// Update distances of all adjacent vertices
		list<AdjListNode>::iterator i;
		if (dist[u] != NINF) {
			for (i = adj[u].begin(); i != adj[u].end(); ++i){
			
				if (dist[i->getV()] < dist[u] + 1)
					dist[i->getV()] = dist[u] + 1;
			}
		}
	}
	
	int maxvalue = *std::max_element(std::begin(dist), std::end(dist));
	
	delete [] visited;

	return maxvalue;
}


int main () {   
    int N,K;
    N = readLong();
    K = readLong();

    vector<int> seq(N);
    for (int i=0; i<N; i++)
    {   
        int value = readLong();
        seq[i] = value ;
    }
	
	Graph g(2*N);
	for(int i=0; i<N; i++) {
		for (int j=i+1; j<N; j++) {
			if (seq[i] < seq[j]) {
				g.addEdge(i,j);
				g.addEdge(i+N,j+N);
			}
			if (seq[j] > seq[i] - K) {
				g.addEdge(i+N,j);
			}

		}
	}

	int maxvalue = NINF;
	for (int i=N; i<2*N; i++) {
		int value = g.longestPath(i);
		if (value > maxvalue) {maxvalue = value;}
	}
	cout << maxvalue + 1 << endl;
	return 0; 
}