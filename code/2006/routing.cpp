#include <stdio.h>

// several constraints, defined by the problem statements
const int MaxN = 100;
const int Max = 1000;

// dis[x][y] is the shortest path from x to y, tr[] is the segment tree, used
// for getting the smallest answer and book-keeping when answer is updated,
// ans[x][y] means there are at least this many nodes in paths 1->x and 1->y
int dis[MaxN][MaxN], tr[MaxN * MaxN * 3], ans[MaxN][MaxN];

// other necessary global variables for this program
int Cases, n, m, i, j, k, x, y;


// from the root node n and its corresponding interval, set all values to Max
// in the segment tree
void Make(int n, int x, int y) {
    tr[n] = Max;
    if (x == y) return;
    Make(n + n, x, (x + y) / 2);
    Make(n + n + 1, (x + y) / 2 + 1, y);
}


// from the root node n, update node ch's value to data and book-keeping
void Change(int n, int x, int y, int ch, int data) {
    if (x == y) {
        tr[n] = data;
    } else if ((x+y) / 2 >= ch) {
        Change(n + n, x, (x + y) / 2, ch, data);
        if (tr[n+n] < tr[n+n+1]) tr[n] = tr[n + n];
        else tr[n] = tr[n + n + 1];
    } else {
        Change(n + n + 1, (x + y) / 2 + 1, y, ch, data);
        if (tr[n+n] < tr[n+n+1]) tr[n] = tr[n + n];
        else tr[n] = tr[n + n + 1];
    }
}


// get the position of the min value in the segment tree
int Get_Min(int n, int x, int y) {
    if (x == y) return x;
    if (tr[n+n] < tr[n+n+1]) return Get_Min(n + n, x, (x + y) / 2);
    else return Get_Min(n + n + 1, (x + y) / 2 + 1, y);
}


int main() {
    Cases = 0;

    while (scanf("%d %d", &n, &m) && n) {
        // initialize the shortest path lengths, arrays index is 0..(n-1)
        for (i = 0; i < n; i++) for (j = 0; j < n; j++) dis[i][j] = Max;
        // construct the adjacency matrix
        for (i = 0; i < m; i++) {
            scanf("%d %d", &x, &y); dis[x - 1][y - 1] = 1;
        }
        // any shortest path length from and to the same node is 0
        for (i = 0; i < n; i++) dis[i][i] = 0;

        // use floyd-warshall algorithm to compute the shortest path length
        // for any pair of nodes
        for (k = 0; k < n; k++)
            for (i = 0; i < n; i++) for (j = 0; j < n; j++)
                if (dis[i][j] > dis[i][k] + dis[k][j])
                    dis[i][j] = dis[i][k] + dis[k][j];

        // initialize the segment tree and change the value of first index
        Make(1, 0, n * n - 1);
        Change(1, 0, n * n - 1, 0, 1);
        // ans[0][0] = 1other values of ans[][] on the diagonal should be Max
        for (i = 0; i < n; i++) for (j = 0; j < n; j++) ans[i][j] = Max;
        ans[0][0] = 1;

        while (tr[1] < Max) {
            // get the interval corresponding to the min value in the tree
            x = Get_Min(1, 0, n * n - 1);
            y = x % n; x /= n;
            // case 3: x and y are all repeated points, then exit the loop
            if (x == 1 && y == 1) break;

            if (x != y && ans[y][x] <= Max &&
                ans[x][y] + dis[x][y] - 1 < ans[y][x])
            {
                ans[y][x] = ans[x][y] + dis[x][y] - 1;
                Change(1, 0, n * n - 1, y * n + x, ans[y][x]);
            }

            // iterate over all points in the interval [x, y], not equal
            for (i = 0; i < n; i++) if (i != x && i != y) {
                // case 1: the next point of x is not repeated
                if (dis[x][i] == 1 && ans[i][y] <= Max &&
                    ans[x][y] + 1 < ans[i][y])
                {
                    ans[i][y] = ans[x][y] + 1;
                    Change(1, 0, n * n - 1, i * n + y, ans[i][y]);
                }

                // case 2: the previous point of y is not repeated
                if (dis[i][y] == 1 && ans[x][i] <= Max &&
                    ans[x][y] + 1 < ans[x][i])
                {
                    ans[x][i] = ans[x][y] + 1;
                    Change(1, 0, n * n - 1, x * n + i, ans[x][i]);
                }
            }

            // case 4: x is repeated and is also the previous point of y
            if (dis[x][y] == 1 && ans[y][y] <= Max && ans[x][y] < ans[y][y]) {
                ans[y][y] = ans[x][y];
                Change(1, 0, n * n - 1, y * n + y, ans[x][y]);
            }

            // case 5: y is repeated and is also the next point of x
            if (dis[x][y] == 1 && ans[x][x] <= Max && ans[x][y] < ans[x][x]) {
                ans[x][x] = ans[x][y];
                Change(1, 0, n * n - 1, x * n + x, ans[x][y]);
            }

            // mark the values of ans already used in the recursion
            ans[x][y] = Max + 1;
            Change(1, 0, n * n - 1, x * n + y, ans[x][y]);
        }

        printf("Network %d\n", ++Cases);
        if (ans[1][1] == Max) printf("Impossible\n\n");
        else printf("Minimum number of nodes = %d\n\n", ans[1][1]);
    }

    return 0;
}
