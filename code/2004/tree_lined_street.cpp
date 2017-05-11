#include <stdio.h>
#include <math.h>
#include <iostream>
#include <algorithm>
using namespace std;
struct line {               //line structure with two points (x1,y1) (x2,y2)
  int x1,y1,x2,y2;
};

int cases,n,ans,s,i,j,k;    //global variables
// cases : case number    n : number of lines   ans : output
// s : number of intersections on one certain line    i, j, k: temporary variables  

line a[110];                //record lines
double b[200];              //record each intersection on one line by its distance to the origin
double len;
bool check(int x1,int y1,int x2,int y2,int x3,int y3) {  //check if p(x1,y1)(x2,y2) is at the clock-wise side of p(x1,y1)(x3,y3)                                                       //used for determining if two lines intersect
   long long t1,t2;
   t1=(long long)x1*(long long)y2-(long long)x2*(long long)y1;
   t2=(long long)x1*(long long)y3-(long long)x3*(long long)y1;
   return((t1>=0&&t2<=0)||(t1<=0&&t2>=0));
}

bool intersect(line A, line B) {                        //check if two points are on the different side of one line and vice versa, if both so they intersect
  return (check(A.x2-A.x1,A.y2-A.y1,B.x1-A.x1,B.y1-A.y1, B.x2-A.x1,B.y2-A.y1)
  && check(B.x2-B.x1,B.y2-B.y1,A.x1-B.x1,A.y1-B.y1, A.x2-B.x1,A.y2-B.y1));
}

double dis(double x,double y) { //distance between one node to the origin
   return sqrt(x*x+y*y);
}

void cross(line A,line B) { //for every two lines determine if they intersect, if so update b and s
   double A1,B1,C1,A2,B2,C2,x,y;
   A1=A.y2-A.y1;B1=A.x1-A.x2;C1 = -(A.x1*A1 + A.y1*B1);
   A2=B.y2-B.y1; B2=B.x1-B.x2; C2 =-(B.x1*A2+B.y1*B2);
   x=-(C1*B2-C2*B1) / (A1*B2-A2*B1);
   y=-(A1*C2-A2*C1) / (A1*B2-A2*B1);
   double l = dis(x-A.x1,y-A.y1);
   if (isfinite(l) && l < dis(A.x2-A.x1,A.y2-A.y1) ) {b[s] = l;}
}

bool init() { //initiate and input data to the global variable, if get 0 end
   int i;
   scanf("%d",&n);
   if(n==0)
   return false;
   for(i=1;i<=n;i++)
    scanf("%d %d %d %d", &a[i].x1,&a[i].y1,&a[i].x2,&a[i].y2);
   return true;
}

int main() {
   cases=0;
   while(init()) {
    ans=0;
    for(i=1;i<=n;i++){
      s = 2;
      b[1]=-25; b[2]=dis(a[i].x2-a[i].x1,a[i].y2-a[i].y1)+25;   //mark two end point 25m further so they can be viewed as intersections
      for(j=1;j<=n;j++)
        if(i!=j) //if two lines are the same skip
        if(intersect(a[i], a[j])) { //if two lines intersect
          s++;cross(a[i],a[j]); //increase the number of intersections (s), and call cross to update b
        }
      sort(b+1, b+s+1); //sort b so that all nodes are arranged in order
      for(j = 1; j < s ; j++) { // for each segments the max number of trees to be planted was [l/50] (lower bound)
        len = b[j+1] - b[j];
        k = (int)(len/50);
        ans+=k; //add to the output
      }
    }
    printf("Map %d\nTrees = %d\n",++cases,ans);
  }
}
