#include<stdio.h>
#include<stdlib.h>

const int d=+2;const int e=-2;
int x,y;
float z;
int temp;
char a,b,c;
int h[10];
int i[10];
int j[10];
int k[20][30];
char m[10];
void init (int a )
{
{
if(a<5)
a=a+1;
for(temp=0;temp<=9;temp++)
{
h[temp]=2*temp+2;
i[temp]=(temp+a)*2;
}

}
}
void print (int a )
{
{
for(temp=0;temp<=9;temp++)
printf(j[temp]);

}
}
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
init(3);
temp=0;
while(temp<=9){
j[temp]=gcd(h[temp],i[temp]);
temp=temp+1;
}
print(1);
return 0;
}