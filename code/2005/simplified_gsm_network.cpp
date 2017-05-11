#include <stdio.h>
#include <math.h>
#include <memory.h>

// the struct to represent a point
struct coor	{
    double x,y;
};

// some limits of unit towers, and cities, given by the problem statement
const int MaxB = 50;
const int MaxC = 50;
const int MaxS = 50 * 50;

// BTS[] is the array of BTS towers, and city[] is the array of cities, S[] is
// the matrix to store the shortest paths
int B, C, R, Q, i, j, k, cases;
coor BTS[MaxB], city[MaxC];	int S[MaxC][MaxC];


// calculate the distance between point (x1, y1) and point (x2, y2)
double dis(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


// calculate the nearest unit's id "k", from a given point (x, y)
int area(double x, double y) {
    int i, k; double min, tmp;
    // initialize min distance to be the distance from (x, y) to unit 0 (k=0)
    min = dis(x, y, BTS[0].x, BTS[0].y); k = 0;
    // iterate over each unit tower and find out the smallest distance
    for (i = 1; i < B; i++)	{
        tmp = dis(x, y, BTS[i].x, BTS[i].y);
        if (tmp < min) { min = tmp; k = i; }
    }
    return k;
}


// use binary search to calculate how many units have been passed on the path
// from (x1, y1) to (x2, y2)
int get_switch(double x1, double y1, double x2, double y2) {
    // if the closest unit are the same for two points, then distance is 0
    if (area(x1, y1) == area(x2, y2)) return 0;
    // if the two points are actually the same point, then distance is 1
    if (dis(x1, y1, x2, y2) < 1e-6) return 1;

    // handle the left part (closer to (x1, y1)) and the right part (closer to
    // (x2, y2)) separately and recursively
    return get_switch(x1, y1, (x1+x2) / 2, (y1+y2) / 2)
        + get_switch((x1+x2) / 2, (y1+y2) / 2, x2, y2);
}


// read input about a GSM network, and initialize global variables
bool init() {
    scanf("%d %d %d %d", &B, &C, &R, &Q);
    if (B == 0 && C == 0 && R == 0 && Q == 0) return false;
    for (i = 0; i < B; i++) scanf("%lf %lf", &BTS[i].x, &BTS[i].y);
    for (i = 0; i < C; i++) scanf("%lf %lf", &city[i].x, &city[i].y);

    memset(S, 0, sizeof(S));
    for (i = 0; i < R; i++)	{
        // store array indices should be 1 less than the id range from 1 to R
        scanf("%d %d", &j, &k); j--; k--;
        S[j][k] = S[k][j] = 1;
    }
    // calculate how many units between cities i and j, if no paths, then set
    // the distance to be the maximum value
    for (i = 0; i < C; i++) for (j = i+1; j < C; j++) {
        if (S[i][j] == 0) S[i][j] = MaxS;
        else S[i][j] = get_switch(city[i].x, city[i].y, city[j].x, city[j].y);
        S[j][i] = S[i][j];
    }
    // the distance between city i and itself is always equal to 0
    for (i = 0; i < C; i++) S[i][i] = 0;

    return true;
}


int main() {
    cases = 0;

    while (init()) {
        printf("Case %d:\n", ++cases);
        // calculate the shortest path based on the floyd-warshall algorithm
        for (k = 0; k < C; k++)
            for (i = 0; i < C; i++) for (j = 0; j < C; j++)
                if (S[i][j] > S[i][k] + S[k][j]) S[i][j] = S[i][k] + S[k][j];
        // based on the request, determine if it is possible, output results
        for (i = 0; i < Q; i++) {
            scanf("%d %d", &j, &k); j--; k--;
            if (S[j][k] >= MaxS) printf("Impossible\n");
            else printf("%d\n", S[j][k]);
        }
    }

    return 0;
}
