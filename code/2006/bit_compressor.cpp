#include <stdio.h>

// a[] is the compressed data, and other necessary global variables
char a[44]; int cases, inf, n, m, len, total, ones, solutions;


// check every possible decompression method
void go(int i) {
    int j, k, tmp_len, all_ones;
    // if the decompressed message is too long, or if the number of 1's is too
    // large, or there has already been more than one solutions
    if (total > n || ones > m || solutions > 1) return;
    // if the decompression is finished, check its validity and # solutions
    if (i >= len) {
        if (total + ones == n + m) solutions++; return;
    }

    // iterate over all bits that start with 1, try to decompress the message
    // at any possible length
    if (a[i] == '1') for (j = i, k = 0, all_ones = 1; j < len; j++) {
        // calculate how many bits of 1's current portion is representing
        if (k < inf) k = k * 2 + a[j] - '0';
        // mark about whether these few bits are all 1's
        all_ones &= a[j] - '0';

        if (a[j+1] != '1') {
            // if the length of these bits are greater than 2, then enter next
            // level of recursion and accumulate number of 1's
            if (k > 2) {
                total += k; ones += k;
                go(j+1);
                // the backtracking step: return to last state after go(j+1)
                total -= k; ones -= k;
            }
            // if these bits have a length <= 2, then must be part of original
            // message, enter next recursion accordingly
            if (all_ones && j-i < 2) {
                tmp_len = j - i + 1; total += tmp_len; ones += tmp_len;
                go(j+1);
                // the backtracking step: return to last state after go(j+1)
                total -= tmp_len; ones -= tmp_len;
            }
        }
    } else {
        // if the current bit is 0, then must be part of original message
        total++; go(i+1); total--;
    }
}


int main() {
    inf = 99999;

    for (cases = 1; scanf("%d%d\n", &n, &m) && n+m; cases++) {
        scanf("%s", &a);

        // increment the value of "len", denoting the length of the bits
        for (len = 0; a[len] > ' '; len++);
        total = 0; ones = 0; solutions = 0;
        // recursive backtracking algorithm to decompress the message
        go(0);

        printf("Case #%d: %s\n", cases, solutions ? solutions-1 ?
            "NOT UNIQUE" : "YES" : "NO");
    }

    return 0;
}
