#include <stdio.h>
#include <math.h>

// (ox[], oy[]) are coordinates of the signal's source, r[] is the transmit
// distance of the signal, (dx, dy) are differences of the circle center's
// x and y coordinates, (px, py) are the cartesian coordinates of the signal
double t, x, y, ox[12], oy[12], r[12], px, py, dx, dy;

// other global variables necessary for this program
double dr, degree, ti, pi, dis, lx, ly, xa, ya, xb, yb;
int n, i, cases, c1, c2;


// check if point (x, y) is close enough to all the circles
int check(double x, double y) {
    int i; double dx, dy;
    // check the distance of the point to every circle
    for(i = 0; i < n; i++) {
        // compute the distance "dr" between (x, y) and circle "i"
        dx = x - ox[i];	dy = y - oy[i];
        dr = sqrt(dx*dx + dy*dy) - r[i];
        if (fabs(dr) > 0.1) return 0;
    }
    return 1;
}


int main() {
    // first of all, the accurate value of pi is given by arccos(-1)
    pi = acos(-1.0);

    while (scanf("%d%lf%lf%lf", &n, &t, &x, &y) && n) {
        // read input about the n signals
        for (i = 0; i < n; i++) {
            scanf("%lf%lf%lf%lf", &px, &py, &degree, &ti);
            // these are relevant info about signal i
            degree = (90-degree) / 180 * pi;
            dis = 100 * ti;
            ox[i] = px + dis * cos(degree);
            oy[i] = py + dis * sin(degree);
            r[i] = 350 * (t-ti);
        }

        printf("Trial %d: ", ++cases);
        // based on first circle, check if the later circles overlap with it
        for (i = 1; i < n; i++) {
            // calculate the differences of the center's coordinates/radius
            dx = ox[i] - ox[0];
            dy = oy[i] - oy[0];
            dr = r[i] - r[0];
            // if the sum of these three squares are positive, then, the two
            // circles do not overlap, so we can select the current one
            if (dx*dx + dy*dy + dr*dr > 0.01) break;
        }

        // if all the circles overlap, then we have multiple solutions
        if (i >= n)	{
            printf("Inconclusive\n"); continue;
        }
        // calculate the distance between two circles' center
        dis = sqrt(dx*dx + dy*dy);
        // two circles have too much difference in radius, no solution
        if (dis < 0.1) {
            printf("Inconsistent\n"); continue;
        }
        // if "lx" too large, then two circles do not intersect, no solution
        lx = (dis*dis + r[0]*r[0] - r[i]*r[i]) / dis / 2;
        if (fabs(lx) > r[0] + 0.1) {
            printf("Inconsistent\n"); continue;
        }

        // if lx is greater than the radius, then change it to the radius
        // lx could also be negative, we should change it as well
        if (lx > r[0]) lx = r[0];
        if (lx < -r[0]) lx = -r[0];
        // calculate the ly value according to the given figure
        ly = sqrt(r[0]*r[0] - lx*lx);
        // normalize the distance differences dx and dy
        dx /= dis; dy /= dis;

        // calculate the (x, y) coordinates of the two intersecting points
        xa = ox[0] + dx*lx - dy*ly;
        ya = oy[0] + dy*lx + dx*ly;
        xb = ox[0] + dx*lx + dy*ly;
        yb = oy[0] + dy*lx - dx*ly;
        // if the two points are too close to each other, delete one of them
        if (sqrt((xa-xb) * (xa-xb) + (ya-yb) * (ya-yb)) < 0.1) {
            xb = 1e9; yb = 1e9;
        }

        // check if the two points satisfy the problem's requirements
        c1 = check(xa,ya); c2 = check(xb,yb);
        if (c1 + c2 == 1) {
            // if exactly one of the two points satisfies requirements, cast
            // c2 to c1 so that we can deal with them consistently
            if (c2) {
                xa = xb; ya = yb;
            }
            // calculate the distance between this point and the destination
            dx = x - xa; dy = y - ya; dis = sqrt(dx*dx + dy*dy);

            if (dis < 0.1) {
                printf("Arrived\n");
            } else {
                // calculate direction required to transmit to destination
                if (dy > 0) degree = acos(dx / dis);
                else degree = pi*2 - acos(dx / dis);
                // transform the degree to the correct units and format
                degree = 90 - degree / pi * 180;
                if (degree < 0) degree += 360;
                if (degree > 360) degree -= 360;
                printf("%.0lf degrees\n", degree);
            }
        } else if (c1) {
            // if both of the two points satisfy the requirements
            printf("Inconclusive\n");
        } else {
            // if none of the points satisfy the requirements, no solution
            printf("Inconsistent\n");
        }
    }

    return 0;
}
