#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


double get_integral(int A_m, int theta, double a, double b);


int main(int argc, char** argv)
{
    
    int i,j,k;
    
    double value = 0.00;
    
	for (i=3; i<4; i++)
	{
		for (j=2; j<3; j++)
		{
			for (k=2; k<5; k++)
			{
			    value = get_integral(i, j, (double)0, (double)k);
			    printf("value=%f \n", value);
			}
		}
	}
    
    return 0;
    
}


double get_integral(int A_m, int theta, double a, double b)
{
	int i, j;
	
	double value;
	
	double s,x,h;
	
	int n=10;//积分和划分区间
	
	h=(double)(b-a)/(double)n;
	
	printf("A_m=%d, theta=%d \n", A_m, theta);
	printf("a=%f, b=%f \n", a,b);

    printf("hhhh=%f\n", h);
	
	s=0;
	double ss=0;
	for(i=1;i<n;i++)
	{
		x=a+i*h;	
			
		s = s + A_m*(exp(-(theta*x)));
		ss = A_m*(exp(-(theta*x)));
		printf("ssssss=%f\n", ss);
	}
	
	s=s*h;
	

	return s;
	
}