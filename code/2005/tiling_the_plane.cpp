#include <stdio.h>
#include <memory.h>

// c[] is the the direction sequence, sequence_a[] and sequence_b[] are 2 sub
// sequences to be matched
char c[55], ch[5555], sequence_a[2777], sequence_b[2777];

// num[] is the length array, combo_a[] and combo_b[] denote the number of the
// repeated elements in sequence a and b (see above), total[] is the frequency
// of one of the 4 directions
int num[55], combo_a[2777], combo_b[2777], total[4];

// other necessary global variables
int n, i, j, k, len, count, reverse, ok, cases;


// the process of tiling the plane given the polygon's shape
void tr(int i, int point) {
    int j, min, step;
    // if all parts the sequence is matched, then we can tile the plane
    if (point >= len) {
        ok = 1; return;
    }
    // if we have tried at least 3 parts, then we can abort the tiling
    if (i > 2) return;

    // try to match over the next dividing point (end of two sides)
    for (j = 0; j < len-point; j++) {
        step = 0;
        // check whether two portions of the sequence can match
        while (sequence_a[point+step] && sequence_a[point+step]
            == sequence_b[j+step])
        {
            // use the portion repeated less times as the next checkpoint
            if(combo_a[point+step] > combo_b[j+step]) min = combo_b[j+step];
            else min = combo_a[point+step];
            step += min + 1;
        }
        // if two sequences match in this checkpoint, then move on to next one
        if (j+step >= len-point) tr(i+1, point+step);
    }
}


int main() {
    while (scanf("%d", &n) && n) {
        // initialize all sequences to 0
        memset(ch, 0, sizeof(ch));
        memset(sequence_a, 0, sizeof(sequence_a));
        memset(sequence_b, 0, sizeof(sequence_b));
        // read input of each line segment's length and direction
        for (i = 0; i < n; i++)	scanf(" %c %d", &c[i], &num[i]);

        ok = 0;
        // iterate over each dividing point in the two sequences
        for (i = 0; i < n/2; i++) {
            len = 0;
            // try to expand the direction sequence
            for (j = 0; j < n; j++)	{
                count = (i + j) % n;
                // repeat a given number of times for the current direction
                for (k = 0; k < num[count]; k++) ch[len++] = c[count];
            }

            // calculate over part a of the sequence
            count = 0;
            for (j = 0; j < 4; j++) total[j] = 0;
            for (j = len/2 - 1; j >= 0; j--) {
                // the first len/2 characters belong to sequence a
                sequence_a[j] = ch[j];
                // accumulate the frequency of the 4 directions
                if (ch[j] == 'N') total[0]++;
                if (ch[j] == 'S') total[1]++;
                if (ch[j] == 'W') total[2]++;
                if (ch[j] == 'E') total[3]++;
                if (j < len/2 - 1 && ch[j] == ch[j+1]) count++;
                else count = 0;
                // store the time of repeated characters into array combo[]
                combo_a[j] = count;
            }

            // calculate over part b of the sequence
            count = 0;
            for (j = len/2; j < len; j++) {
                // reverse each direction and accumulate, in this case
                reverse = len - j - 1;
                if (ch[j] == 'N') {
                    sequence_b[reverse] = 'S'; total[1]--;
                }
                if (ch[j] == 'S') {
                    sequence_b[reverse] = 'N'; total[0]--;
                }
                if (ch[j] == 'W') {
                    sequence_b[reverse] = 'E'; total[3]--;
                }
                if (ch[j] == 'E') {
                    sequence_b[reverse] = 'W'; total[2]--;
                }
                if (j > len/2 && ch[j] == ch[j-1]) count++;
                else count = 0;
                combo_b[reverse] = count;
            }

            // determine if the directions are same; if not, stop counting
            for (j = 0; j < 4; j++)	if (total[j]) break;
            // if all directions have same frequency, set "length" to be the
            // length of sequence a, and now iterate over all breakpoints
            if (j > 3) {
                len /= 2; tr(0, 0);
                if (ok) break;
            }
        }

        printf("Polygon %d: ", ++cases);
        if (ok) printf("Possible\n"); else printf("Impossible\n");
    }

    return 0;
}
