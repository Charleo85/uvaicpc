#include <stdio.h>
#include <algorithm>

using namespace std;

// MaxS is the maximum number of streets, MaxN is the maximum coordinates
const int MaxS = 500;
const int MaxN = 500 * 2 + 4;

// the unit vector in 4 horizontal and vertical directions, for simplicity
const int add[4][2] = {0,1,  0,-1, -1,0, 1,0};

// home coordinates (xh, yh), university coordinates (xu, yu), answer is ans
int cases, xh, yh, xu, yu, n, N, q1, q2, q3, ans, i;

// grid to push flow over the graph, so we could determine the mininum number
// of streets Peter has to cross
int coor[2][MaxS * 2 + 4];
int w[MaxS * 2 + 4][MaxS * 2 + 4];
int list[MaxN * MaxN][2];

// if there's a street blocking the way, represented by a 4-bit bitmask
int map[MaxN][MaxN];


// binary search over the array coor[type] for an edge with index value T
int find(int type,int T) {
    int l, r, m; l = 0; r = n + n + 1;
    while (l != r) {
        m = (l + r) / 2;
        if (coor[type][m] < T) l = m + 1; else r = m;
    }
    return l;
}


// read input and set up the coordinates and the flow graph
bool init() {
    int i, j, k, tmp; int streets[MaxS][4];
    scanf("%d", &n); if (n == 0) return false;

    // read in each street's original coordinates
    for (i = 0; i < n; i++){
        for (j = 0; j < 4; j++) scanf("%d", &streets[i][j]);
        coor[0][i] = streets[i][0];
        coor[1][i] = streets[i][1];
        coor[0][i + n] = streets[i][2];
        coor[1][i + n] = streets[i][3];
    }
    // read in peter's home and university coordinates
    scanf("%d %d %d %d", &xh, &yh, &xu, &yu);
    coor[0][n + n] = xh; coor[1][n + n] = yh;
    coor[0][n + n + 1] = xu; coor[1][n + n + 1] = yu;

    // sort all coordinates; shift all coordinates right and up by 1, so we
    // could leave space for Peter to start his walk
    sort(coor[0],coor[0] + n + n + 2);
    sort(coor[1],coor[1] + n + n + 2);

    // normalize, ensure that x1 < x2 or y1 < y2, for convenience purposes
    for (i = 0; i < n; i++) {
        if (streets[i][0] < streets[i][2]) {
            tmp = find(0, streets[i][0]) + 1;
            streets[i][2] = find(0, streets[i][2]) + 1;
            streets[i][0] = tmp;
        } else {
            tmp = find(0, streets[i][2]) + 1;
            streets[i][2] = find(0, streets[i][0]) + 1;
            streets[i][0] = tmp;
        }
        if (streets[i][1] < streets[i][3]) {
            tmp = find(1, streets[i][1]) + 1;
            streets[i][3] = find(1, streets[i][3]) + 1;
            streets[i][1] = tmp;
        } else {
            tmp = find(1, streets[i][3]) + 1;
            streets[i][3] = find(1, streets[i][1]) + 1;
            streets[i][1] = tmp;
        }
    }

    // get the new coordinates of university and home after normalizing
    xh = find(0, xh) + 1; yh = find(1, yh) + 1;
    xu = find(0, xu) + 1; yu = find(1, yu) + 1;

    // the range now is actually 0 to (n + n + 3) in each dimension
    N = n + n + 3;
    // draw the streets on the graph, and represent if there's a street using
    // 4-bit binary values (bitmasks)
    memset(map, 0, sizeof(map));

    for (i = 0; i < n; i++) {
        // handle horizontal and vertical streets separately
        if (streets[i][0] == streets[i][2]) {
            for (j = streets[i][1]; j < streets[i][3]; j++) {
                map[streets[i][0]][j] |= 4;
                map[streets[i][0] - 1][j] |= 8;
            }
        } else {
            for (j = streets[i][0]; j < streets[i][2]; j++) {
                map[j][streets[i][1]] |= 2;
                map[j][streets[i][1] - 1] |= 1;
            }
        }
    }
    return true;
}


// simulate the scenario if Peter wants to finish crossing under t streets
void go(int x,int y,int t) {
    int i, h, l;
    // try to push flow in all the 4 directions
    for (i = 0; i < 4; i++) if (!(map[x][y] & (1<<i)) || t) {
        h = x + add[i][0]; l = y + add[i][1];
        if (h < 0 || l < 0 || h > N || l > N || w[h][l]) continue;

        w[h][l] = 1;  list[q3][0] = h; list[q3][1] = l; q3++;
        if (map[x][y] & (1<<i)) go(h, l, t-1);
        else go(h, l, t);
    }
}


int main() {
    cases = 0;

    while (init()) {
        memset(w, 0, sizeof(w)); w[xh][yh] = 1;
        list[0][0] = xh; list[0][1] = yh;
        // push flow from the starting point, not crossing any streets now
        q3 = 1; go(xh, yh, 0);
        // when doing BFS, q1 is the source, q2 is the sink, and q3 is sink
        // for the next level of flow
        q1 = 0; q2 = q3; ans = 0;

        while (true) {
            if (w[xu][yu]) break;
            // from the previous level, try to push flow one level further
            for (i = q1; i < q2; i++) go(list[i][0], list[i][1], 1);
            q1 = q2; q2 = q3; ans++;
        }

        printf("City %d\n", ++cases);
        printf("Peter has to cross %d streets\n", ans);
    }

    return 0;
}
