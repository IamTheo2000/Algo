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
// Function to traverse the DAG
// and apply Dynamic Programming
// to find the longest path
void dfs(int node, vector<vector<int>>& adj, vector<int>& dp, vector<bool>& vis)
{
	// Mark as visited
	vis[node] = true;

	// Traverse for all its children
	for (int i = 0; i < adj[node].size(); i++) {

		// If not visited
		if (!vis[adj[node][i]])
			dfs(adj[node][i], adj, dp, vis);

		// Store the max of the paths
		dp[node] = max(dp[node], 1 + dp[adj[node][i]]);
	}
}

// Function that returns the longest path
int findLongestPath(vector<vector<int>> adj, int n)
{
	// Dp array
	vector<int> dp(n);
	std::fill(dp.begin(), dp.end(), 0);

	// Visited array to know if the node
	// has been visited previously or not
	vector<bool> vis(n);
    std::fill(vis.begin(), vis.end(), false);

	// Call DFS for every unvisited vertex
	for (int i = 1; i <= n; i++) {
		if (!vis[i])
			dfs(i, adj, dp, vis);
	}

	int ans = 0;

	// Traverse and find the maximum of all dp[i]
	for (int i = 1; i <= n; i++) {
		ans = max(ans, dp[i]);
	}
	return ans;
}

// Driver Code
int main()
{   
    int N,K;
    N = readLong();
    K = readLong();

    vector<int> seq(N);
    for (int i=0; i<N; i++)
    {   
        int value = readLong();
        seq[i] = value ;
    }

	vector<vector<int>> adj(2*N);
    for(int i=0; i<N; i++) {
		for (int j=i+1; j<N; j++) {
			if (seq[i] < seq[j]) {
                adj[i].push_back(j);
                adj[i+N].push_back(j+N);
			}
			if (seq[j] > seq[i] - K) {
                adj[i+N].push_back(j);
			}

		}
    }
    
	cout << findLongestPath(adj, 2*N);
	return 0;
}