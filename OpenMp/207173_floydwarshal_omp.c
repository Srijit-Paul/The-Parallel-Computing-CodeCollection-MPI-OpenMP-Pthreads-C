#include<stdio.h> 
#include<stdlib.h> 
#include<sys/time.h> 
#include<omp.h>

int min(int,int); 
int main() 
{ 
    int n,k,i,j,c[10][10]; 
    int tid; 
    omp_set_num_threads(0); 
    { 
        tid=omp_get_thread_num(); 
        printf("Enter the number of nodes:"); 
        scanf("%d",&n); 
        printf("Enter the cost matrix:\n"); 
        for(i=0;i<n;i++) 
        for(j=0;j<n;j++) 
              scanf("%d",&c[i][j]); 
        for(k=0;k<n;k++) 
        {
                for(i=0;i<n;i++) 
                for(j=0;j<n;j++) 
                        c[i][j]=min(c[i][j],c[i][k]+c[k][j]); 
        } 
        printf("\n All pairs shortest path\n"); 
        for(i=0;i<n;i++) 
        {
               for(j=0;j<n;j++) 
            printf("%d\t",c[i][j]); 
               printf("\n"); 
        } 
    } 
    return 0; 
}

int min(int a,int b)

{

  return(a<b?a:b);

}
