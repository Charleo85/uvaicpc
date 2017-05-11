#include <stdio.h>
#include <string.h>

// information about an ant
struct info {
    // coordinates: (x, y)
    // length traveled so far: len
    // time waited at an intersection: wait
    // direction it is facing: dir
    int x, y, len, wait, dir;
};

// max 50 line segments, 100 ants, 250 line segment length
const int MaxN = 50, MaxM = 100, MaxL = 250;

// unit vectors for adding coordinates easily
const int Add[4][2] = {-1, 0, 0, 1, 1, 0, 0, -1};

// the i th unit vector has direction route[i]
int route[MaxN * MaxL];

// the latest direction of coordinate (x,y) is map[x][y]
int map[MaxL+1][MaxL+1];

// ants[x][y][k] means there's an ant at (x,y) facing direction k
int ants[MaxL+1][MaxL+1][4];

// global variables
int N, cases, n, m, D, Rs, fin, sta, T, ex, ey;

// the ant at index list[i] is the first ith ant that completes
int list[MaxM];

// the array of ants
info ant[MaxM];


// gets the input for current test case and calculates the route that
// ant Carl has passed through
void init() {
    int i, j, k, t, x1, y1, x2, y2;
    scanf("%d %d %d", &n, &m, &D);

    x1 = y1 = 0; Rs = 0;
    // calculate the route Carl has passed through
    for (i = 0; i < n; i++) {
        scanf("%d %d", &x2, &y2);

        // determine the direction of the line segment
        if (x1 > x2) t = 0;
        else if (y1 < y2) t = 1;
        else if (x1 < x2) t = 2;
        else t = 3;

        // set the values for "route" in the current line segment
        while (x1 != x2 || y1 != y2) {
            route[Rs] = t; Rs++;
            x1 += Add[t][0]; y1 += Add[t][1];
        }
    }

    // no ants have finished, and no ants have started
    fin = 0; sta = 0;
    // initialize the array of ants
    for (i = 0; i <= MaxL; i++) for (j = 0; j <= MaxL; j++) {
        for (k = 0; k < 4; k++) ants[i][j][k] = -1;
    }
}


// simulate one time step of ant movement
void work() {
    int ok, i, j, x, y, d, p, tmp, m1, m2, m3;
    // go[i] means whether ant i can possibly move (0) or not (1)
    int go[MaxM]; memset(go, 0, sizeof(go));

    // how many ants have finished so far
    ok = 0;
    for (i = 0; i < sta; i++) if (ant[i].x < 0) ok++;

    // whether an ant can move based on its priority
    for (i = 0; i < sta; i++) if (ant[i].x >= 0) {
        x = ant[i].x; y = ant[i].y;
        for (j = 0; j < 4; j++) {
            if (ants[x][y][j] >= 0) {
                tmp = ants[x][y][j];
                // wait longer, or same wait but travels longer
                m1 = (ant[tmp].wait > ant[i].wait);
                m2 = (ant[tmp].wait == ant[i].wait);
                m3 = (ant[tmp].len > ant[i].len);

                // if ant "tmp" has higher priority than ant "i",
                // then ant "i" cannot move
                if (m1 || (m2 && m3)) {
                    p = 1; go[i] = 1; ok++;
                    break;
                }
            }
        }
    }

    // if there's an ant that blocks the way, some more ants might
    // cannot move, handle them in this part
    do {
        ok = 0;
        for (i = 0; i < sta; i++) {
            if (!go[i] && ant[i].x >= 0) {
                // the movement direction of ant "i"
                d = map[ant[i].x][ant[i].y];

                // the new coordinate of ant "i"
                x = ant[i].x + Add[d][0];
                y = ant[i].y + Add[d][1];

                // if there's some ant that blocks the way, then
                // ant "i" cannot move, should be updated
                if (ants[x][y][d] >= 0 && go[ants[x][y][d]] == 1) {
                    go[i] = 1; ok = 1;
                    continue;
                }
            }
        }
    } while (ok);

    for (i = 0; i < sta; i++) {
        if (!go[i] && ant[i].x >= 0) {
            // update the ant that can actually be moved
            ant[i].len++; ant[i].wait = 0;
            ants[ant[i].x][ant[i].y][ant[i].dir] = -1;
        } else if (ant[i].x >= 0) {
            // or otherwise the wait time should be incremented
            ant[i].wait++;
        }

        for (i = 0; i < sta; i++) {
            if (!go[i] && ant[i].x >= 0) {
                x = ant[i].x; y = ant[i].y;
                // if the start point is clear, add a new ant
                if (x == 100 && y == 100 && i != sta-1) {
                    ants[100][100][map[100][100]] = i+1;
                }
                // update the current ant's info
                d = map[x][y];
                ant[i].x += Add[d][0];
                ant[i].y += Add[d][1];
                ant[i].dir = d;
                // if some ant has reached the dest, remove it
                if (ant[i].x == ex && ant[i].y == ey) {
                    list[fin] = i; fin++;
                    ant[i].x = -1;
                } else {
                    ants[ant[i].x][ant[i].y][d] = i;
                }
            }
        }
    }
}


// output the solution of the current test case
void write() {
    int i;
    printf("Case %d:\n", cases);
    printf("Carl finished the path at time %d\n", ant[0].len+1);
    printf("The ants finish in the following order:\n");
    printf("%d", list[0]);
    for (i = 1; i < m; i++) printf(" %d", list[i]);
    printf("\n");
    printf("The last ant finished the path at time %d\n", T+1);
    if (cases < N) printf("\n");
}


int main() {
    int X, Y;
    scanf("%d", &N);

    for (cases = 1; cases <= N; cases++) {
        init();
        // array indices are made positive by adding 100
        X = 100; Y = 100; ex = -1; ey = -1;

        for (T = 0; fin < m; T++) {
            // update map indices based on Carl's route
            if (T < Rs) {
                map[X][Y] = route[T];
                X += Add[route[T]][0]; Y += Add[route[T]][1];
                if (T == Rs - 1) {
                    ex = X; ey = Y;
                }
            }
            // if there's some ant that has just started
            if (T % D == 0 && sta < m) {
                if (ants[100][100][map[100][100]] < 0)
                    ants[100][100][map[100][100]] = sta;
                ant[sta].x = 100; ant[sta].y = 100;
                ant[sta].len = 0; ant[sta].wait = 0;
                ant[sta].dir = map[100][100];
                sta++;
            }

            work();
        }

        write();
    }

    return 0;
}
