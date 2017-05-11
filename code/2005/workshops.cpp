#include <stdio.h>
#include <algorithm>

using namespace std;

// the struct to represent a workshop, and its room assignment
struct info {
    int p,t;
};

// the comparator of struct info, used for sorting an array
bool cmp(info x, info y) {
    return (x.t < y.t);
}

// some constraints defined by the problem statement
const int MaxW = 1000; const int MaxR = 1000;

// sum_w, sum_p denotes the workshops and people yet to be assigned
int w, r, cases, sum_w, sum_p, i, j, l;

// workshop[] is the array of workshops, room[] is the array of rooms
info workshop[1100], room[1100];


// read input and initialize all global variables and arrays
bool init() {
    int i, hh, mm;
    scanf("%d", &w); if (w == 0) return false;

    sum_w = w; sum_p = 0;
    for (i = 0; i < w; i++) {
        scanf("%d %d", &workshop[i].p, &workshop[i].t);
        sum_p += workshop[i].p;
    }

    scanf("%d", &r);
    for (i=0;i<r;i++) {
        scanf("%d %d:%d", &room[i].p, &hh, &mm);
        // calculate the time that room i can be used for workshop
        room[i].t = (hh - 14) * 60 + mm;
    }

    // sort both workshop[] and room[] according to time, in increasing order
    sort(workshop, workshop + w, cmp);
    sort(room, room + r, cmp);

    return true;
}


int main() {
    cases = 0;

    while (init()) {
        // use a greedy strategy, assign rooms according to their time order
        for (i = 0; i < r; i++) {
            l = -1;
            for (j = 0; j < w; j++) {
                // if there's some workshop that cannot be assigned to room i
                // at any time, then break out, since later workshops are even
                // longer, and still cannot fit into this room
                if (workshop[j].t > room[i].t) break;

                // otherwise, if workshop j satisfies all requirements, then
                // assign it to room i
                else if (workshop[j].p <= room[i].p && workshop[j].p >= 0) {
                    if (l<0 || workshop[l].p<workshop[j].p) l = j;
                }
            }

            // if the room i is assigned some workshop, then calculate number
            // of people in the workshop, and set the flag
            if (l >= 0) {
                sum_w --; sum_p -= workshop[l].p; workshop[l].p = -1;
            }
        }

        printf("Trial %d: %d %d\n\n", ++cases, sum_w, sum_p);
    }

    return 0;
}
