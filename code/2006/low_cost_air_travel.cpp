#include <stdio.h>

// several constraints, defined by the problem statements
const int Max_City = 200;
const int Max_Ticket = 20;
const int Max_TicketLength = 10;
const int Max_TripLength = 10;

// the struct to represent info about a discounted ticket
struct info {
    int cost, n;
    int city[Max_TicketLength];
};

// global variables needed for this program
int cases, NT, NI, i, j, k, l, tmp, trip, change, sum, n;

// city[] is the array of cities, list[i] denotes the i'th city on the current
// min-cost trip route, route[] is all tickets used by current route, ticket[]
// is the array of tickets
int city[Max_City], list[Max_TripLength], route[Max_TripLength * Max_City];
info ticket[Max_Ticket];

// f[n][m][0] is the minimum cost of traveling the first n cities, and finally
// arriving at city m, f[n][m][1] is the ticket number, f[n][m][2] is number
// of cities on this ticket, f[n][m][3] is the starting city of this ticket
int f[Max_TripLength][Max_City][4];


// find the index of city x, if not possible, update the array of cities
int find(int x) {
    int i;
    for (i = 0; i < sum; i++) if (city[i] == x) return i;
    city[sum] = x; sum++;
    return (sum - 1);
}


int main() {
    cases = 0;

    while (true) {
        scanf("%d", &NT); if (NT == 0) break;
        sum = 0;
        // read input about each type of ticket and the cities it covers
        for (i = 0; i < NT; i++) {
            scanf("%d %d", &ticket[i].cost, &ticket[i].n);
            for (j = 0; j < ticket[i].n; j++) {
                scanf("%d", &tmp); ticket[i].city[j] = find(tmp);
            }
        }

        scanf("%d", &NI);
        for (trip = 0; trip < NI; trip++) {
            // read input about the i'th minimum-cost route
            scanf("%d", &n);
            for (i = 0; i < n; i++) {
                scanf("%d", &tmp); list[i] = find(tmp);
            }
            // several initializations on the 3-d array f
            for (i = 0; i < n; i++) for (j = 0; j < sum; j++) f[i][j][0] = -1;
            for (i = 0; i < 4; i++) f[0][list[0]][i] = 0;

            // iterate over the cities the route covers so far
            for (i = 0; i < n; i++) {
                while (true) {
                    change = 0;
                    // for the city the traveler is in, if we already have the
                    // minimum cost to arrive at city j, then we can iterate
                    // over all the tickets starting from city j
                    for (j = 0; j < sum; j++) if (f[i][j][0] >= 0)
                        for (k = 0; k < NT; k++) if (ticket[k].city[0] == j)
                    {
                        // starting from the i+1'th city, iterate over all the
                        // cities reachable by the ticket k
                        tmp = i + 1;
                        for (l = 1; l < ticket[k].n; l++) {
                            // if ticket k can extend the route to tmp cities,
                            // and we have not got a minimum cost for such a
                            // plan, then we update the optimal solution
                            if (tmp < n && list[tmp] == ticket[k].city[l])
                                tmp++;

                            if(f[tmp-1][ticket[k].city[l]][0] < 0 ||
                                f[tmp-1][ticket[k].city[l]][0] >
                                f[i][j][0]+ticket[k].cost)
                            {
                                f[tmp - 1][ticket[k].city[l]][0] =
                                    f[i][j][0] + ticket[k].cost;
                                f[tmp - 1][ticket[k].city[l]][1] = k;
                                f[tmp - 1][ticket[k].city[l]][2] = i;
                                f[tmp - 1][ticket[k].city[l]][3] = j;
                                // indicate that some changes have been made
                                change++;
                            }
                        }
                    }

                    // if no updates have been made so far, breakj while loop
                    if (change == 0) break;
                }
            }

            printf("Case %d, Trip %d: Cost = %d\n", ++cases, trip+1,
                f[n - 1][list[n - 1]][0]);

            // reverse search the array to find the sequence of all tickets
            // and print them out at the end
            l = 0; i = n - 1; j = list[n - 1];
            while (i || j != list[0]) {
                // record all info about the cities involved in this ticket
                route[l] = f[i][j][1]; k = f[i][j][2]; j = f[i][j][3];
                i = k; l++;
            }
            printf(" Tickets used:");
            for (i = l-1; i >= 0; i--) printf(" %d",route[i] + 1);
            printf("\n");
        }
    }

    return 0;
}
