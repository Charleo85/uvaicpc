#include <stdio.h>
#include <memory.h>
#include <math.h>

// go[] is the direction that cards go, correct[i][] denotes the correct order
// of cards after shuffie i times, card[] is the current shuffled order, ans[]
// is the final answer, err[i] is the incorrect position in i'th shuffle
int go[52], correct[11][52], card[52], next[52], err[11], ans[11];
int n, i, j, dfs, errors, best, cases, s;


// reverse the process of a shuffle, so we can find out incorrect cards
void anti_shuffle(int n) {
    int tmp[52], i;
    // store the anti-shuffled cards in a temporary array, then update next[]
    for (i = 0; i < 52; i++) tmp[correct[n][i]] = next[i];
    memcpy(next, tmp, sizeof(tmp));
}


// calculate the difference function between sequence a and sequence b
int differents(int *a, int *b) {
    int i, j, s; int step[52], go[52];
    memset(step, 0, sizeof(step));
    // the going direction of element a[i] is stored in go[a[i]]
    for (i = 0; i < 52; i++) go[a[i]] = b[i];

    s = 0;
    // if element i is not visited, then set it as the start of iterations
    for (i = 0; i < 52; i++) if (!step[i]) {
        j = i;
        // if not going back to the start, then continue iterating
        while (!step[j]) {
            step[j] = 1; j = go[j];	s++;
        }
        // the return value is one less than the loop length of the cards
        s--;
    }
    return s;
}


// recursively identify each incorrect position during every shuffle
void trytrytry(int i) {
    int j, save[52];
    // calculate the difference function between the correct card sequence and
    // the current obtained sequence
    dfs = differents(card, correct[i]);
    // if the value is larger than number of shuffles, then stop recursion
    if (dfs > i) return;
    // base case, update best answer, and record current incorrect position
    if (!i)	{
        if (errors < best)	{
            best = errors; memcpy(ans, err, sizeof(err));
        }
        return;
    }

    // copy the current card sequence in order for backup
    memcpy(save, card, sizeof(card));
    // initialize this time's error to be equal to -1, which means no errors
    err[i] = -1;
    for (j = 0; j < 51; j++) {
        // initialize a new sequence of cards
        memcpy(next, card, sizeof(card));
        // exchange the positions of two neighboring cards
        next[j] += next[j+1];
        next[j+1] = next[j] - next[j+1]; next[j] -= next[j+1];

        // calculate difference function, and anti-shuffle the sequence once
        dfs = differents(next, correct[i]);
        anti_shuffle(1);

        // accumulate numbef of errors, and record the error position j
        memcpy(card, next, sizeof(card)); errors++; err[i] = j;
        // when there's a potentially better answer, enter the next recursion
        if (errors + dfs < best) trytrytry(i-1);
        // the backtracking step, return the error number to the last state
        errors--; err[i] = -1; memcpy(card, save, sizeof(card));
    }

    // if this shuffle does not contain any errors, then anti-shuffle 1 time,
    // and directly enter the next recursion
    memcpy(next, card, sizeof(card));
    anti_shuffle(1);
    memcpy(card, next, sizeof(card));
    trytrytry(i-1);
    // the backtracking step, return the card sequence to the last state
    memcpy(card, save, sizeof(card));
}


int main() {
    // initialize the card sequences and the go[] directions
    for (i = 0; i < 52; i++) {
        go[i] = 26 * (1 - i%2) + i/2;
        correct[0][i] = i;
    }
    // correctly shuffle 10 times, and record the correct sequences
    for (i = 1; i < 11; i++) for (j = 0; j < 52; j++)
        correct[i][j] = correct[i-1][go[j]];

    scanf("%d", &s);
    for (cases = 1; cases <= s; cases++) {
        for (i = 0; i < 52; i++) scanf("%d", &card[i]);
        for (i = 0; i < 11; i++) {
            // compare this sequence with the correct sequence
            dfs = differents(card, correct[i]);
            // if the function value is not greater than time shuffled, then
            // the given sequence must have been shuffled i times
            if (dfs <= i) break;
        }
        printf("Case %d\nNumber of shuffles = %d\n", cases, i);
        if (!dfs) {
            printf("No error in any shuffle\n\n"); continue;
        }
        n = i;	best = dfs + 1; errors = 0;
        // check any incorrect position for all i times of shuffle
        trytrytry(i);
        for (i = 1; i <= n; i++) if (ans[i] > -1)
            printf("Error in shuffle %d at location %d\n", i, ans[i]);
        printf("\n");
    }

    return 0;
}
