#include<stdio.h>
#include<stdlib.h>

int x,y,z;
int gcd (int a,int b )
{
{
if(b==0)
return(a);

else
return(gcd(b,a%b));
}
}
int main()
{
x=24;
y=18;
z=gcd(x,y);
printf(z);
return 0;
}