#include <stdio.h>
#include <math.h>

// required precision of radius is 1 * 10^(-6)
const double eps = 1e-6;

// the radius has a range ra <= r <= rb
double ra, rb, r;
int x[25], y[25], n, i, len, px, py, cases;

// the direction of an edge is representable by 1-byte char
char dr;


// check whether the heliport with center (ox, oy) and radius r
// satisfies all the requirements
bool check(double ox, double oy) {
    // a ray from (ox, oy) intersects the polygon s times
    int i, s; s = 0;

    // if the ray intersects the polygon even number of times,
    // then (ox, oy) is outside the polygon; not applicable
    for (i = 0; i < n; i++)
        if (x[i] > ox && ((y[i] > oy) ^ (y[i+1] > oy))) s++;
    if (s % 2 == 0) return false;

    // check each edge/corner if it's inside the heliport
    for (i = 0; i < n; i++) {
        // a corner is inside the circle
        if ((x[i]-ox) * (x[i]-ox) + (y[i]-oy) * (y[i]-oy)
            < (r-eps) * (r-eps)) return false;

        // a horizontal edge is inside the circle
        if (x[i] == x[i+1] && ((y[i] > oy) ^ (y[i+1] > oy))
            && fabs(x[i]-ox) < r-eps) return false;

        // a vertical edge is inside the circle
        if (y[i] == y[i+1] && ((x[i] > ox) ^ (x[i+1] > ox))
            && fabs(y[i]-oy) < r-eps) return false;
    }
    // otherwise the radius satisfies our requirements
    return true;
}


// check whether one can build heliport somewhere w/ radius r
// three possible cases, as indicated by three for loops
bool ok() {
    int i, j; double di, dd, mx, my, dx, dy;

    // heliport is beside two perpendicular edges
    for (i = 0; i < n; i++) {
        if (x[i] == x[i+1]) {
            for (j = 0; j < n; j++) {
                if (y[j] == y[j+1]) {
                    // two edges found, check the points with
                    // distances r from both edges
                    if (check(x[i]+r, y[j]+r)) return true;
                    if (check(x[i]+r, y[j]-r)) return true;
                    if (check(x[i]-r, y[j]+r)) return true;
                    if (check(x[i]-r, y[j]-r)) return true;
                }
            }
        }
    }

    // heliport is beside one edge and intersects one corner
    for (i = 0; i < n; i++) for (j = 0; j < n; j++) {
        if (x[i] == x[i+1]) {
            // center is on the right of a vertical edge
            di = fabs(x[j] - (x[i]+r));
            if (di < r) {
                dd = sqrt(r * r - di * di);
                if (check(x[i]+r, y[j]+dd)) return true;
                if (check(x[i]+r, y[j]-dd)) return true;
            }

            // center is on the left of a vertical edge
            di = fabs(x[j] - (x[i]-r));
            if (di < r) {
                dd = sqrt(r * r - di * di);
                if (check(x[i]-r, y[j]+dd)) return true;
                if (check(x[i]-r, y[j]-dd)) return true;
            }
        } else {
            // center is above a horizontal edge
            di = fabs(y[j] - (y[i]+r));
            if (di < r) {
                dd = sqrt(r * r - di * di);
                if (check(x[j]+dd, y[i]+r)) return true;
                if (check(x[j]-dd, y[i]+r)) return true;
            }

            // center is below a horizontal edge
            di = fabs(y[j] - (y[i]-r));
            if (di < r) {
                dd = sqrt(r * r - di * di);
                if (check(x[j]+dd, y[i]-r)) return true;
                if (check(x[j]-dd, y[i]-r)) return true;
            }
        }
    }

    // heliport is intersecting two corners
    for (i = 0; i < n-1; i++) for (j = i+1; j < n; j++) {
        mx = (x[i] + x[j]) / 2.0;
        my = (y[i] + y[j]) / 2.0;
        di = sqrt((x[i]-mx)*(x[i]-mx) + (y[i]-my)*(y[i]-my));
        if (di > 0 && di < r) {
            // some point with distance r from both corners
            dd = sqrt(r * r - di * di);
            dx = (my-y[i]) / di * dd;
            dy = (x[i]-mx) / di * dd;
            if (check(mx+dx, my+dy)) return true;
            if (check(mx-dx, my-dy)) return true;
        }
    }

    // no possible case; this radius r is not applicable
    return false;
}


int main() {
    while (scanf("%d", &n) && n) {
        if (cases) printf("\n");
        // current coordinates (px, py) while reading input
        px = 0; py = 0;
        for (i = 1; i <= n; i++) {
            scanf("%d %c", &len, &dr);

            if (dr == 'R') px += len;
            else if (dr == 'L') px -= len;
            else if (dr == 'U') py += len;
            else py -= len;

            x[i] = px; y[i] = py;
        }

        // radius is between 0 and 999 by constraints
        ra = 0; rb = 999;
        // binary search for the maximum applicable radius
        while (rb - ra > eps) {
            r = (ra + rb) / 2;
            if (ok()) ra = r;
            else rb = r;
        }
        printf("Case Number %d radius is: %.2lf\n",
            ++cases, r);
    }
    return 0;
}
