#include <stdio.h>
#include <math.h>
#include <iostream>
#include <algorithm>
using namespace std;
struct line {
  int x1,y1,x2,y2;
};

int cases,n,ans,s,i,j,k;

line a[110]; //线段序列
double b[200]; //以b记录某条线段与其他线段的交点到某端点的距离
double len;
bool check(int x1,int y1,int x2,int y2,int x3,int y3) //判断 p(x1,y1)(x2,y2) 是否在 p(x1,y1)(x3,y3) 的顺时针方向
{
   long long t1,t2;
   t1=(long long)x1*(long long)y2-(long long)x2*(long long)y1;
   t2=(long long)x1*(long long)y3-(long long)x3*(long long)y1;
   return((t1>=0&&t2<=0)||(t1<=0&&t2>=0));
}
double dis(double x,double y)
{
   return sqrt(x*x+y*y);
}
void cross(line A,line B)
{
   double A1,B1,C1,A2,B2,C2,x,y;
   A1=A.y2-A.y1;B1=A.x1-A.x2;C1 = -(A.x1*A1 + A.y1*B1);
   A2=B.y2-B.y1; B2=B.x1-B.x2; C2 =-(B.x1*A2+B.y1*B2);
   x=-(C1*B2-C2*B1) / (A1*B2-A2*B1);
   y=-(A1*C2-A2*C1) / (A1*B2-A2*B1);
   double l = dis(x-A.x1,y-A.y1);
   if (isfinite(l) && l < dis(A.x2-A.x1,A.y2-A.y1) ) {b[s] = l;}
}

bool init()
{
   int i;
   scanf("%d",&n);
   if(n==0)
   return false;
   for(i=1;i<=n;i++)
    scanf("%d %d %d %d", &a[i].x1,&a[i].y1,&a[i].x2,&a[i].y2);
   return true;
}

/*
int main() {
  while(init()) {
    for (i = 1; i <= n; i++) {
      for (j = 1; j <= n; j++) {
        if(i!=j) {//判断第 i 条线段与第 j 条线段是否相交,即一条线段的两个端
        if(check(a[i].x2-a[i].x1,a[i].y2-a[i].y1,a[j].x1-a[i].x1,a[j].y1-a[i].y1, a[j].x2-a[i].x1,a[j].y2-a[i].y1)
        && check(a[j].x2-a[j].x1,a[j].y2-a[j].y1,a[i].x1-a[j].x1,a[i].y1-a[j].y1, a[i].x2-a[j].x1,a[i].y2-a[j].y1)) {
          cout << i << " " << j << "intersect" << endl;
       }}}
      }
    }
  return 0;
}
*/


int main() {
   cases=0;
   while(init()) {
    ans=0;
    for(i=1;i<=n;i++){
      s = 2;
      b[1]=-25; b[2]=dis(a[i].x2-a[i].x1,a[i].y2-a[i].y1)+25;   //把两个端点分别拉远 25 米作为虚拟的交点处理
      for(j=1;j<=n;j++) {
        if(i!=j) {//判断第 i 条线段与第 j 条线段是否相交,即一条线段的两个端
        if(check(a[i].x2-a[i].x1,a[i].y2-a[i].y1,a[j].x1-a[i].x1,a[j].y1-a[i].y1, a[j].x2-a[i].x1,a[j].y2-a[i].y1)
        && check(a[j].x2-a[j].x1,a[j].y2-a[j].y1,a[i].x1-a[j].x1,a[i].y1-a[j].y1, a[i].x2-a[j].x1,a[i].y2-a[j].y1)) {
          s++;cross(a[i],a[j]);
        }}}
      sort(b+1, b+s+1);//交点数+1,更新b表
      for(j = 1; j < s ; j++) {
        cout << b[j] << " ";
        len = b[j+1] - b[j];
        k = (int)(len/50);
        while(len-k*50>1e-6)k++;
        while(len-k*50<1e-6)k--;
        ans+=k;
      }
      cout << b[s] << endl;
    }
    printf("Map %d\nTrees = %d\n",++cases,ans);
  }
}
