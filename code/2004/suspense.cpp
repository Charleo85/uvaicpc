#include <stdio.h>
#include <string.h>
#include <math.h>

// the information about which animal lives in a particular window
char ch;

// Jan and Tereza lives on the Tj, Tt 'th floor, respectively
int C, Tj, Tt, i, j, k;

// d is the distance between the two buildings, ans is the length of the rope
// that connects the two windows
double d, ans;

// a[] is the info about all animals in Jan's building, and b[] is the info
// about all animals in Tereza's building
int a[30], b[30];


// helper method to calculate the length of a parabola given a, c, and x
double calc_2(double a, double c, double x) {
    return x / 2 * sqrt(a*x*x + c) + c / (2*sqrt(a)) *
        log(x * sqrt(a) + sqrt(a*x*x + c));
}

// calculate the length of a parabola with formula Y = A (x - B)^2 + C, after
// some mathematic manipulations
double calc_1(double C) {
    // use a, b, c to represent the coefficients of the parabola in order to
    // simplify the calculations of solving an equation
    double A, B, a, b, c;
    if (Tt == Tj) B = d / 2;
    else {
        a = (Tt-Tj) * 3; b = 2 * d * (Tj*3-2-C); c = -d * d * (Tj*3-2-C);
        B = (-b + sqrt(b*b - 4*a*c)) / (2*a);
        if (B < 0 || B > d) B = (-b - sqrt(b*b - 4*a*c)) / (2*a);
    }
    A = (Tj*3 - 2 - C) / (B*B);
    return calc_2(4*A*A, 1, B) + calc_2(4*A*A, 1, d-B);
}


// determine the type of the animal: 1 means cat, 2 means bird, 0 means other
int in() {
    scanf("%c", &ch);
    // discard any whitespace characters by continuing scanf
    while (ch != 'C' && ch != 'B' && ch != 'N') scanf("%c", &ch);
    if (ch == 'C') return 1;
    else if (ch == 'B') return 2;
    else return 0;
}


int main() {
    C = 0;

    while (true) {
        scanf("%d %d %lf", &Tj, &Tt, &d);
        if (Tj == 0 && Tt == 0 && d < 1e-6) break;

        memset(a, 0, sizeof(a)); memset(b, 0, sizeof(b));
        // fill in the info about Jan's and Tereza's buildings
        for (i = 1; i <= Tj; i++) a[i] = in();
        for (i = 1; i <= Tt; i++) b[i] = in();

        // if not the first, then print an empty line to match the format
        if (C) printf("\n");

        // search over all the floors below where Jan and Tereza live
        ans = -1;
        for (i = 1; i < Tt && i < Tj; i++) {
            // at least 1 meter from the ground, so 1st floor is excluded
            // if there's not cat to jump onto the rope, and no birds to
            // attack the rope, then satisfies our requirements
            if (i > 1 && ((a[i] != 1 && b[i] != 1) ||
                (a[i-1] != 2 && b[i-1] != 2)))
            {
                ans = calc_1(i*3 - 2 - 0.5 + 1);
                printf("Case %d: %.3lf\n", ++C, ans);
                break;
            }

            // same as before, second case
            if ((a[i] != 1 && b[i] != 1) || (a[i] != 2 && b[i] != 2)) {
                ans = calc_1(i*3 - 2 + 1);
                printf("Case %d: %.3lf\n", ++C, ans);
                break;
            }

            // same as before, third case
            if ((a[i+1] != 1 && b[i+1] != 1) || (a[i] != 2 && b[i] != 2)) {
                ans = calc_1(i*3 - 2 + 0.5 + 1);
                printf("Case %d: %.3lf\n", ++C, ans);
                break;
            }
        }

        // if no such bridge can be built, then output impossible
        if (ans < 0) printf("Case %d: impossible\n", ++C);
    }

    return 0;
}
