// generate random testcase for debug
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
using namespace std;
void random(int n){
  for (int i = 0 ; i < n ; i++) {
    int lines = rand() % 100;
    cout << lines << endl;
    for (int j = 0 ; j < lines ; j++) {
      cout << rand() % 100000 << " " << rand() % 100000 << " "
        << rand() % 100000 << " " << rand() % 100000<< endl;
    }
  }
}
int main() {
  int n;
  cin >> n;
  random(n);
  cout << 0 << endl;
  return 0;
}
