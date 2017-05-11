#include <stdio.h>
#include <string.h>
#include <algorithm>

using std::max; using std::min;

// information about a map: its numbers of rows and columns, and its features
struct MAPS {
    int n, m;
    char feature[100][100];
};

// info about merging two maps, including its scores and the offset in the
// horizontal and vertical directions
struct Merge {
    int score, r, c;
};

// the current row of a particular map
char s[2000];

// there are n maps in total, and the highest merging score is denoted by max
int cases, n, i, j, k, sum, maxx, x, y;

// an array of all maps (original)
MAPS map[20];

// merge[x][y] is the score of merging maps x and y
Merge merge[20][20];

// the remaining signs of those maps
bool sign[20];


// calculate the merging score of two characters, 0 for no feature, 1 for any
// matching feature, and -10000 if they do not match
int calc(char a, char b) {
    if (a == '-' || b == '-') return 0;
    if (a == b) return 1;
    return -10000;
}


// try to merge maps x and y, and calculate the info denoted by struct Merge
Merge Try(int x, int y) {
    int r, c, i, j, k, sc, x1, y1, x2, y2;
    Merge tmp;

    // initialize the score to be 0
    tmp.score = 0;
    for (r = -map[y].n+1; r < map[x].n; r++) {
        for (c = -map[y].m+1; c < map[x].m; c++) {
            // calculate the vertices of merging maps x and y
            x1 = max(0, r); y1 = max(0, c);
            x2 = min(map[x].n, map[y].n+r); y2 = min(map[x].m, map[y].m+c);

            // calculate the score of merging
            sc = 0;
            for (i = x1; i < x2; i++) for (j = y1; j < y2; j++)
                sc += calc(map[x].feature[i][j], map[y].feature[i-r][j-c]);

            // update the optimal score of merging
            if (sc > tmp.score) {
                tmp.score = sc; tmp.r = r; tmp.c = c;
            }
        }
    }
    return tmp;
}


// merge maps indicated by indices x and y into a new map with index z
void add_map(int x, int y, int z) {
    int i, j, k, Dx, Dy;
    // calculate the offset of map x in the process of merging
    Dx = max(-merge[x][y].r, 0); Dy = max(-merge[x][y].c, 0);
    // calculate the dimensions of map z
    map[z].n = max(map[x].n, map[y].n+merge[x][y].r) + Dx;
    map[z].m = max(map[x].m, map[y].m+merge[x][y].c) + Dy;

    // fill the feature map of z to have no features
    for (i = 0; i < map[z].n; i++) for (j = 0; j < map[z].m; j++)
        map[z].feature[i][j] = '-';

    // fill the region of the feature map with features from map x
    for (i = 0; i < map[x].n; i++) for (j = 0; j < map[x].m; j++)
        map[z].feature[i+Dx][j+Dy] = map[x].feature[i][j];

    // update the indices to reflect the offset
    Dx += merge[x][y].r; Dy += merge[x][y].c;

    // fill the region of the feature map with features from map y
    for (i = 0; i < map[y].n; i++) for (j = 0; j < map[y].m; j++)
        if (map[y].feature[i][j] != '-')
            map[z].feature[i+Dx][j+Dy] = map[y].feature[i][j];
}


int main() {
    for (cases = 1; ; cases++) {
        scanf("%d", &n); if (n == 0) break;
        // read input about all original maps
        for (i = 1; i <= n; i++) {
            scanf("%d %d ", &map[i].n, &map[i].m);
            for (j = 0; j < map[i].n; j++) {
                scanf("%s", &s);
                for (k = 0; k < map[i].m; k++) map[i].feature[j][k] = s[k];
            }
        }

        // calculate merging info about any two given maps
        for (i = 1; i <= n; i++) for (j = i+1; j <= n; j++)
            merge[i][j] = Try(i, j);
        // initialize all maps to be usable in the sign[] array
        memset(sign, 1, sizeof(sign));

        while (true) {
            maxx = 0;
            // find two maps with the highest merging score among all maps
            for (i = 1; i <= n; i++) if (sign[i])
            for (j = 1; j <= n; j++) if (sign[j]) {
                if (merge[i][j].score > maxx) {
                    maxx = merge[i][j].score;
                    x = i; y = j;
                }
            }
            // if the max merging score is 0, then process completed, exit
            if (maxx == 0) break;

            // maps that have already been merged are not usable any more
            sign[x] = 0; sign[y] = 0;
            add_map(x, y, ++n);
            // calculate the new map's merging score against all others
            for (i = 1; i < n; i++) if (sign[i])
                merge[i][n] = Try(i, n);
        }

        // output resulting map according to format
        if (cases > 1) printf("\n");
        printf("Case %d:\n", cases);
        x = 0;
        for (i = 1; i <= n; i++) if (sign[i]) {
            if (x) printf("\n"); x++;
            printf("   MAP %d:\n", i);

            printf("   +");
            for (j = 0; j < map[i].m; j++) printf("-");
            printf("+\n");

            for (j = 0; j < map[i].n; j++) {
                printf("   |");
                for (k = 0; k < map[i].m; k++)
                    printf("%c", map[i].feature[j][k]);
                printf("|\n");
            }

            printf("   +");
            for (j = 0; j < map[i].m; j++) printf("-");
            printf("+\n");
        }
    }

    return 0;
}
