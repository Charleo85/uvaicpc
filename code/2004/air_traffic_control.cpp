#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <math.h>

using namespace std;

// required precision of radius is 1 * 10^(-5)
const double eps = 1e-5;

// struct of the coordinates of a point
struct coor {
    double x, y;
};
// operator overload: "-" means shift point a to the position of point b
coor operator -(coor a, coor b)	{
    a.x	-= b.x;
    a.y	-= b.y; return a;
}

// NP is the plane number, NC is the number of control centers, and ok is to
// check whether some conditions are met
double r; int C, NP, NC, m, i, j, k, ok;

// covers[i] records how many control centers plane i is controlled, ans[i]
// records how many planes are under control of center i, cov[i] records the
// best control radius of control center i, and Tc[i] records the coverage of
// planes under the control of center i
int covers[100], ans[10], cov[100], Tc[100];

// coordinates of all planes, and some extra global structs
coor A, B; coor P[100];


// compare the priorities of point a and point b, based on their coordinates;
// pay attention to the error possibly caused by precision
bool cmp(coor a, coor b) {
    return a.y > b.y+eps || (a.y > b.y-eps && a.x > b.x);
}


// returns the distance from point a to the origin
double len(coor a) {
    return sqrt(a.x*a.x + a.y*a.y);
}


// given 3 points A, B, and C, determine the center of the unique circle that
// intersects all 3 points
bool cross(coor A, coor B, coor C, coor &O) {
    double A1, B1, C1, A2, B2, C2, tmp;

    A1 = B.x - A.x; B1 = B.y - A.y;
    // the line perpendicular to AB and which passes through midpoint of AB
    C1 = (A.y*A.y - B.y*B.y + A.x*A.x - B.x*B.x) / 2.0;
    A2 = C.x - A.x; B2 = C.y - A.y;
    // the line perpendicular to ABCand which passes through midpoint of AC
    C2 = (A.y*A.y - C.y*C.y + A.x*A.x - C.x*C.x) / 2.0;

    tmp = B1*A2 - B2*A1;
    // if the two lines are parallel to each other, then no circle made
    if (fabs(tmp) < eps) return false;
    // otherwise, fill the coordinates of the center in O and return true
    O.y = (C2*A1 - C1*A2) / tmp; O.x = (C1*B2 - C2*B1) / tmp;
    return true;
}


// check whether plan "Tc" is better then the best-so-far solution "cov"
bool check() {
    int i;
    for (i = 0; i < NP; i++) {
        if (cov[i] && !Tc[i]) return false;
        if (!cov[i] && Tc[i]) return true;
    }
    return true;
}


// calculate the best control radius based on border coordinates of A B, and
// coordinates of plane C
void work(coor A, coor B, coor C) {
    coor O; double Tr; int i, in, on;

    // if the radius cannot be determined, then returns, otherwise calculate
    // the circle center denoted by "O"
    if (!cross(A, B, C, O)) return;

    Tr = len(A-O); in = 0; on = 0;
    // calculate number of planes inside control, denoted by "in", and number
    // of planes on the border, denoted by "on"
    for (i = 0; i < NP; i++) {
        if (len(O-P[i]) < Tr-eps) in++;
        else if (len(O-P[i]) < Tr+eps) on++;
    }
    // if obtained illegal values for "in" or "on", then returns
    if (in > m || in+on < m) return;

    // calculate the remaining number of planes on the border
    on = m-in;
    // iterate over all planes to determine if each plane is under control
    for (i = 0; i < NP; i++) {
        if (len(O-P[i]) < Tr-eps) Tc[i] = 1;
        else if (len(O-P[i]) < Tr+eps && on) { Tc[i] = 1; on--; }
        else Tc[i] = 0;
    }
    // compare this solution with the best obtained solution so far
    if (r < 0|| Tr < r-eps || (Tr < r+eps && check())) {
        r = Tr;
        for (i = 0; i < NP; i++) cov[i] = Tc[i];
    }
}


int main() {
    while (scanf("%d %d", &NP, &NC) && NP) {
        for (i = 0; i < NP; i++) scanf("%lf %lf", &P[i].x, &P[i].y);
        // sort the planes according to their coordinates
        sort(P, P+NP, cmp);
        memset(covers, 0, sizeof(covers)); ok=1;

        for (i = 0; i < NC; i++) {
            scanf("%d %lf %lf %lf %lf", &m, &A.x, &A.y, &B.x, &B.y);
            r = -1;
            // for each plane, work over all regions to get the best one
            for (j = 0; j < NP; j++) work(A, B, P[j]);
            // accumulate the count of how many controls are on this plane
            for (j = 0; j < NP; j++) covers[j] += cov[j];
            // for some control center, no region is legal (r<0), so not ok
            if (r < 0) ok = 0;
        }

        memset(ans, 0, sizeof(ans));
        // calculate the number of planes center i is now controlling
        for (i = 0; i < NP; i++) ans[covers[i]]++;

        // output the results of this test case
        printf("Trial %d: ", ++C);
        if (ok) for (i = 0; i <= NC; i++) printf("%d  ", ans[i]);
        else printf("Impossible");
        printf("\n\n");
    }

    return 0;
}
