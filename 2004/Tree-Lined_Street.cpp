#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <climits>
#define ll long long
using namespace std;

struct line{              // line struct include two points (x1, y1) (x2, y2)
  double x1, x2, y1, y2;
};

line lines[101];          // store all the line segments in an array
vector<double> section[101];

double dist(ll x1, ll y1, ll x2, ll y2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double length(line l) {
  return sqrt((l.x1 - l.x2) * (l.x1 - l.x2) + (l.y1 - l.y2) * (l.y1 - l.y2));
}

bool check(int x1, int y1, int x2, int y2, int x3, int y3) {
  ll t1, t2;
  t1 = (ll)x1 * (ll)y2 - (ll)x2 * (ll)y1;
  t2 = (ll)x1 * (ll)y3 - (ll)x3 * (ll)y1;
  return ((t1>=0 && t2<=0) || (t1<=0 && t2>=0));
}

bool cross(line A, line B) {
  return ((check(A.x2-A.x1, A.y2-A.y1, B.x1-A.x1,B.y1-A.y1,B.x2-A.x1,B.y2-A.y1))
    && (check(B.x2-B.x1, B.y2-B.y1, A.x1-B.x1,A.y1-B.y1,A.x2-B.x1,A.y2-B.y1)));
}

void intersect(line A, line B, int i, int j) {
  if (!cross(A, B)) return;
  double A1, B1, C1, A2, B2, C2, x, y;
  A1 = A.y2 - A.y1; B1= A.x1 - A.x2; C1 = -(A.x1 * A1 + A.y1*B1);
  A2 = B.y2 - B.y1; B2= B.x1 - B.x2; C2 = -(B.x1 * A2 + B.y1*B2);
  x = -(C1*B2-C2*B1)/(A1*B2-A2*B1);
  y = -(A1*C2-A2*C1)/(A1*B2-A2*B1);
//  cout << "line " << i << " " << j << ": " <<  x << " " << y << endl;
  section[i].push_back(dist(x, y, A.x1, A.y1));
  section[j].push_back(dist(x, y, B.x1, B.y1));
}

int main() {              // main algorithm
  int l;                  // number of lines in the map
  int map = 0;            // map index
  for(;;) {               // input
    cin >> l;
    if (l == 0) return 0;
    for (int i = 0; i < 101; i++) {
      section[i].clear();
    }

    for (int i = 0; i < l ; i++) {
      ll x1, x2, y1, y2;
      cin >> x1 >> y1 >> x2 >> y2;
      line l;
      l.x1 = x1;
      l.x2 = x2;
      l.y1 = y1;
      l.y2 = y2;
      lines[i] = l;
    }

    for (int i = 0; i < l; i++) {
      section[i].push_back(-25);
      section[i].push_back(length(lines[i]) + 25);
    }

    for (int i = 0 ; i < l; i++) {
      for (int j = i + 1; j < l; j++) {
        intersect(lines[i], lines[j], i ,j);
      }
    }

    for (int i = 0; i < l; i++) {
      sort(section[i].begin(), section[i].end());
    }


/*    for (int i = 0; i < l; i++) {
      int s = section[i].size();
      for (int j = 0; j < s; j++) {
        cout << section[i][j] << " ";
      }
      cout << endl;
    }
*/
    int ans = 0;
    for (int i = 0 ; i < l; i++) {
      int s = section[i].size();
      for (int j = 0; j < s - 1; j++) {
        cout << section[i][j] << " ";
        double len = section[i][j + 1] - section[i][j];
    //    cout << len << endl;
        int k = (int)(len/50);
//        while (len - k*50 > 1e-6) k++;
//        while (len - k*50 < 1e-6) k--;
        ans += k;
      }
      cout << section[i][s-1] << endl;
    }

    cout << "Map " << map + 1 << endl;
    cout << "Trees = " << ans << endl;
    map++;
  }

}
