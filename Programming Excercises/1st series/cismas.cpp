#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include<tuple>
#define BSIZE 1<<15

using namespace std;

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

// This unordered multimap will contain all subsequences sums that are less than K
// in the following form: sum:(length,finish,start)
// e.g. for sequence [1,2,3] with K = 4
// multimap = {1:(1,0,0),2:(1,0,0),3:(1,0,0):3:(2,1,0)}
unordered_multimap<int, tuple<int,int,int>> sums;

// Function to read from stdin
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
// Function to find all subsequences with sum less than K
// and fill the unordered multimap
// Insipration from https://www.geeksforgeeks.org/number-subarrays-sum-less-k/
void countSubarrays(vector<int> arr, int n, int k)
{
    int start = 0, end = 0, count = 0, sum = arr[0];
 
    while (start < n && end < n) {
 
        if (sum < k) {
            end++;
 
            if (end >= start) {
                int temp = sum;
                for(int i=start+1; i<=end; i++)
                {
                    // cout << "Inserted: " << temp << ", " << end << ", "<< i << endl;
                    sums.insert({temp, make_tuple(end-i+1,end,i)});
                    temp -= arr[i-1];
                }
            }
            if (end < n)
                sum += arr[end];
        }
 
        else {
            sum -= arr[start];
            start++;
        }
    }
}

int main () {
    int N,K;
    N = readLong();
    K = readLong();
    int minresult = 2147483647;
    vector<int> seq(N);
    for (int i; i<N; i++)
    {   
        int value = readLong();
        seq[i] = value ;
    }
    countSubarrays(seq, N, K+1);
    for (auto const &pair: sums) {
        // std::cout << "{" << pair.first << ": (" << get<0>(pair.second) << "," << get<1>(pair.second) << "," 
        // << get<2>(pair.second) << ")}";

        // if you found a key in multimap with sum K
        if (pair.first == K){
            minresult = min(get<0>(pair.second),minresult);
        }

        // If key from multimap less than K look for complement
        if (sums.find(K-pair.first) != sums.end()){
        //     sums.erase(pair.first);
        //}  else {
            auto its = sums.equal_range(K-pair.first);
            for (auto it = its.first; it != its.second; ++it) {
                if(!(get<2>(pair.second) <= get<1>(it->second) && get<1>(it->second) <= get<2>(pair.second)))
                {
                    minresult = min(minresult,get<0>(pair.second) + get<0>(it->second));
                }
            }
        }
    }
    cout << (minresult == 2147483647 ? -1 : minresult) << endl;
    return 0;   
}