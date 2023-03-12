// C++ program to find single source shortest
// paths for Directed Acyclic Graphs
#include<iostream>
#include<list>
#include<stack>
#include<vector>
#include<tuple>
#include <climits>
// #define _CRTDBG_MAP_ALLOC
// #include <crtdbg.h>
#define BSIZE 1<<15
#define INF ULLONG_MAX
using namespace std;

// Graph is represented using adjacency list. Every node
// of adjacency list contains vertex number of the vertex
// to which edge connects. It also
// contains weight of the edge
class AdjListNode
{
	int v;
	unsigned long long int weight;
public:
	AdjListNode(int _v, unsigned long long int _w) { v = _v; weight = _w;}
	int getV() { return v; }
	unsigned long long int getWeight() { return weight; }
};

// Class to represent a graph using adjacency
// list representation
class Graph
{
	int V; // No. of vertices'

	// Pointer to an array containing adjacency lists
	list<AdjListNode> *adj;
    // Paths is the adjlist if we flip every edge
    vector<int> paths;
    // Vector of tuples containing the pairs P,S for every node (except node 0)
    vector<tuple<int,int>> costs;
    // vector<unsigned long long int> correct;

	// A function used by shortestPath
	void topologicalSortUtil(int v, bool visited[], stack<int> &Stack);
public:
	Graph(int V); // Constructor
    ~Graph();   //Destructor

	// function to add an edge to graph
	void addEdge(int u, int v, unsigned long long int weight);
    // Function to add P,S pair to cost vector
    void addCost(int p, int s);

    //Some debug functions
    // void addCorrect(unsigned long long int c);
    void printAdj();
    void printPaths();
    void printCosts();

	// Finds best path from given source vertex
	void shortestPath(int s);
};

// Class to represent a Line of nodes
class Line
{
	int V; // No. of vertices
	list<AdjListNode> *adj;
    vector<tuple<int,int>> costs;
    vector<int> paths;
public:
	Line(int V); // Constructor
    ~Line();
	void addEdge(int u, int v, unsigned long long int weight);
    void addCost(int p, int s);
    void shortestLine(int s);
};

Graph::Graph(int V)
{
	this->V = V;
	adj = new list<AdjListNode>[V];
    paths.reserve(V);
    costs.reserve(V-1);
    // correct.reserve(V);
    
}
Graph::~Graph() // Destructor
{
	delete [] adj;
}

Line::Line(int V)
{
	this->V = V;
    adj = new list<AdjListNode>[V];
    costs.reserve(V-1);    
    paths.reserve(V);
}
Line::~Line() // Destructor
{
	delete [] adj;
}

void Graph::addEdge(int u, int v, unsigned long long int weight)
{
	AdjListNode node(v, weight);
	adj[u].push_back(node); // Add v to u's list
    paths[v] = u; // Add the opossite
}

void Line::addEdge(int u, int v, unsigned long long int weight)
{   
    AdjListNode nodeu(u,weight), nodev(v,weight);
	adj[u].push_back(nodev);
    adj[v].push_back(nodeu);
}

void Graph::addCost(int p, int s)
{
	costs.push_back(tuple<int, int>(p,s));
}

void Line::addCost(int p, int s)
{
	costs.push_back(tuple<int, int>(p,s));
}

// For debugging
// void Graph::addCorrect(unsigned long long int c)
// {
// 	correct.push_back(c);
// }

// A recursive function used by shortestPath.
// See below link for details
// https://www.geeksforgeeks.org/topological-sorting/
void Graph::topologicalSortUtil(int v, bool visited[], stack<int> &Stack)
{
	// Mark the current node as visited
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	list<AdjListNode>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
	{   
        // cout << i->getV() << endl;
		// AdjListNode node = *i;
		if (!visited[i->getV()])
			topologicalSortUtil(i->getV(), visited, Stack);
	}
	// Push current vertex to stack which stores topological sort
	Stack.push(v);
}

// The function to find best paths from given vertex.
// It uses recursive topologicalSortUtil() to get topological
// sorting of given graph.
void Graph::shortestPath(int s)
{
	stack<int> Stack, Helpstack;
    // The kilometer distance from every node to s
	vector<unsigned long long int> dist(V);
    // Maximum distances from every node to s
    vector<unsigned long long int> atleast(V);
    // The minimum cost from every node to s
    vector<unsigned long long int> mins(V);

	// Mark all the vertices as not visited
	bool *visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Call the recursive helper function to store
	// Topological Sort starting from all vertices
	// one by one

	for (int i = 0; i < V; i++){
		if (visited[i] == false){
			topologicalSortUtil(i, visited, Stack);
        }
    }
	// Initialize distances to all vertices as
	// infinite and distance to source as 0
	for (int i = 0; i < V; i++)
		dist[i] = INF;
	dist[s] = 0;
    atleast[s] = 0;
    mins[0] = 0;

    Helpstack = Stack;
	// Process vertices in topological order
	while (Stack.empty() == false)
	{
		// Get the next vertex from topological order
		int u = Stack.top();
		Stack.pop();
		// Update distances of all adjacent vertices
		list<AdjListNode>::iterator i;
		if (dist[u] != INF)
		{
		for (i = adj[u].begin(); i != adj[u].end(); ++i)
			if (dist[i->getV()] > dist[u] + i->getWeight())
				dist[i->getV()] = dist[u] + i->getWeight();           
		}
	}

    for (int i=0; i<V-1; i++){
        // cout << get<0>(costs[i]) << " " << get<1>(costs[i]) << " " << dist[i+1] << endl;
        atleast[i+1] = get<0>(costs[i]) + dist[i+1]*get<1>(costs[i]);
    }

    Helpstack.pop();
    while (Helpstack.empty() == false)
	{
		int u = Helpstack.top();
        Helpstack.pop();

        unsigned long long int minimum = atleast[u];
        int current = paths[u];

        while (current != 0){
            minimum = min(minimum, atleast[u] - dist[current]*get<1>(costs[u-1]) + mins[current]);
            current = paths[current];
        }
        mins[u] = minimum;
	}
    
    // Print the minimum cost to s for every node
    cout << mins[1];
    for (int i = 2; i < V; i++)
		cout << " " << mins[i];
    cout << endl;

    // Correct output (for debugging)
    // for (int i=1; i<V; i++){
    //     (mins[i] == correct[i-1])? cout << mins[i] << " " : cout << "\033[1;31m" << correct[i-1] << "\033[0m" << " ";;
    // }
    delete [] visited;
}

void Line::shortestLine(int s) {
    // The minimum cost from every node to s
    vector<unsigned long long int> mins(V);
    vector<unsigned long long int> dist(V);
    dist[0] = 0;
    paths[0] = 0;
    mins[0] = 0;
    

    list<AdjListNode>::iterator sp;
    for( sp = adj[0].begin(); sp != adj[0].end(); sp++){
        int previous = 0;
        int i=sp->getV();
        int next=sp->getV();
        dist[i] = sp->getWeight();
        while (adj[i].size() != 1)
        {   
            paths[i] = previous;
            list<AdjListNode>::iterator j;
            for( j = adj[i].begin(); j != adj[i].end(); j++){
                if (j->getV() != previous){
                    next = j->getV();
                    dist[next] = j->getWeight() + dist[i]; 

                    unsigned long long int minimum = get<0>(costs[i-1]) + dist[i]*get<1>(costs[i-1]);
                    int current = paths[i];

                    while (current != 0){
                        minimum = min(minimum, get<0>(costs[i-1]) + dist[i]*get<1>(costs[i-1]) - dist[current]*get<1>(costs[i-1]) + mins[current]);
                        current = paths[current];
                    }
                    mins[i] = minimum;
                }
            }
            previous = i;
            i = next;
        }
        paths[i] = previous;
        list<AdjListNode>::iterator j;
        for( j = adj[i].begin(); j != adj[i].end(); j++){
            next = j->getV();

            unsigned long long int minimum = get<0>(costs[i-1]) + dist[i]*get<1>(costs[i-1]);
            int current = paths[i];
            while (current != 0){
                minimum = min(minimum, get<0>(costs[i-1]) + dist[i]*get<1>(costs[i-1]) - dist[current]*get<1>(costs[i-1]) + mins[current]);
                current = paths[current];
            }
            mins[i] = minimum;
            
        }

    }

    cout << mins[1];
    for (int i = 2; i < V; i++)
		cout << " " << mins[i];
    cout << endl;
    
}

//Function to print adj list (for debugging)
void Graph::printAdj() {
    for (int v=0; v<V; v++) {
        cout << v << " -> ";
        list<AdjListNode>::iterator i;
        // if (adj[v].begin() == adj[v].end())
        //     cout << "Found";
        for (i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            AdjListNode node = *i;
            cout << node.getV() << " ";
        }
        cout << endl;
    }
}

//Function to print paths vector (for debugging)
void Graph::printPaths() {
    for (int v=0; v<V; v++) {
        cout << v << " -> " << paths[v] << endl;
    }
}

//Function to print costs vector of tuples (for debugging)
void Graph::printCosts() {
for(auto& tuple: costs) {
        cout << get<0>(tuple) << " " << get<1>(tuple) << endl;   
    }
}


//Read from standard input (Fotakis slides)
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

int main()
{
    int N = readLong();

    //Find the shortes paths to s=0
    int s = 0;
    if (N<=8000) {
        // Create a tree-like graph with N nodes
        Graph g(N);
        for (int i=0; i<N-1; i++){
            int node1 = readLong();
            int node2 = readLong();
            unsigned long long int distance = readLong();
            (node1 < node2) ? g.addEdge(node1-1, node2-1, distance) : g.addEdge(node2-1, node1-1, distance);
        }

        for (int i=0; i<N-1; i++){
            int p = readLong();
            int s = readLong();
            g.addCost(p,s);
        }
        g.shortestPath(s);
    }
    else {
        Line l(N);
        for (int i=0; i<N-1; i++){
            int node1 = readLong();
            int node2 = readLong();
            unsigned long long int distance = readLong();
            l.addEdge(node1-1, node2-1, distance);
            
        }
        for (int i=0; i<N-1; i++){
            int p = readLong();
            int s = readLong();
            l.addCost(p,s);
        }
        
        l.shortestLine(s);

    }
    // _CrtDumpMemoryLeaks();
	return 0;
}
