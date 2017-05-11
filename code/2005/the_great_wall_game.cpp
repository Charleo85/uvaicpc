#include <stdio.h>
#include <string.h>

// the max dimension of the board is 15, as stated in the problem statement
const int MaxN = 15;

// a[][] is the adjacency matrix of the bipartite graph, Lx[] and Ly[] are the
// coordinates of possible moves, matchY[] denotes the matching edges
int n, ans, cases;
int a[MaxN+1][MaxN+1], Lx[MaxN+1], Ly[MaxN+1], matchY[MaxN+1];

// usedX[], usedY[] denotes the validity of visiting points in sets X and Y
bool usedX[MaxN+1], usedY[MaxN+1];


// whether there's an augmenting path from node r in the equivalent subgraph
bool path(int r) {
    int i; usedX[r] = true;
    for (i = 0; i < n; ++i) if (Lx[r] + Ly[i] == a[r][i]) if (!usedY[i]) {
        usedY[i] = true;
        if (matchY[i] < 0 || path(matchY[i])) {
            matchY[i] = r; return true;
        }
    }
    return false;
}


// complete the bipartite matching, using a graph that represent the board
void work() {
    int i, j, k, res = 0;
    // calculate initial values for the array Lx[]
    for (i = 0; i < n; ++i)	{
        Lx[i] = -2147483647;
        for (j = 0; j < n; ++j) if (a[i][j] > Lx[i]) Lx[i] = a[i][j];
    }
    // initialize Ly to be zeros, and no initial matching edges (value = -1)
    memset(Ly, 0, sizeof(Ly));
    memset(matchY, -1, sizeof(matchY));

    for (i = 0; i < n; ++i) while (true) {
        // set the nodes in X and Y to be unvisited at the beginning
        memset(usedX, false, sizeof(usedX));
        memset(usedY, false, sizeof(usedY));
        // if there's an augmenting path from i, then break out of the loop
        if (path(i)) break;

        // calculate how much can be improved (denoted by "delta")
        int delta = 2147483647, v;
        for (j = 0; j < n; ++j) if (usedX[j])
            for (k = 0; k < n; ++k) if(!usedY[k])
        {
            v = Lx[j] + Ly[k] - a[j][k]; if (v < delta) delta = v;
        }
        // update the corresponding labels in usedX[] and usedY[]
        for (j = 0; j < n; ++j) if (usedX[j]) Lx[j] -= delta;
        for (j = 0; j < n; ++j) if (usedY[j]) Ly[j] += delta;
    }

    // calculate the sum of weights of matching weights, and update optimum
    for (i = 0; i < n; ++i) if (matchY[i] >= 0) res += a[matchY[i]][i];
    if (res > ans) ans = res;
}


// calculate the distance from point (x, y) to the origin
int dis(int x, int y) {
    if (x * y >= 0) x += y;
    else x -= y;
    if (x > 0) return x; else return -x;
}


int main() {
    int i, j, k; int P[MaxN+1][2];

    while (true) {
        scanf("%d", &n); if (n == 0) break;
        ans = - (n * n + n);
        // we are calculate a maximum for matching, so we negate every value
        // and so we are searching for "minumum negative"
        for (i = 1; i <= n; i++) scanf("%d %d", &P[i][0], &P[i][1]);

        for (i = 1; i <= n; i++) {
            // construct a bipartite map based on the i'th row of the board
            for (j = 1; j <= n; j++) for (k = 1; k <= n; k++)
                a[j-1][k-1] = - dis(P[j][0] - i, P[j][1] - k);
            work();
        }
        for (i = 1; i <= n; i++) {
            // construct a bipartite map based on the i'th column of the board
            for (j = 1; j <= n; j++) for (k = 1; k <= n; k++)
                a[j-1][k-1] = - dis(P[j][0] - k,P[j][1] - i);
            work();
        }

        // construct the bipartite based on the 2 main diagonals of the board
        for (j = 1; j <= n; j++) for (k = 1; k <= n; k++)
            a[j-1][k-1] = - dis(P[j][0] - k,P[j][1] - k);
        work();
        for (j = 1; j <= n; j++) for (k = 1; k <= n; k++)
            a[j-1][k-1] = - dis(P[j][0] - k,P[j][1] - (n - k + 1));
        work();

        printf("Board %d: %d moves required.\n\n", ++cases, -ans);
    }

    return 0;
}
