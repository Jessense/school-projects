#include<stdio.h>
#include<stdlib.h>

int a[10];
int x;
int y;
char m;
int z;
void readarray ( )
{
int i;
{
for(i=1;i<=9;i++)
read(a[i]);

}
}
void exchange (int i,int j )
{
{
x=a[i];
a[i]=a[j];
a[j]=x;
}
}
int partition (int y,int z )
{
int i,j;
int pivot;
{
pivot=a[y];
i=y;
j=z;
while(i<j){
while((i<j)&&(a[i]<=pivot))i=i+1;
while((i<j)&&(a[j]>=pivot))j=j-1;
if((i<j))
exchange(i,j);
}
exchange(i,y);
return(i);
}
}
void quicksort (int m,int n )
{
int k;
{
if((m>=n))
return;
k=partition(m,n);
quicksort(m,k-1);
quicksort(k+1,n);
}
}
int main()
{
printf("Please input a sequenc");
readarray;
quicksort(1,9);
for(x=1;x<=9;x++)
scanf(a[x]);

return 0;
}