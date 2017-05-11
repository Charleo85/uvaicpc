#include <stdio.h>

// MaxN is the maximum number of towers, MaxM is the maximum number of zones
const int MaxN = 20;
const int MaxM = 10;

// n is how many towers are planned, r is how many towers are actually built,
// m is number of zones, tot is current coverage, T is the best plan
int cases, n, m, r, tot, ans, T, i, j;

// sum[] is how many people are served for each tower, common[i][1] is the
// tower selected in zone i, common[i][2] is the number of users covered
int sum[MaxN], common[MaxM][2], S[1 << MaxN];


// read input and initialize all global variables and arrays
bool init() {
    int i, j, k, tmp;
    scanf("%d %d", &n, &r); if (n == 0 && r == 0) return false;
    for (i = 0; i < n; i++) scanf("%d", &sum[i]);

    scanf("%d", &m);
    for (i = 0; i < m; i++) {
        scanf("%d", &k);
        // for each tower in the zone, construct a corresponding bitmask
        common[i][0] = 0;
        for (j = 0; j < k; j++) {
            scanf("%d", &tmp); common[i][0] |= 1 << (tmp - 1);
        }
        scanf("%d", &common[i][1]);
    }

    return true;
}


int main() {
    cases = 0;
    // calculate the number of 1's in a particular bitmask
    for (i = 0; i < (1<<20); i++) for (j = 0; j < 20; j++)
        if (i & (1<<j)) S[i]++;

    while (init()) {
        ans = -1;
        // the number of 1's in the bitmask is the number of towers to build
        for (i = 0; i < (1<<n); i++) if (S[i] == r) {
            tot = 0;
            // calculate the number of users covered by these towers
            for (j = 0; j < n; j++) if (i & (1<<j)) tot += sum[j];
            // calculate the total coverage by number of towers in each zone
            for (j = 0; j < m; j++)	if (common[j][0] & i)
                tot -= common[j][1] * (S[common[j][0] & i] - 1);

            // if there's a greater coverage, update the optimal solution
            if (tot > ans) {
                ans = tot; T = i;
            } else if (tot == ans) {
                // if the total coverage is the same as the optimal solution
                // then if tower j in status i was not in the optimum, then
                // update the optimal solution, otherwise keep
                for (j = 0; j < n; j++) {
                    if (((1<<j) & i) && !((1<<j) & T)) {
                        T = i; break;
                    }
                    if (((1<<j) & T) && !((1<<j) & i)) break;
                }
            }
        }

        printf("Case Number %d\n", ++cases);
        printf("Number of Customers: %d\n", ans);
        printf("Locations recommended:");
        for (i = 0; i < n; i++) if ((1<<i) & T) printf(" %d", i+1); 
        printf("\n\n");
    }

    return 0;
}
