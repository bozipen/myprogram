// K_means.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include<stdio.h> 
#include<math.h> 

int x[15][3]={{3,1,0},{3,2,0},{2,2,0},{3,3,0},{2,3,0},{8,8,0},{8,9,0},{9,8,0},{9,7,0},{9,9,0},{16,5,0},{16,4,0},{15,5,0},{15,6,0},{16,6,0}}; 

double oldcentral[3][3];//旧的中心点的坐标 
double newcentral[3][3];//计算距离，分类后求平均值得到的新的中心点的坐标 

int clas[15];//15个点各属于哪个类，类的编号从0开始 ：0， 1， 2
int clsno; //用来记录与当前点最近的中心点的编号：0， 1， 2

int minDist(double x,double y,double z) //计算三个数的最小值，返回其序号。 
{ 
	if(x<=y&&x<=z) 
		return 0; 
	if(y<x&&y<=z) 
		return 1; 
	if(z<x&&z<y) 
		return 2; 
} 

double distA(int i,int j)//计算两个点的距离，i和j分别是数组x和newcentral中的序号 ，即i是指当前点，j表示中心点。返回当前点到中心点的距离
{ 
	double distx,disty,distz,dist; 

	distx=(double)x[i][0]-(double)newcentral[j][0]; 
	disty=(double)x[i][1]-(double)newcentral[j][1]; 
	distz=(double)x[i][2]-(double)newcentral[j][2]; 

	dist=sqrt(distx*distx+disty*disty+distz*distz); 

	return dist; 
} 

void main() 
{ 
	int count[3];//记录每一类的个数； 
	int i,j; 
	double dist[3];//求坐标点距离3个中心点距离时用到的变量，记录当前点到中心点（编号为9 1 2）的距离 
	double cenL,cenLx,cenLy,cenLz;//最后求新旧中心点距离的时候用到的变量 

	printf("The 15 points are:\n");//把所有点的坐标打印一遍，非必要语句 
	for(i=0;i<15;i++) 
		printf("%d %d %d \n",x[i][0],x[i][1],x[i][2]); 

	for(i=0;i<3;i++)//新旧中心点赋初值。 
		for(j=0;j<3;j++) 
		{ 
			newcentral[i][j]=(double)x[i][j]; 
			oldcentral[i][j]=-9999.0; 
		} 

		for(i=0;i<3;i++)//头3个点作为初始的中心点。 
		{ 
			clas[i]=i; //假设第零个点属于第0类，第一个点属于第1类，··· 
		} 

		while(1)//无限循环，退出条件是新旧中心点的距离小于0.005. 
		{ 
			for(i=0;i<3;i++)//记录每一类的个数的数组赋初值 
				count[i]=0;  

			for(i=0;i<15;i++)//对15个点分别计算到中心点的距离。 
			{ 
				for(j=0;j<3;j++) 
					dist[j]=distA(i,j); 

				clsno=minDist(dist[0],dist[1],dist[2]);//求距离最小值，返回距离最小的对应中心点坐标。 

				clas[i]=clsno;//将此点归到距离最小的那一类。 
				count[clsno]++; //这一类（编号为clsno）数目加1

			} 

			for(i=0;i<3;i++)//新中心点的坐标拷贝到旧中心点数组中，因新中心点需重新计算。 
				for(j=0;j<3;j++) 
					oldcentral[i][j]=newcentral[i][j];

			for(i=0;i<3;i++)//对新中心点坐标赋初值，进行下面的计算。 
				for(j=0;j<3;j++) 
					newcentral[i][j]=0.0; 

			for(i=0;i<15;i++)//对每一类的坐标点计算其坐标之和。 
				for(j=0;j<3;j++) 
					newcentral[clas[i]][j] +=x[i][j]; 

			for(i=0;i<3;i++)//坐标之和除以count数组元素，即得中心点坐标 
				for(j=0;j<3;j++) 
					newcentral[i][j]=newcentral[i][j]/count[i]; 

			int flag=0;//标志 
			for(i=0;i<3;i++)//求新旧中心点的距离 
			{ 
				cenLx=newcentral[i][0]-oldcentral[i][0]; 
				cenLy=newcentral[i][1]-oldcentral[i][1]; 
				cenLz=newcentral[i][2]-oldcentral[i][2]; 

				cenL=sqrt(cenLx*cenLx+cenLy*cenLy+cenLz*cenLz); 

				if(cenL>0.005)//只要有一个距离过大，表明未收敛，重新开始循环 
					flag=1; 
			} 

			if(flag!=1)//只有当标志未被设置，退出while循环。 
				break; 
		} 

		for(i=0;i<15;i++) //打印15个点各自所属的类。 
		{ 
			printf("point %d(%d,%d,%d) belongs to class %d\n",i,x[i][0],x[i][1],x[i][2],clas[i]); 
		} 
		getchar();
}
