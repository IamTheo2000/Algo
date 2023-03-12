// https://www.geeksforgeeks.org/find-largest-median-of-a-sub-array-with-length-at-least-k/
#include <stdio.h>
#include <iostream>
#include <vector>
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

bool good(vector<int> arr, int& N, int& K,
          int& median)
{
    vector<int> pre(N);
    for (int i = 0; i < N; i++) {
        if (arr[i] >= median)
            pre[i] = 1;
        else
            pre[i] = -1;
 
        if (i > 0)
            pre[i] += pre[i - 1];
    }

    int mx = pre[K - 1];
    int mn = 0;
 
    for (int i = K; i < N; i++) {
        mn = min(mn, pre[i - K]);
        mx = max(mx, pre[i] - mn);
    }
    if (mx > 0)
        return true;
    return false;
}

int maxMedian(vector<int> arr, int N, int K)
{

    int l = 1, r = N + 1;

    int mx_median = -1;
 
    while (l <= r) {
        int mid = (l + r) / 2;
        if (good(arr, N, K, mid)) {
            mx_median = mid;
            l = mid + 1;
        }
        else
            r = mid - 1;
    }
    return mx_median;
}

int main () {
    int N,K;
    N = readLong();
    K = readLong();

    vector<int> seq(N);
    for (int i; i<N; i++)
    {   
        int value = readLong();
        seq[i] = value ;
    }

    cout << maxMedian(seq, N, K) << endl;
    return 0;   
}