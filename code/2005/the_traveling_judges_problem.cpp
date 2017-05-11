#include <stdio.h>
#include <string.h>
#include <memory.h>

// constraints about maximum cities and judges, defined by problem statement
const int MaxC = 20;
const int MaxJ = 10;

// road[][] stores the graph, judges[] stores the starting city of each judge
// and the back pointers in the spanning tree is stored in fa[]
int cases, NC, NJ, T, DC, map, best_map, ans, i, k;
int road[MaxC][MaxC], judges[MaxJ], fa[MaxC];


// read input about cities, notice that all indices are decremented by 1, to
// fit into those array indices
bool init() {
    int NR, i, C1, C2;
    scanf("%d", &NC); if (NC == -1) return false;
    // T is set to 2^DC, so it can denote a city bitmask
    scanf("%d", &DC); DC--; T = 1 << DC;
    scanf("%d", &NR);

    memset(road, 0, sizeof(road));
    // initialize ans to some large value
    ans = 1000;
    for (i = 0; i < NR; i++) {
        scanf("%d %d", &C1, &C2); C1--; C2--;
        scanf("%d", &road[C1][C2]);
        // the graph is undirected, so road[C2][C1] is same as road[C1][C2]
        road[C2][C1] = road[C1][C2];
        ans += road[C1][C2];
    }

    scanf("%d", &NJ);
    // read in info about all judges and set the masks correctly
    for (i = 0; i < NJ; i++) {
        scanf("%d", &judges[i]); judges[i]--;
        // set bit judge[i] in the city bitmask T, so this city is required
        T |= 1 << judges[i];
    }
    return true;
}


// according to a bitmask that denotes the required cities, calculate the min
// spanning tree using prim's algorithm
void check(int map) {
    int i, j, k, sum, min; int d[MaxC];
    // d[i] = -2 if node i does not connect to the spanning tree, = -1 if node
    // i is inside the spanning tree, or > 0 if it denotes the min distance
    // between point i and the currently constructed spanning tree
    for (i = 0; i < MaxC; i++) d[i] = -2;
    // initialize the destination city (node) to be inside the spanning tree
    d[DC] = -1; sum = 0;

    // iterate over all edges connected to DC, to find a smallest one
    for (i = 0; i < MaxC; i++) if (((1<<i) & map) && road[DC][i]) {
        d[i] = road[DC][i]; fa[i] = DC;
    }
    // body part of prim's algorithm: constructing the minimum spanning tree
    while (true) {
        min = ans + 10;
        // calculate the node k that is closest to the current tree
        for (i = 0; i < MaxC; i++) if (d[i] > 0 && d[i] < min) {
            min = d[i]; k = i;
        }
        // if there's no point to expand, then exit the construction
        if (min > ans) break;
        // select node k into the tree, and accumulate the total tree length
        sum += min; d[k] = -1;
        // update the distances and the back pointers of point k, i
        for (i = 0; i < MaxC; i++) if (((1<<i) & map) && road[k][i]) {
            if (d[i] == -2 || d[i] > road[k][i]) {
                d[i] = road[k][i]; fa[i] = k;
            }
        }
    }

    // if there's no expansion, then this plan is not working; abort
    for (i = 0; i < MaxC; i++) if (((1<<i) & map) && d[i] == -2) return;

    if (sum < ans) {
        // if the tree's length is smaller than the optimal solution, then we
        // update the optimal solution and the corresponding map
        ans = sum; best_map = map;
    } else if (sum == ans) {
        // if the length is equal, then compare how many edges two solutions
        // involve, and select the smaller one
        j = 0; k = 0;
        for (i = 0; i < MaxC; i++) {
            if ((1<<i) & best_map) j++;
            if ((1<<i) & map) k++;
        }
        if (j > k) {
            best_map = map; return;
        }
        if (j < k) return;
        // if i was not previously selected but is selected now, then we know
        // this solution is better, and we update it to be the optimal one
        for (i=0;i<MaxC;i++) {
            if (((1<<i) & best_map) && !((1<<i) & map)) return;
            if (!((1<<i) & best_map) && ((1<<i) & map)) {
                best_map = map; return;
            }
        }
    }
}


int main() {
    cases = 0;

    while (init()) {
        best_map = 0;
        // iterate over all possible maps (city bitmasks)
        for (map = 0; map < (1<<NC); map++) if ((map&T) == T) check(map);
        printf("Case %d: distance = %d\n", ++cases, ans);
        // get the corresponding route to transfer all judges, and output
        check(best_map);
        for (i = 0; i < NJ; i++) {
            printf("	%d", judges[i]+1); k = judges[i];
            while (k != DC) {
                printf("-%d", fa[k]+1); k = fa[k];
            }
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}
