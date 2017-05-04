#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unordered_map>
using namespace std;

// there are at most 147000 different days between 1700 and 2100
#define MAX_DATE 147000

// keep track of the case number (needed for output)
static int caseNum = 1;

// records which dates are valid dates for our output
bool dates[MAX_DATE];

// "itod" (Index to Date): given a specific index for "dates",
// returns the associated calendar date for this index
int itod[MAX_DATE];

// "dtoi" (Date to Index): given a date in calendar,
// returns the index it is represented in our "dates" array
// **This is exactly the reverse of "itod";
// it has to be stored as unordered_map,
// since 21001231 exceeds the max memory size for an array
unordered_map<int, int> dtoi;

// given an integer between 17000101 and 21001231,
// returns whether this integer is a valid date in calendar
bool isCorrectDate(int date) {
    int y = date / 10000, m = (date%10000) / 100, d = date % 100;
    if (m == 0 || m > 12) return false;
    if (d == 0 || d > 31) return false;
    if (d==31 && (m==4 || m==6 || m==9 || m==11)) return false;
    if (m == 2) {
        if (y%4 == 0 && (y%100 != 0 || y%400 == 0)) return d <= 29;
        else return d <= 28;
    }
    return true;
}

// pre-generate "itod" and "dtoi" for all possible dates
void genMaps() {
    int ind = 0;
    for (int i = 17000101; i <= 21001231; i++) {
        if (!isCorrectDate(i)) continue;
        itod[ind++] = i;
        dtoi[i] = ind-1;
    }
}

// given two dates "start" and "end" (possibly the same),
// prints the output in the correct format to stdout
void printOutput(int start, int end) {
    int y1 = start / 10000, m1 = (start%10000) / 100, d1 = start%100;
    int y2 = end / 10000, m2 = (end%10000) / 100, d2 = end%100;
    if (start == end) // required to print only one date
        printf("    %d/%d/%d\n", m1, d1, y1);
    else
        printf("    %d/%d/%d to %d/%d/%d\n", m1, d1, y1, m2, d2, y2);
}

// main part of the solution
void solve(int b, int a) {
    printf("Case %d:\n", caseNum);

    // reads all dates whose data have been previously retrieved
    // and store them in a vector (we need these data later)
    vector<int> dates1, dates2;
    for (int i = 0; i < b; i++) {
        int d1, d2; scanf("%d %d\n", &d1, &d2);
        dates1.push_back(d1);
        dates2.push_back(d2);
    }

    // reads all remaining dates whose data are to be retrieved
    for (int i = 0; i < a; i++) {
        int d1, d2; scanf("%d %d\n", &d1, &d2);
        // these dates are required for output; set them to true
        fill(dates+dtoi[d1], dates+dtoi[d2]+1, true);
    }

    for (int i = 0; i < b; i++) {
        // these dates have been previously retrieved (no longer needed)
        fill(dates+dtoi[dates1[i]], dates+dtoi[dates2[i]]+1, false);
    }

    // iterate over all possible dates to get desired time intervals
    int i = 0;
    bool need = false;
    while (i <= dtoi[21001231]) {
        if (dates[i]) {
            int start = itod[i];
            while (dates[++i]); // scan for the end of this interval
            int end = itod[i-1];
            printOutput(start, end);

            // the "no quotes" message should no longer be displayed
            need = true;
        } else
            i++;
    }

    // no dates are required, print the corresponding message
    if (!need) printf("    No additional quotes are required.\n");
}

int main() {
    genMaps();

    bool need = false;
    for (;;) {
        // the array "dates" need to be initialized as false everywhere
        fill(dates, dates+MAX_DATE, false);

        // read input
        int b, a; scanf("%d %d\n", &b, &a);

        // check if we reach the end of test file
        if (b == 0 && a == 0) break;

        // keep track of line feeds between cases
        if (!need) need = true;
        else printf("\n");

        solve(b, a);
        caseNum++;
    }

    return 0;
}
