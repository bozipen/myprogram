// K_means.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include<stdio.h> 
#include<math.h> 

int x[15][3]={{3,1,0},{3,2,0},{2,2,0},{3,3,0},{2,3,0},{8,8,0},{8,9,0},{9,8,0},{9,7,0},{9,9,0},{16,5,0},{16,4,0},{15,5,0},{15,6,0},{16,6,0}}; 

double oldcentral[3][3];//�ɵ����ĵ������ 
double newcentral[3][3];//������룬�������ƽ��ֵ�õ����µ����ĵ������ 

int clas[15];//15����������ĸ��࣬��ı�Ŵ�0��ʼ ��0�� 1�� 2
int clsno; //������¼�뵱ǰ����������ĵ�ı�ţ�0�� 1�� 2

int minDist(double x,double y,double z) //��������������Сֵ����������š� 
{ 
	if(x<=y&&x<=z) 
		return 0; 
	if(y<x&&y<=z) 
		return 1; 
	if(z<x&&z<y) 
		return 2; 
} 

double distA(int i,int j)//����������ľ��룬i��j�ֱ�������x��newcentral�е���� ����i��ָ��ǰ�㣬j��ʾ���ĵ㡣���ص�ǰ�㵽���ĵ�ľ���
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
	int count[3];//��¼ÿһ��ĸ����� 
	int i,j; 
	double dist[3];//����������3�����ĵ����ʱ�õ��ı�������¼��ǰ�㵽���ĵ㣨���Ϊ9 1 2���ľ��� 
	double cenL,cenLx,cenLy,cenLz;//������¾����ĵ�����ʱ���õ��ı��� 

	printf("The 15 points are:\n");//�����е�������ӡһ�飬�Ǳ�Ҫ��� 
	for(i=0;i<15;i++) 
		printf("%d %d %d \n",x[i][0],x[i][1],x[i][2]); 

	for(i=0;i<3;i++)//�¾����ĵ㸳��ֵ�� 
		for(j=0;j<3;j++) 
		{ 
			newcentral[i][j]=(double)x[i][j]; 
			oldcentral[i][j]=-9999.0; 
		} 

		for(i=0;i<3;i++)//ͷ3������Ϊ��ʼ�����ĵ㡣 
		{ 
			clas[i]=i; //�������������ڵ�0�࣬��һ�������ڵ�1�࣬������ 
		} 

		while(1)//����ѭ�����˳��������¾����ĵ�ľ���С��0.005. 
		{ 
			for(i=0;i<3;i++)//��¼ÿһ��ĸ��������鸳��ֵ 
				count[i]=0;  

			for(i=0;i<15;i++)//��15����ֱ���㵽���ĵ�ľ��롣 
			{ 
				for(j=0;j<3;j++) 
					dist[j]=distA(i,j); 

				clsno=minDist(dist[0],dist[1],dist[2]);//�������Сֵ�����ؾ�����С�Ķ�Ӧ���ĵ����ꡣ 

				clas[i]=clsno;//���˵�鵽������С����һ�ࡣ 
				count[clsno]++; //��һ�ࣨ���Ϊclsno����Ŀ��1

			} 

			for(i=0;i<3;i++)//�����ĵ�����꿽���������ĵ������У��������ĵ������¼��㡣 
				for(j=0;j<3;j++) 
					oldcentral[i][j]=newcentral[i][j];

			for(i=0;i<3;i++)//�������ĵ����긳��ֵ����������ļ��㡣 
				for(j=0;j<3;j++) 
					newcentral[i][j]=0.0; 

			for(i=0;i<15;i++)//��ÿһ�����������������֮�͡� 
				for(j=0;j<3;j++) 
					newcentral[clas[i]][j] +=x[i][j]; 

			for(i=0;i<3;i++)//����֮�ͳ���count����Ԫ�أ��������ĵ����� 
				for(j=0;j<3;j++) 
					newcentral[i][j]=newcentral[i][j]/count[i]; 

			int flag=0;//��־ 
			for(i=0;i<3;i++)//���¾����ĵ�ľ��� 
			{ 
				cenLx=newcentral[i][0]-oldcentral[i][0]; 
				cenLy=newcentral[i][1]-oldcentral[i][1]; 
				cenLz=newcentral[i][2]-oldcentral[i][2]; 

				cenL=sqrt(cenLx*cenLx+cenLy*cenLy+cenLz*cenLz); 

				if(cenL>0.005)//ֻҪ��һ��������󣬱���δ���������¿�ʼѭ�� 
					flag=1; 
			} 

			if(flag!=1)//ֻ�е���־δ�����ã��˳�whileѭ���� 
				break; 
		} 

		for(i=0;i<15;i++) //��ӡ15��������������ࡣ 
		{ 
			printf("point %d(%d,%d,%d) belongs to class %d\n",i,x[i][0],x[i][1],x[i][2],clas[i]); 
		} 
		getchar();
}
