#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<omp.h>

int i,j,k,a,b,u,v,n,ne=1,edge1,edge2,e;
int min,mincost=0,cost[101][101],parent[101];
int find(int);
int uni(int,int);
void main()
{
 printf("\n\n\tImplementation of Kruskal's algorithm\n\n");
 printf("\nEnter the no. of vertices\n");
 scanf("%d",&n);
 printf("Enter the cost of each cell as adjacency matrix. \n");
for(i=1;i<=n;i++)
 {
  for(j=1;j<=n;j++)
  {
   scanf("%d",&cost[i][j]);
   if(cost[i][j]==0)
    cost[i][j]=999;
  }
 }
 printf("\nThe edges of Minimum Cost Spanning Tree are\n\n");
 #pragma omp parallel reduction(+: mincost), private(min,a,u,v,b)
 {
 while(ne<n)
 {
  for(i=1,min=999;i<=n;i++)
  {
   for(j=1;j<=n;j++)
   {
    if(cost[i][j]<min)
    {
     min=cost[i][j];
     a=u=i;
     b=v=j;
    }
   }
  }
  u=find(u);
  v=find(v);
  if(uni(u,v))
  {
   printf("\n%d edge (%d,%d) =%d\n",ne++,a,b,min);
   mincost +=min;
  }
  cost[a][b]=cost[b][a]=999;
 }
 }
 printf("\n\tMinimum cost = %d\n",mincost);

}
int find(int i)
{
 while(parent[i])
  i=parent[i];
 return i;
}
int uni(int i,int j)
{
 if(i!=j)
 {
  parent[j]=i;
  return 1;
 }
 return 0;
}
