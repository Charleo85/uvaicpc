#include <stdio.h>
#include <string.h>

// the struct to represent info about a crease, sign means whether this is an
// external (1) crease or not (0), if it's external, then sum counts how many
// pieces of paper are visible, if it's internal, then up, down count how many
// pieces of paper are visible from up and down
struct info	{
    int sign, up, down, uCover, dCover, sum;
};

// the maximum number of crease along a single direction
const int MaxN = 64;

// hor[][] records horizontal creases, and ver[][] records vertical creases,
// Nhor[] and Nver[] records the information after a fold
info hor[MaxN+2][MaxN+1], ver[MaxN+1][MaxN+2];
info Nhor[MaxN+2][MaxN+1], Nver[MaxN+1][MaxN+2];

// Lhor[i] = k means the horizontal crease with index i is the k'th crease in
// the order, same idea for Lver
int Lhor[MaxN+1], Lver[MaxN+1];

// other global variables necessary for the program
int Cases, N, n, m, K, ans; char dir;


// initialize all variables
void init()	{
    int i;
    memset(hor, 0, sizeof(hor));
    memset(ver, 0, sizeof(ver));
    // mark all creases as external creases, and visible pieces equal to 1
    for (i = 0; i < n; i++) {
        hor[0][i].sign = hor[n][i].sign = 1;
        hor[0][i].sum = hor[n][i].sum = 1;
        ver[i][0].sign = ver[i][n].sign = 1;
        ver[i][0].sum = ver[i][n].sum = 1;
    }
    // the crease indices are ordered from up to bottom and from left to right
    for (i = 0; i <= n; i++) Lhor[i] = Lver[i] = i;
}


// fold crease x along line in the direction dir, and calculates the index of
// this particular crease in the new picture
int get(int x, int s, int line, char dir) {
    if (dir == 'D' || dir == 'R') {
        return x-line > 0 ? x-line : line-x;
    } else if (line + line >= s) {
        if (x <= line) return x;
        return line - (x - line);
    } else {
        if (x >= line) return s - x;
        return (s - line) - line + x;
    }
}


// simulate a time step of folding the paper
void work() {
    int i, j, k, nNew, mNew, x, y; int line; char dir;
    scanf("%d%c", &line, &dir);
    while (dir!='U' && dir!='D' && dir!='R' && dir!='L') scanf("%c", &dir);

    // determine which line the crease follows, calculate the new paper's size
    // and re-index all the creases after the fold
    if (dir == 'U' || dir == 'D') {
        line = Lhor[line]; mNew = m;
        if (line > n - line) nNew = line;
        else nNew = n - line;
        for (i = 0; i <= N; i++) Lhor[i] = get(Lhor[i], n, line, dir);
    } else {
        line = Lver[line]; nNew = n;
        if (line > m - line) mNew = line;
        else mNew = m - line;
        for (i = 0; i <= N; i++) Lver[i] = get(Lver[i], m, line, dir);
    }

    // record which creases are external in the new picture
    memset(Nhor, 0, sizeof(Nhor)); memset(Nver, 0, sizeof(Nver));
    for (i = 0; i < nNew; i++) Nver[i][0].sign = Nver[i][mNew].sign = 1;
    for (i = 0; i < mNew; i++) Nhor[0][i].sign = Nhor[nNew][i].sign = 1;

    // for each fold, relate it with the last picture, and accumulate values
    if (dir == 'U') for (i = 0; i < mNew; i++) Nhor[nNew][i].sum = 1;
    if (dir == 'D') for (i = 0; i < mNew; i++) Nhor[0][i].sum = 1;
    if (dir == 'L') for (i = 0; i < nNew; i++) Nver[i][mNew].sum = 1;
    if (dir == 'R') for (i = 0; i < nNew; i++) Nver[i][0].sum = 1;

    // handle all the horizontal creases
    for (i = 0; i <= n; i++) for (j = 0; j < m; j++) {
        // determine new indices for crease (i, j)-(i, j+1) in old picture
        x = i; y = j;
        if (dir == 'U' || dir == 'D') x = get(x, n, line, dir);
        else y = get(y, m, line, dir);
        // creases (x, y)-(x, y+1) could have coordinates (x', y')-(x', y'-1)
        // after folding the paper
        if ((dir == 'R' && j < line) || (dir == 'L' && j >= line)) y--;

        // some creases change to external creases after the fold
        if (Nhor[x][y].sign) {
            // if already external, then accumulate the total number, if it
            // is internal, then turn the bottom piece to the external side
            if (hor[i][j].sign) Nhor[x][y].sum += hor[i][j].sum;
            else Nhor[x][y].sum += hor[i][j].down;
        } else {
            // some external crease changes to internal
            if (hor[i][j].sign)	{
                // divide into two cases: whether the piece falls into the
                // upper or the lower part of the new crease
                if ((i < line && dir == 'U') || (i >= line && dir == 'D'))
                    Nhor[x][y].down += hor[i][j].sum;
                else Nhor[x][y].up += hor[i][j].sum;
            } else {
                // preserve the number of visible pieces, and record that the
                // crease's lower part has been covered
                if ((i < line && dir == 'U') || (i >= line && dir == 'D') ||
                    (j < line && dir == 'L') || (j >= line && dir == 'R'))
                {
                    // if this crease isn't covered, then we know the visible
                    // pieces are still visible, and accumulate the 2 values
                    Nhor[x][y].down = hor[i][j].down; Nhor[x][y].dCover = 1;
                    if (!Nhor[x][y].uCover) Nhor[x][y].up += hor[i][j].up;
                } else {
                    // similar to above case, handle the upper part
                    Nhor[x][y].up = hor[i][j].down; Nhor[x][y].uCover = 1;
                    if (!Nhor[x][y].dCover) Nhor[x][y].down += hor[i][j].up;
                }
            }
        }
    }

    // handle all vertical creases, refer to the above loop for explanations
    for (i = 0; i < n; i++) for (j = 0; j <= m; j++) {
        x = i; y = j;
        if (dir == 'U' || dir == 'D') x = get(x, n, line, dir);
        else y = get(y, m, line, dir);
        if ((dir == 'U' && i >= line) || (dir == 'D' && i < line)) x--;

        if (Nver[x][y].sign) {
            if (ver[i][j].sign) Nver[x][y].sum += ver[i][j].sum;
            else Nver[x][y].sum += ver[i][j].down;
        } else {
            if (ver[i][j].sign) {
                if ((j < line && dir == 'L') || (j >= line && dir=='R'))
                    Nver[x][y].down += ver[i][j].sum;
                else Nver[x][y].up += ver[i][j].sum;
            } else {
                if ((i < line && dir == 'U') || (i >= line && dir == 'D') ||
                    (j < line && dir == 'L') || (j >= line && dir == 'R'))
                {
                    Nver[x][y].down = ver[i][j].down; Nver[x][y].dCover = 1;
                    if (!Nver[x][y].uCover) Nver[x][y].up += ver[i][j].up;
                } else {
                    Nver[x][y].up = ver[i][j].down; Nver[x][y].uCover = 1;
                    if (!Nver[x][y].dCover) Nver[x][y].down += ver[i][j].up;
                }
            }
        }
    }

    // replace old data with the data after folding the paper
    memcpy(hor, Nhor, sizeof(hor));
    memcpy(ver, Nver, sizeof(ver));
    n = nNew; m = mNew;
}


int main() {
    while (scanf("%d %d", &n, &K) && (n || K)) {
        n++; N = m = n;

        init();
        // simulate every time step of folding the paper
        for (; K; K--) work();	//模拟每一步折叠

        // accumulate number of all visible pieces and print the result
        ans = hor[0][0].sum - 1 + hor[1][0].sum - 1 + ver[0][0].sum - 1 +
            ver[0][1].sum - 1;
        printf("Case %d: %d pockets\n", ++ Cases, ans);
    }

    return 0;
}
