#include <stdio.h>

// ones[] and tens[] are arrays used for computing the divisors of any given
// bipartite numbers, others are global variables
int ones[9999], tens[999], n, i, j, k, s, t, best_i, best_j, best_s, best_t;


// check if the current bipartite number is greater than n
bool ck() {
    int p, q, r;
    if (i > 5) return 1;
    p = s; r = t;
    for (q = 0; q < j; q++) p = p * 10 + s;
    for (q = 0; q < i-j; q++) p = p * 10;
    for (q = 1 ;q < i-j; q++) r = r * 10 + t;
    return p + r > n;
}


int main() {
    while(scanf("%d", &n) && n)	{
        printf("%d: ", n);
        // base case: the public key is just 1, output bipartite number 10
        if (n == 1) {
            printf("1 1 1 0\n"); continue;
        }
        // initialize the first term in the arrays a and b
        ones[0] = 1; tens[0] = 1;
        // generate the array a: a[i+1] = (a[i]*10 + 1) % n
        for (i = 1; i < 9999; i++) ones[i] = (ones[i-1] * 10 + 1) % n;
        // generate the array b: b[i+1] = b[i] % n
        for (i = 1; i < 999; i++) tens[i] = tens[i-1] * 10 % n;

        // iterate over the total length of the bipartite number
        for (i = 1, best_s = 0; i < 9999; i++) {
            k = 0;
            // limit the length of the first portion, to improve efficiency
            if ((n % 10 == 0 || n % 25 == 0) && i > 11) k = i - 11;
            // if t and s are not equal and n divides the bipartite number,
            // and the number is greater than n, then this solution is good
            // and so we update the solution
            for (j = k; j < i; j++) for (s = 1; s < 10; s++)
                for (t = 0; t < (n%10 ? 10 : 1); t++)
                    if (t != s && (((long long)ones[j]) * tens[i-j] * s +
                        ones[i-j-1] * t) % n == 0 && ck() && (!best_s ||
                        s < best_s || s == best_s && j>best_j && s<best_t))
            {
                best_i = i; best_j = j; best_s = s; best_t = t;
            }
            // if we have found the solution, then no need to check longer
            // bipartite numbers any more
            if (best_s) break;
        }

        printf("%d %d %d %d\n", best_j+1, best_s, best_i-best_j, best_t);
    }

    return 0;
}
