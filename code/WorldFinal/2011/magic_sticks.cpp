#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;

const int MaxN = 509;
const double Pi = 3.1415926535897932384626;
// const double Tau =
int N;
double A[MaxN], Sum[MaxN][MaxN], Max[MaxN][MaxN], S[MaxN][MaxN], F[MaxN][MaxN];

double fmax(double a, double b){
  return a>b ? a:b;
}

double MaxAreaOf(int s, int t){
  double L = Max[s][t]/2.0;
  double x = (Max[s][t] - 1.0)/2.0;
  double R = fmax( Max[s][t]/(2.0*sin(Pi/(t-s+1))), (4.0*x >= 1.0 ? ((x*x)/sqrt(4.0*x-1.0)) : 0.0));
  double SL, SR;

  while (true) {
    SL = 0.0;
    SR = 0.0;
    for (int k=s; k<=t; k++) SL += sqrt(L*L-(A[k]/2.0)*(A[k]/2.0)*(A[k]/2.0));
    for (int k=s; k<=t; k++) SR += sqrt(R*R-(A[k]/2.0)*(A[k]/2.0)*(A[k]/2.0));
    if (SL+1e-5 > SR) break;
    double Mid = (L+R)/2.0;
    double deg = 0.0;
    for (int k=s; k<=t; k++) deg += 2*asin(A[k]/2.0/Mid);
    if (deg - 2*asin(Max[s][t]/2.0/Mid) <= Pi){
      deg -= 2*asin(Max[s][t]/2.0/Mid);
      deg += (2*Pi - 2*asin(Max[s][t]/2.0/Mid));
      if (deg < 2*Pi) L = Mid;
      else R = Mid;
    }else {
      if (deg < 2*Pi) R = Mid;
      else L = Mid;
    }
  }

  L = (L+R)/2.0;
  double deg = 0.0;
  for (int k = s; k <= t; k++) deg += 2*asin(A[k]/2.0/L);
  L = (L+R)/2.0;
  double Ret = 0.0;
  for (int k = s; k <= t; k++) Ret += sqrt(L*L - (A[k]/2.0)*(A[k]/2.0)*(A[k]/2.0));
  Ret -= sqrt(L*L - (Max[s][t]/2.0)*(Max[s][t]/2.0)*(Max[s][t]/2.0));
  if (deg - 2*asin(Max[s][t]/2.0/L) <= Pi) Ret -= sqrt(L*L- (Max[s][t]/2.0)*(Max[s][t]/2.0)*(Max[s][t]/2.0));
  else Ret += sqrt(L*L- (Max[s][t]/2.0)*(Max[s][t]/2.0)*(Max[s][t]/2.0));

  return Ret;
}

void solve(){
  for (int len = 1; len <= N; len++) for (int i = 1; i+len-1 <= N; i++){
    int j = i + len - 1;
    F[i][j] = S[i][j];
    for (int k = i; k < j ; k++) F[i][j] = fmax(F[i][j], F[i][k]+F[k+1][j]);
  }
  cout.precision(4);
  cout<< fixed << F[1][N] << endl;
}

int main(){
  int n = 0;
  while (cin >> N && N){
    for (int i=1; i <= N; i++) cin >> A[i];
    for (int i=1; i <= N; i++) Sum[i][i] = A[i];
    for (int i=1; i <= N; i++) Max[i][i] = A[i];
    for (int i=1; i <= N; i++) {
      for (int j = i+1; j <= N; j++){
        Sum[i][j] = Sum[i][j-1] + A[j];
        Max[i][j] = fmax(Max[i][j-1], A[j]);
      }
    }
    for (int i=1; i <= N; i++){
       for (int j = i; j <= N; j++){
        if (i-1 >= 1){
          if (A[i]*2 > A[i-1] && A[i-1]*2 > A[i]){
            S[i][j] = 0.0;
            continue;
          }
        }
        if (j+1 <= N){
          if (A[j]*2 > A[j+1] && A[j+1]*2 > A[j]){
            S[i][j] = 0.0;
            continue;
          }
        }
        if (i-3 >= 1){
          if (A[i-1] + A[i-2] > A[i-3] && A[i-2]+A[i-3] > A[i-1] && A[i-1]+A[i-3] > A[i-2]){
            S[i][j] = 0.0;
            continue;
          }
        }
        if (j+3 <= N){
          if ( A[j+1] + A[j+2] > A[j+3] && A[j+2]+A[j+3] > A[j+1] && A[j+1]+A[j+3] > A[j+2]){
            S[i][j] = 0.0;
            continue;
          }
        }
        if (Max[i][j] * 2 >= Sum[i][j]) S[i][j] = 0.0;
        else S[i][j] = MaxAreaOf(i, j);
      }
    }

    cout << "Case " << ++n << ": ";
    solve();
  }
}
