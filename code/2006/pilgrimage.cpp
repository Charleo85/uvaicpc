#include <stdio.h>
#include <string.h>

// the maximum amount of people in the group
const int MaxP = 2000 * 50;

// the keyword of an operation
char s[20];

// record the list of operation, list[i][0] is the type of the i'th operation,
// and list[i][1] is the operation count
int list[50][2];

// the number of times the group satisfies the requirements of PAY
int a[MaxP+1];

// other necessary global variables for the program
int n, m, i, j, k, P, sum, delta, min;


int main() {
    while (scanf("%d", &n) && n) {
        // initialize the number of PAY operations
        sum = 0; P = 0;
        for (i = 0; i < n; i++)	{
            scanf("%s %d", s, &m);
            // ignore operations of type COLLECT
            if (s[0] == 'C') continue;
            // accumulate number of PAY operations, and set value of list[][]
            if (s[0] == 'P') P += m;
            else {
                if (P && sum) {
                    list[sum][0] = 2; list[sum][1] = P; sum ++;
                }
                // only PAY operations between IN and OUT are valid
                if (s[0] == 'I') list[sum][0] = 0;
                else list[sum][0] = 1; list[sum][1] = m;
                sum ++; P = 0;
            }
        }

        // a[x] = k means a group of x people can satisfy k PAY operations, in
        // this way, the final answer should be number of PAY for all people
        memset(a, 0, sizeof(a));
        delta = 0; min = 0; P = 0;
        for (i = 0; i < sum; i++) {
            // delta is the change of group size and min records the change of
            // group size when it reaches the lowest
            if (list[i][0] == 0) delta += list[i][1];
            if (list[i][0] == 1) delta -= list[i][1];
            if (min > delta) min = delta;
            if (list[i][0] != 2) continue;
            // set all factors Y in PAY operatuon X to initial group size Z
            P++;
            for (j = 1; j <= list[i][1]; j++)
                if (j > delta && list[i][1] % j == 0) a[j-delta]++;
        }

        // if no valid P opeeration, output the lower bound of group size, or
        // otherwise output the last group size satisfying all requirements
        if (P == 0) printf("SIZE >= %d\n", 1 - min);
        else {
            k = 0;
            for (i = 1-min; i <= MaxP; i++) if (a[i] == P) {
                if (k) printf(" ");
                k = i; printf("%d", k);
            }
            // if no valid group size at any time, then output "IMPOSSIBLE"
            if (!k) printf("IMPOSSIBLE");
            printf("\n");
        }
    }

    return 0;
}
