#include <stdio.h>
#include <memory.h>
#include <string.h>

// a is the input character, count is the frequency of characters,
// current is the letters in the first entry, mark is the position
// marker, remain is the letters not covered in the first entry,
// ans is the best answer
char a[41], count[26], current[21], mark[41], remain[81], ans[41];

// pos[len][j][k] is the resulting character of starting from the
// first character, go over a length "len" string "k" times with a
// step of "j"
char pos[41][41][41];

int i, j, k, n;
int go, len, step, skips, maxl, remains, start, unique, cases, ok;


// pre-process "pos[][][]" using a dynamic programming approach so
// that some repeat calculations can be avoided later
void preprocess() {
    // iterate over a string of length "len", with step "step"
    for (len = 1; len < 41; len++) {
        for (step = 1; step < 41; step++) {
            // no position has been reached at the beginning
            memset(mark, 0, sizeof(mark));
            // initialize the first position in the string
            go = 0; mark[0] = 1; pos[len][step][0] = 0;

            // iterate over the string "len-1" times
            for (i = 1; i < len; i++) {
                // how many positions have been skipped
                skips = (step-1) % (len-i) + 1;

                // if the current position is not marked, then we
                // record this visit and decrement "skips"
                while (skips) {
                    go++;
                    if (go >= len) go = 0;
                    if (!mark[go]) skips--;
                }
                mark[go] = 1;

                // record the position after stepping "i" times
                pos[len][step][i] = go;
            }
        }
    }
}


int main() {
    preprocess();

    while (true) {
        scanf("%s", &a);
        n = strlen(a); if (n < 2) break;

        // the longest solution's length is initialized to 0
        maxl = 0;
        // whether solution of length "maxl" is unique
        unique = 0;
        memset(ans, 0, sizeof(ans));
        memset(count, 0, sizeof(count));

        // calculate the frequency of each character and store them
        for (i = 0; i < n; i++) count[a[i]-'A']++;

        // i is the starting position of the first pass
        // this character has to appear more than 1 time
        for (i = 0; i < n; i++) if (count[a[i]-'A'] > 1) {
            // j is the step length of the first pass
            for (j = 1; j < n; j++) {
                memset(current, 0, sizeof(current));
                memset(mark, 0, sizeof(mark));
                // the current position is i
                go = i;

                // len is the length of a possible solution
                for (len = 0; len < n/2; len++) {
                    // record character we've just passed through
                    current[len] = a[go]; mark[go] = 1;

                    // if we obtain a length greater than maxl, and
                    // the solution is a different one
                    if (len >= maxl && strcmp(current, ans)) {
                        remains = 0; ok = 0;
                        // record and count the remaining characters
                        for (k = 0; k < n; k++) if (!mark[k])
                            remain[remains++] = a[k];

                        // to make the problem simpler, copy all the
                        // remaining characters to end of string
                        for (k = 0; k < remains; k++)
                            remain[remains+k] = remain[k];

                        // start is the position of the second pass
                        for (start = 0; start < remains; start++)
                        if (remain[start] == current[0]) {
                            // step is step length of second pass
                            for (step = j+1; step <= n; step++) {
                                // check next characters one by one
                                for (k = 1; k <= len; k++) {
                                if (remain[pos[remains][step][k]
                                    + start] != current[k]) break;
                                }
                                // if all characters match, then we
                                // compare with recorded solution
                                if (k > len) {
                                    ok = 1;
                                    if (len > maxl) {
                                        // longer solution found,
                                        // update length/uniqueness
                                        maxl = len;
                                        unique = 1;
                                        memcpy(ans, current,
                                            sizeof(current));
                                    } else if (unique) {
                                        // same length, not unique
                                        maxl++;
                                        unique = 0;
                                    } else {
                                        // current length does not
                                        // have a solution
                                        unique = 1;
                                        memcpy(ans, current,
                                            sizeof(current));
                                    }
                                    break;
                                }
                            }
                            // if obtain some solution, then break
                            if (ok) break;
                        }
                    }
                    // step over the next j characters
                    skips = j;
                    while (skips) {
                        go++;
                        if (go >= n) go = 0;
                        if (!mark[go]) skips--;
                    }
                }
            }
        }
        if (unique) printf("Code %d: %s\n", ++cases, ans);
        else printf("Code %d: Codeword not unique\n", ++cases);
    }

    return 0;
}
