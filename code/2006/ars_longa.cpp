#include <stdio.h>
#include <memory.h>
#include <math.h>

// x[], y[] and z[] are coordinates of the balls, a[][] are tension forces on
// each ball, b[][] are the systems of equations
double x[101], y[101], z[101], a[300][101], b[300][101];

// other global variables necessary for the program
double eps, c; int n, m, r, i, j, k, l, t;


// use Gaussian elimination to determine if a system of equation has solution
bool ok() {
    l = 0;
    // eliminate the m variables, one by one
    for (i = m; i; i--) {
        k = l;
        // for the i'th variable, find the equation w/ greatest coefficients
        for (j = l+1; j < r; j++) if (fabs(b[j][i]) > fabs(b[k][i])) k = j;
        // if all equations have zero coefficients on this variable, skip it
        if (fabs(b[k][i]) < eps) continue;

        // change this equation (as above) to the l'th row in the system
        memcpy(x, b[l], sizeof(x));
        memcpy(b[l], b[k], sizeof(x));
        memcpy(b[k], x, sizeof(x));

        // eliminate all the i'th variable in the remaining equations below
        for (j = l+1; j < r; j++) {
            c = b[j][i] / b[l][i];
            for (k = 0; k < i; k++) b[j][k] -= c * b[l][k];
            b[j][i] = 0;
        }
        l++;
    }
    for (i = l; i < r; i++) if (fabs(b[i][0]) > eps) return false;
    return true;
}


int main() {
    eps = 0.1;

    while (scanf("%d%d", &n, &m) && n) {
        memset(a, 0, sizeof(a));
        for (i = 0; i < n; i++)	scanf("%lf%lf%lf", &x[i], &y[i], &z[i]);

        for (i = 1; i <= m; i++) {
            // the indices should fit the array dimensions, so decrement by 1
            scanf("%d%d", &j, &k); j--; k--;
            // if ball j does not touch the ground (may fall down)
            if (z[j]) {
                // the tension forces on ball j in x, y, and z directions
                a[j*3][i] = x[k] - x[j];
                a[j*3+1][i] = y[k] - y[j];
                a[j*3+2][i] = z[k] - z[j];
            }
            // if ball k does not touch the ground (may fall down)
            if (z[k]) {
                // the tension forces on ball k in x, y, and z directions
                a[k*3][i] = x[j] - x[k];
                a[k*3+1][i] = y[j] - y[k];
                a[k*3+2][i] = z[j] - z[k];
            }
        }

        // record how many systems of equations are to be established
        r = 0;
        // for each ball, if it does not touch the ground (may fall), then we
        // establish a system of equations to calculate its equillibrium
        for (i = 0; i < n; i++)	if (z[i] > 0) {
            // add an equation to the system in x, y, and z directions
            memcpy(b[r++], a[i*3], sizeof(x));
            memcpy(b[r++], a[i*3+1], sizeof(y));
            memcpy(b[r++], a[i*3+2], sizeof(z));
            // the combined force in z direction should be same as gravity
            b[r-1][0] = 1;
        }

        printf("Sculpture %d: ", ++t);
        // if no solution, then the system is not static
        if (!ok()) {
            printf("NON-STATIC\n"); continue;
        }
        r = 0;
        // check each ball again, to get the result of whether it's stable
        for (i = 0; i < n; i++)	if (z[i] > 0) {
            memcpy(b[r++], a[i*3], sizeof(x));
            memcpy(b[r++], a[i*3+1], sizeof(y));
            memcpy(b[r++], a[i*3+2], sizeof(z));
            b[r-1][0] = sin(i*3+1);
            b[r-2][0] = sin(i*3+2);
            b[r-3][0] = sin(i*3+3);
        }
        if (!ok()) {
            printf("UNSTABLE\n"); continue;
        }
        printf("STABLE\n");
    }

    return 0;
}
