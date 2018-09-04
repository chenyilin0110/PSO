#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define run 30 
#define maxIteration 400 
#define particle 50 
#define Dim 2 
#define Ud 100 
#define a 20
#define b 0.2
#define wStart 0.9
#define wEnd 0.4
 
void countF(double array[],int c,int iteration);

int main()
{
	int ru = 0,i,j;
	double runData[run + 1][maxIteration] = {0};
	while(ru < run)
	{
		int iteration = 1,c1,c2,Ld,vMax,vMin;	
		double particleData[particle][Dim * 3 + 2] = {0},c,r1,r2,best[Dim + 1],w = wStart;
		//particleData = x1 speed1 x2 speed2 f localBest localBestAddressX1 localBestAddressX2 	
		
		//parameter setting
		c = 2.0 * M_PI;//M_PI 3.14159265358979323846
		c1 = c2 = 2;//learning 
		Ld = Ud * (-1);//Ud & Ld
		vMax = Dim * 2;
		vMin = Dim * (-2);
		
		srand(time(NULL));
		
		//initial x address
		for(i=0;i<particle;i++)
		{
			int temp = 0;
			for(j=0;j<Dim;j++)
			{
				int r = rand();
				particleData[i][temp] = ( (Ud - Ld) * r ) / (RAND_MAX + 1.0) + Ld;
				if(particleData[i][temp] > Ud)
					particleData[i][temp] = Ud;
				else if(particleData[i][temp] < Ld)
					particleData[i][temp] = Ld;
				temp = temp + 2;
			}			
		}
		//initial speed 
		for(i=0;i<particle;i++)
		{
			int temp = 1;
			for(j=0;j<Dim;j++)
			{
				int r = rand();
				particleData[i][temp] = ( (vMax - vMin) * r ) / (RAND_MAX + 1.0) + vMin;
				if(particleData[i][temp] > vMax)
					particleData[i][temp] = vMax;
				else if(particleData[i][temp] < vMin)
					particleData[i][temp] = vMin;
				temp = temp + 2;
			}
		}
		//count f & initial f & initial localBest & initial localBestAddress
		for(i=0;i<particle;i++)
			countF(particleData[i],c,iteration);
		//initial best = particleData[0]		
		best[0] = particleData[0][Dim * 2 + 1];
		for(i=1;i<particle;i++)
		{
			if(best[0] > particleData[i][Dim * 2 + 1])
			{
				int temp = 0;
				best[0] = particleData[i][Dim * 2 + 1];
				for(j=1;j<=Dim;j++)
				{
					best[j] = particleData[i][temp];
					temp = temp + 2;
				}
			}
		}
		runData[ru][iteration-1] = best[0];
		while(iteration < maxIteration)
		{
			iteration++;
			//update x & speed
			for(j=0;j<particle;j++)
			{
				int speed = 1;
				for(i=0;i<Dim;i++)
				{
					r1 = (double)rand()/RAND_MAX;
					r2 = (double)rand()/RAND_MAX;
					particleData[j][speed] = w * particleData[j][speed] + (c1 * r1 * (particleData[j][Dim * 2 + i + 2] - particleData[j][speed - 1])) + (c2 * r2 * (best[i + 1] - particleData[j][speed -1]));
					
					if(particleData[j][speed] > vMax)
						particleData[j][speed] = vMax;
					else if(particleData[j][speed] < vMin)
						particleData[j][speed] = vMin;
			
					particleData[j][speed - 1] = particleData[j][speed -1] + particleData[j][speed];
					
					if(particleData[j][speed - 1] > Ud)
						particleData[j][speed - 1] = Ud;
					else if(particleData[j][speed - 1] < Ld)
						particleData[j][speed - 1] = Ld;
					
					speed = speed + 2;
				}	
				countF(particleData[j],c,iteration);
			}
			for(i=1;i<particle;i++)
			{
				if(best[0] > particleData[i][Dim * 2 + 1])
				{
					int temp = 0;
					best[0] = particleData[i][Dim * 2 + 1];
					for(j=1;j<=Dim;j++)
					{
						best[j] = particleData[i][temp];
						temp = temp + 2;
					}
				}
			}
			runData[ru][iteration-1] = best[0];
			w = wStart - (wStart - wEnd) / maxIteration * iteration;
		}
		ru++;
	}
	for(j=0;j<maxIteration;j++)
	{
		for(i=0;i<run;i++)
			runData[run][j] += runData[i][j];
		runData[run][j] = runData[run][j] / run;
	}
	for(i=0;i<maxIteration;i++)
		printf("%d %.16f\n",i + 1,runData[run][i]);
	
}

void countF(double particleData[],int c,int iteration)
{
	double one = 0,second = 0,total;
	int count = 0,f = Dim * 2,i,j;
	for(i=0;i<Dim;i++)
	{
		one += pow(particleData[count],2);
		second += cos(c * particleData[count]);
		count = count + 2;
	}
	one = sqrt(one / Dim) * (-b);
	second = second / Dim;
	particleData[f] = (-a) * exp(one) - exp(second) + a + exp(1);
	count = 0;
	if(iteration == 1)
	{
		particleData[f + 1] = particleData[f];
		// inital localBestAddress
		for(i=0;i<Dim;i++)
		{
			particleData[f + i + 2] = particleData[count];
			count = count + 2;
		}
	}
	else
	{
		if(particleData[f] < particleData[f + 1])
		{
			particleData[f + 1] = particleData[f];
			//save localBestAddress
			for(i=0;i<Dim;i++)
			{
				particleData[f + i + 2] = particleData[count];
				count = count + 2;
			}
		}
	}
}
