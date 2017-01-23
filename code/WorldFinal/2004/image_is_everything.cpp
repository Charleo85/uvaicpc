#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

/*    to fold
5|          front, left, back, right, top, bottom
2|1|4|3|      1     2     3     4      5     6
     |6|
*/

const int MaxN=11; // at most size of 10, but use 11 to avoid seeing the additional side
char f[6][MaxN][MaxN]; // color view read from data: [6]each view [][] square matrix
int p[6][MaxN][MaxN];  // number of cubic(s) removed: [6]each view [][] square matrix
int g[MaxN][MaxN][MaxN]; // 3D matrix: 0 as not exist, -1 no info yet, char A-Z (1-27) as color
int n; //cubic size

bool update(int m) {
   bool upd=false;
   for(int i=0; i<n; ++i) for(int j=0; j<n; ++j){ //analyze each pixel in current view
      int &t=p[m][i][j]; //the number of cubic removed at this pixel
      char ch=f[m][i][j]; //the color at this pixel in the original view
      if(t==n) continue; //already removed all cubics at this pixel
      int *val; //value of actual cubic at this pixel to analyze
      switch (m) {
          case 0: // front
            val = &(g[n-t-1][j][i]);
            break;
          case 1: // left
            val = &(g[j][t][i]);
            break;
          case 2: // back
            val = &(g[t][n-j-1][i]);
            break;
          case 3: // right
            val = &(g[n-j-1][n-t-1][i]);
            break;
          case 4: // top
            val = &(g[i][j][t]);
            break;
          case 5: // bottom
            val = &(g[n-i-1][j][n-t-1]);
            break;
          default:
            break;
      }
      if (*val==0){ ++t; upd=true;}
        //the cubic here does not exist
      else if(ch=='.'){ *val=0; ++t; upd=true; }
        //update the cubic here as not exist
      else if(*val==-1){ *val=ch-'A'+1; upd=true;}
        //if the cubic color is not analzed, use the color on the other side
      else if(*val!=ch-'A'+1){ *val=0; ++t; upd=true; }
        //if different from the cubic color from the other side, update the cubic here as not exist,
   }
   return upd;
}

int main() {
  while(true) {
    scanf("%d", &n); // read size of cubic
    if (n==0) break;
    for (int i=0; i<n; ++i) scanf(" %s %s %s %s %s %s", f[0][i], f[1][i], f[2][i], f[3][i], f[4][i], f[5][i]); // read view
    for (int i=0; i<6; ++i) fill(p[i][0], p[i][n], 0); //initize all view as non removed
    for (int i=0; i<n; ++i) fill(g[i][0], g[i][n], -1); //initize all cubics not exist
    // fill(g[0][0], g[n][0], -1);
    while (true) {
       bool quit=true;
       for (int i=0; i<6; i++) if(update(i)) quit=false;
       if(quit) break;
    }
    int ans=n*n*n; // init weight for perfect cubic
    for(int i=0; i<n; ++i) for(int j=0; j<n; ++j) for(int k=0; k<n; ++k)
      if(g[i][j][k]==0) --ans; // if the cubic at position[i][j][k] does not exit, remove this weight
    printf("Maximum weight: %d gram(s)\n", ans);
  }
  return 0;
}
