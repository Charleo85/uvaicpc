#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

// constraints, and necessary global variables for the program
const int MaxP = 50;
int P, R, cases, i, j, k, sum, ans;

// name[] is the list of names, and sum denotes its length
string name[MaxP], s1, s2;

// adjacency matrix to represent the graph
int a[MaxP][MaxP];


// given a name and return its node index, if the name has not appeared before
// then update the list of names
int find(string s) {
    int i;
    for (i = 0; i < sum; i++) if (name[i] == s) return i;
    name[sum] = s; sum++; return sum - 1;
}


// read input and initialize all variables to initial values
bool init() {
    scanf("%d %d", &P, &R);
    if (P == 0 && R == 0) return false;

    sum = 0; memset(a, 0, sizeof(a));
    // constructing the adjacency matrix
    for (i = 0; i < R; i++) {
        cin >> s1 >> s2;
        j = find(s1); k = find(s2); a[j][k] = a[k][j] = 1;
    }
    return true;
}


int main() {
    cases = 0;

    while (init()) {
        // use floyd-warshall algorithm to compute the shortest path between
        // any pair of nodes in the graph
        for (k = 0; k < P; k++)
            for (i = 0; i < P; i++) for (j = 0; j < P; j++)
                if (a[i][k] && a[k][j])
                    if (a[i][j] == 0 || a[i][j] > a[i][k] + a[k][j])
                        a[j][i] = a[i][j] = a[i][k] + a[k][j];

        ans = 0;
        // if there are node pairs that can never be reached, then the graph
        // is not connected, or the max degree of separation is the greatest
        // value of length of all shortest paths
        for (i = 0; i < P; i++) for (j = i+1; j < P; j++) {
            if (a[i][j] == 0) ans = P + 10;
            else if (a[i][j] > ans) ans = a[i][j];
        }

        if (ans > P) printf("Network %d: DISCONNECTED\n\n", ++cases);
        else printf("Network %d: %d\n\n", ++cases, ans);
    }

    return 0;
}
