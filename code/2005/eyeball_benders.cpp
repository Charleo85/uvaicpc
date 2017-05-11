#include <stdio.h>
#include <math.h>

// the struct to represent a line segment by two points
struct line {
    double x1, y1, x2, y2;
};
// the struct to represent a point
struct point {
    double x, y;
};

// based on the required precision, we can calculate with an eps of 1*10^(-6)
const double eps = 1e-6;

// scale is the ratio between the large and the small picture
bool ok; int T, n, m, i, j, k; double scale;

// pL, pS are endpoints on the large and the small picture, respectively
point pL, pS;

// record the line segments on both the large and the small picture
line tL; line L[60], S[60], SN[60];


// read input and return a line struct
line in() {
    double tmp; line t;
    scanf("%lf %lf %lf %lf", &t.x1, &t.y1, &t.x2, &t.y2);
    if (t.x1 > t.x2 + eps) {
        // horizontal line, x coordinates are different
        tmp = t.x1; t.x1 = t.x2; t.x2 = tmp;
    }
    if (t.y1 > t.y2 + eps) {
        // vertical line, y coordinates are different
        tmp = t.y1; t.y1 = t.y2; t.y2 = tmp;
    }
    return t;
}


// compare if doubles x and y are equal or not, floating precision handled
bool same(double x, double y) {
    return (y - eps < x && x < y + eps);
}


// calculate the distance between point (x, y) and the origin
double dis(double x, double y) {
    return sqrt(x * x + y * y);
}


// determines if a line p in the small picture corresponds to some lines in
// the large picture
bool findS(line p) {
    // iterate over all line segments in the large picture
    int i;
    for (i = 0; i < n; i++) {
        // if line p in the small picture corresponds to line i in the large
        // picture, then found, return true
        if (same(p.x1, p.x2) && same(L[i].x1, L[i].x2) &&
            same(p.x1, L[i].x1)) if (L[i].y1 <= p.y1 && p.y2 <= L[i].y2)
                return true;
        if (same(p.y1, p.y2) && same(L[i].y1, L[i].y2) &&
            same(p.y1, L[i].y1)) if (L[i].x1 <= p.x1 && p.x2 <= L[i].x2)
                return true;
    }
    // no matches found, line p does not correspond to any line segments
    return false;
}


// determines if a line p in the large picture corresponds to some lines in
// the small picture, similar to findS
bool findL(line p) {
    int i;
    // iterate over all line segments in the small picture
    for (i = 0; i < m; i++) {
        // if line p in the large picture corresponds to line i in the small
        // picture, then found, return true
        if (same(p.x1, p.x2) && same(SN[i].x1, SN[i].x2) &&
            same(p.x1, SN[i].x1)) if (SN[i].y1 <= p.y1 && p.y2 <= SN[i].y2)
                return true;
        if (same(p.y1, p.y2) && same(SN[i].y1, SN[i].y2) &&
            same(p.y1, SN[i].y1)) if (SN[i].x1 <= p.x1 && p.x2 <= SN[i].x2)
                return true;
    }
    // no matches found, line p does not correspond to any line segments
    return false;
}


// fix some scale and corresponding features, determine if the small and the
// large picture match with each other
bool check() {
    double MaxX, MaxY, MinX, MinY; int i, j, k;
    // determine the 4 bounding coordinates of the sliding window
    MaxX = MinX = pL.x;	MaxY = MinY = pL.y;
    // for each line segment in the small picture, calculate its coordinates
    // after scaling and shifting, handle sliding window accordingly
    for (i = 0; i < m; i++) {
        SN[i].x1 = (S[i].x1 - pS.x) * scale + pL.x;
        SN[i].y1 = (S[i].y1 - pS.y) * scale + pL.y;
        SN[i].x2 = (S[i].x2 - pS.x) * scale + pL.x;
        SN[i].y2 = (S[i].y2 - pS.y) * scale + pL.y;

        // if the line SN[i] has appeared in the large picture before, then
        // terminate this method directly
        if (!findS(SN[i])) return false;

        if (SN[i].x1 < MinX) MinX = SN[i].x1;
        if (SN[i].x2 > MaxX) MaxX = SN[i].x2;
        if (SN[i].y1 < MinY) MinY = SN[i].y1;
        if (SN[i].y2 > MaxY) MaxY = SN[i].y2;
    }

    // for each line segment in the large picture, determine, based on if the
    // line is horizontal or vertical, if there's any corresponding line in
    // the small picture inside the sliding window
    for (i = 0; i < n; i++) {
        if (same(L[i].x1, L[i].x2)) {
            // if the line i in the large picture is vertical
            if (L[i].x1 > MaxX) {
                // if line i is strictly one the left or right of the sliding
                // window and the distance is less than eps, return false
                if (L[i].y1 > MaxY)
                    if (dis(L[i].x1-MaxX, L[i].y1-MaxY) < 0.005) return false;
                else if (L[i].y2 < MinY)
                    if (dis(L[i].x1-MaxX, L[i].y2-MinY) < 0.005) return false;
                else
                    if (L[i].x1 - MaxX < 0.005) return false;
            } else if (L[i].x2 < MinX) {
                // similar case as before
                if (L[i].y1 > MaxY)
                    if (dis(L[i].x2-MinX, L[i].y1-MaxY) < 0.005) return false;
                else if (L[i].y2 < MinY)
                    if (dis(L[i].x2-MinX, L[i].y2-MinY) < 0.005) return false;
                else
                    if (MinX - L[i].x2 < 0.005) return false;
            } else {
                if (L[i].y2 < MinY-0.005 || L[i].y1 > MaxY+0.005) continue;
                if (L[i].y2 < MinY || L[i].y1 > MaxY) return false;
                // select the portion of line i inside the sliding window, in
                // the large picture, and see if it matches with small picture
                tL = L[i];
                if (tL.y1 < MinY) tL.y1 = MinY;
                if (tL.y2 > MaxY) tL.y2 = MaxY;
                if (!findL(tL)) return false;
            }
        } else {
            // if the line i in the large picture is horizontal
            if (L[i].y1 > MaxY) {
                // similar as in the first portion of the outer if statement
                if (L[i].x1>MaxX)
                    if (dis(L[i].x1-MaxX, L[i].y1-MaxY) < 0.005) return false;
                else if (L[i].x2<MinX)
                    if (dis(L[i].x2-MinX, L[i].y1-MaxY) < 0.005) return false;
                else
                    if (L[i].y1 - MaxY < 0.005) return false;
            } else if (L[i].y2 < MinY) {
                // similar as in the first portion of the outer if statement
                if (L[i].x1 > MaxX)
                    if (dis(L[i].x1-MaxX, L[i].y2-MinY) < 0.005) return false;
                else if (L[i].x2 < MinX)
                    if (dis(L[i].x2-MinX, L[i].y2-MinY) < 0.005) return false;
                else
                    if (MinY - L[i].y2 < 0.005) return false;
            } else {
                // similar as in the first portion of the outer if statement
                if (L[i].x2 < MinX-0.005 || L[i].x1 > MaxX+0.005) continue;
                if (L[i].x2 < MinX || L[i].x1 > MaxX) return false;
                tL = L[i];
                if (tL.x1 < MinX) tL.x1 = MinX;
                if (tL.x2 > MaxX) tL.x2 = MaxX;
                if (!findL(tL)) return false;
            }
        }
    }

    // if reached this point, then all lines in the window match correctly
    return true;
}


// determine if the pair of small/large pictures is a valid puzzle
void work() {
    int k;
    // if there's only one line in the small picture
    if (m == 1) {
        // calculate the scale and determine validness in this scenario
        scale = 0.005 / (S[0].x2 - S[0].x1 + S[0].y2 - S[0].y1);
        ok = scale<=1 && check();
    } else for (k = 0; k < n; k++) {
        // iterate over each line in the large picture that may correspond to
        // line 1 in the small picture
        if ((L[k].x1 - L[k].x2) * (S[1].x1 - S[1].x2) ||
            (L[k].y1 - L[k].y2) * (S[1].y1 - S[1].y2))
        {
            if (S[1].x1 != S[1].x2) {
                // line 1 is horizontal
                if (same(S[1].y1, pS.y)) {
                    // if the fixed endpoint has same y coordinates as line 1
                    // we calculate the distance in x direction
                    if (!same(S[1].x1, pS.x))
                        scale = (L[k].x1 - pL.x) / (S[1].x1 - pS.x);
                    else
                        scale = (pL.x - L[k].x2) / (pS.x - S[1].x2);
                } else {
                    // if the fixed endpoint does not have same y coordinates
                    // then we can directly calculate scale in y direction
                    scale = (L[k].y1 - pL.y) / (S[1].y1 - pS.y);
                }
            } else {
                // line 1 is vertical, similar code as before
                if (same(S[1].x1, pS.x)) {
                    if (!same(S[1].y1, pS.y))
                        scale = (L[k].y1 - pL.y) / (S[1].y1 - pS.y);
                    else
                        scale = (pL.y - L[k].y2) / (pS.y - S[1].y2);
                } else {
                    scale = (L[k].x1 - pL.x) / (S[1].x1 - pS.x);
                }
            }
            // if the scale is less than 1 and the match is successful, then
            // the work is finished, note that we have to keep those scales
            // positive, or otherwise the image might be flipped
            ok = scale <= 1 && scale > eps && check();
            if (ok) return;
        }
    }
}


int main() {
    T = 0;

    while (scanf("%d %d", &m, &n) && (m || n)) {
        for (i = 0; i < m; i++) S[i] = in();
        for (i = 0; i < n; i++) L[i] = in();

        ok = false;
        for (i = 0; i < n; i++) for (j = 0; j < m; j++) if (!ok) {
            // mark line j in small picture as line 0, for convenience
            tL = S[0]; S[0] = S[j]; S[j] = tL;
            // assume the matching endpoint is the left/bottom corner
            pL.x = L[i].x1; pL.y = L[i].y1;
            pS.x = S[0].x1; pS.y = S[0].y1;
            work(); if (ok) break;
            // assume the matching endpoint is the top/right corner
            pL.x = L[i].x2; pL.y = L[i].y2;
            pS.x = S[0].x2; pS.y = S[0].y2;
            work(); if (ok) break;
        }

        if (ok) printf("Case %d: valid puzzle\n\n", ++T);
        else printf("Case %d: impossible\n\n", ++T);
    }

    return 0;
}
