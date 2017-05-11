#include <stdio.h>
#include <memory.h>

// profit[][] is the profit of each combination, pies[] is the number of each
// pie, ices[] is number of each ice cream
int profit[55][55], pies[55], ices[55], pies_left[55], ices_left[55];

// these are arrays needed to complete the max flow algorithm
int profit_p[55], profit_i[55], combins[55][55], from_p[55], from_i[55];

// other necessary global variables
int n, m, i, j, k, new_profit, sum, cases, inf, updated;
double total_profit, x;


// compute the max flow min cost algorithm
void go() {
    // initialize the combinations, each has a flow of 0
    memset(combins, 0, sizeof(combins));
    // initialize the total profit
    total_profit = 0;

    // for each pie compute the augmenting path associated with it; each time
    // increment the total flow by 1
    for(i = 0; i < sum; i++) {
        // initialize all costs to infinity
        for(j = 0; j < m; j++) profit_i[j] = inf;
        for(j = 0; j < n; j++) {
            // initialize this pie node's source and costs
            from_p[j] = -1; profit_p[j] = inf;
            // if there are pies left, then this node's cost is set to zero
            if (pies_left[j]) profit_p[j] = 0;
        }

        // while there's an augmenting path, keep the solution updated
        updated = 1;
        while (updated) {
            updated = 0;
            // check each pie's node, if there's an augmenting path that pass
            // this point, then pie j and ice cream k can be combined
            for (j = 0; j < n; j++)	if (profit_p[j] > inf)
                for (k = 0; k < m; k++) if (profit[j][k] > 0)
            {
                // compute the new augmenting path's cost, and compare it w/
                // the original cost
                new_profit = profit_p[j] + profit[j][k];
                if( profit_i[k] == inf || new_profit < profit_i[k]) {
                    profit_i[k] = new_profit; from_i[k] = j;
                    updated = 1;
                }
            }
            // check each ice cream's node, similar to the pie case above
            for (j = 0; j < m; j++)	if (profit_i[j] > inf)
                for (k = 0; k < n; k++) if (combins[k][j])
            {
                new_profit = profit_i[j] - profit[k][j];
                if (profit_p[k] == inf || new_profit < profit_p[k]) {
                    profit_p[k] = new_profit; from_p[k] = j;
                    updated = 1;
                }
            }
        }

        // the index of ice cream corresponding to the minimum cost
        k = 0;
        // check each ice cream node's augmenting path costs; if smaller than
        // previous node's cost, then update the node index
        for (j = 1; j < m; j++)
            if (profit_i[k] == inf || !ices_left[k] ||
                profit_i[j] < profit_i[k] && ices_left[j]) k = j;

        // decrement the remaining ice creams, and update the total flow
        ices_left[k]--; total_profit += profit_i[k];

        // find out the augmenting path in this time step
        while (true) {
            // pie j is combined with ice cream k, increment the flow by one
            j = from_i[k]; combins[j][k]++;
            // if no pie j left, the break out of this loop
            if (from_p[j] < 0) break;
            // backtracking step, revert the flow back one step
            k = from_p[j]; combins[j][k]--;
        }

        // used pie j, the remaining value should be decremented by one
        pies_left[j]--;
    }
}


int main() {
    inf = -(1<<30);

    while (scanf("%d%d", &n, &m) && n) {
        sum = 0;
        // read input about pies
        for (i = 0; i < n; i++) {
            scanf("%d", &pies[i]);
            pies_left[i] = pies[i]; sum += pies_left[i];
        }
        // read input about ice creams
        for (i = 0; i < m; i++) {
            scanf("%d", &ices[i]);
            ices_left[i] = ices[i];
        }
        // read input about the possible profits
        for (i = 0; i < n; i++)	for (j = 0; j < m; j++) {
            scanf("%lf", &x);
            profit[i][j] = int(x * 100 + 0.1);
        }

        // the main algorithm, this time for the min profit
        go();

        printf("Problem %d: %.2lf to ", ++cases, total_profit/100);
        // recover the original pies and ice creams info
        memcpy(pies_left, pies, sizeof(pies));
        memcpy(ices_left, ices, sizeof(ices));
        for (i = 0; i < n; i++)	for (j = 0; j < m; j++)
            if (profit[i][j] > 0) profit[i][j] = 1001 - profit[i][j];

        // main algorithm again, this time for the max profit
        go();

        printf("%.2lf\n", (sum * 1001 - total_profit) / 100);
    }

    return 0;
}
