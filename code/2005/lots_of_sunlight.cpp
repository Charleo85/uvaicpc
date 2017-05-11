#include <stdio.h>
#include <math.h>

// h[] is each building's height, d[] is the distance from the next building
// to the left, and other global variables
int n, h[111], d[111], width, height, sum;
int i, di, num, level, id, cases, dh, dw, h1, m1, h2, m2;

// Half_pi stores the value pi/2, corresponding to 45 degrees angle
double min, max, angle, Half_pi;


int main() {
    // the precise value of pi/2 is given by arccos(0)
    Half_pi = acos(0);

    while (scanf("%d", &n) && n) {
        scanf("%d %d", &width, &height);
        // calculate the distance between current and the leftmost building
        sum = 0;
        for (i = 0; i < n; i++) {
            scanf("%d", &h[i]); d[i] = sum;
            if (i < n-1) {
                scanf("%d", &di); sum += di + width;
            }
        }

        printf("Apartment Complex: %d\n", ++cases);
        while (scanf("%d", &num) && num) {
            printf("Apartment %d: ", num);
            // calculate this apartment's height and its id
            level = num / 100 - 1; id = num % 100 - 1;
            // if the querying building does not exist, then exit directly
            if (id < 0 || id >= n || level < 0 || level >= h[id]) {
                printf("Does not exist\n"); continue;
            }
            // initialize the starting and the ending angles to be 0
            min = 0; max = 0;

            // check the left buildings one by one
            for (i = 0; i < id; i++) {
                // calculate the horizontal and vertical distances
                dw = d[id] - d[i] - width; dh = height * (h[i] - level);
                // if the height is no lower, then possible to have sunlight
                // calculate the angle and compare it with optimum
                if (dh > 0) {
                    angle = asin(dh / sqrt(dh*dh + dw*dw));
                    if (angle > min) min = angle;
                }
            }

            // check the right buildings one by one, similar to left case
            for (i = id+1; i < n; i++) {
                dw = d[i] - d[id] - width; dh = height * (h[i] - level);
                if (dh > 0)	{
                    angle = asin(dh / sqrt(dh*dh + dw*dw));
                    if (angle > max) max = angle;
                }
            }

            // convert the starting and ending angles to time, in seconds
            min = 20220 + 22800 * min / Half_pi;
            max = 65820 - 22800 * max / Half_pi;
            // calculate the times, according to the output format
            h1 = int(min / 3600); min -= h1 * 3600;
            m1 = int(min / 60); min -= m1 * 60;
            h2 = int(max / 3600); max -= h2 * 3600;
            m2 = int(max / 60); max -= m2 * 60;

            printf("%02d:%02d:%02d - %02d:%02d:%02d\n", h1, m1, int(min),
                h2, m2, int(max));
        }
    }

    return 0;
}
