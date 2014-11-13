/************************************************************************************
* File Name: arma.h
* Desc: arma算法相关的结构声明,包括：
       1.
       2.
       3.
* Author:maxb
* Creation Date:2014-11-04
* Revision history:
*      1.
*      2.
*************************************************************************************/

#ifndef __ARMA_H__
#define __ARMA_H__

#include <math.h>
#include <vector>
#include <list>
#include <string>
#include <iostream>
using namespace std;


typedef double DataType;     //数据类型定义

typedef struct
{
	DataType value;		//数据测量值
	string   time; 		//数据测量时间，需要标准格式 yyyy-mm-dd hh24:mi:ss
	short    flag;     	//数据是否异常，1:异常，0:正常
}Data;

typedef struct
{
	int p;
	int q;
	DataType bic_value;

}S_BIC;

typedef vector<DataType> CData;     //序列数据集合
typedef vector<Data>     CDataSet;  //序列数据集合
typedef vector<S_BIC>    CBICSet;   //BIC集合



class CARMA
{
	
	public:
		CARMA(void);		
		~CARMA();
		
		//接口
		CARMA(CDataSet& his_data, int p, int q);//预测模型接口
		void get_threshold(int k, int A_m, int theta, int t);//阈值范围接口
		
		//结果输出接口
		
			
	private:		
		
		//预测模型求解相关
		void	get_model();	//确定模型p,q值
		int	parameter_estimation(int p, int q, CData &v_p, CData &v_q);//根据p,q值，进行最小二乘参数估计
		DataType	get_bic_value(int p, int q, CData &v_p, CData &v_q);//根据p,q，参数值，获取BIC值
		DataType get_mean_data();//获取数据均值
		void	print_his_data();//打印历史数据信息
		
		int	get_max_pq(int p, int q){ return (p > q) ? p : q; };
		
		//遗忘曲线求解相关	
		int forget_curve_division(int k, int A_m, int theta, int t);//遗忘曲线获取积分面积为1的t值
		DataType get_integral(int A_m, int theta, DataType a, DataType b)//根据遗忘曲线参数求曲线积分



		//聚类相关方法
		void k_means(int k, DataType means[]);//K均值聚类
		int clusterOfTuple(int k, DataType means[],Data tuple);//获取当前点属于哪个聚类
		DataType getVar(int k, vector<Data> clusters[],DataType means[]);//获得给定聚类的平方误差
		DataType getMeans(vector<Data> cluster);//获得当前聚类的均值
		DataType getdistXY(Data &x, Data &y);//获取两个数据点的欧氏距离


		
		//矩阵运算相关
		int InitMatrix(double** matrix, int row, int column);//初始化矩阵
		void FreeMatrix(double **matrix, int row, int column);//释放矩阵
		int TranspositionMatrix(double** matrix, double** transpositionMatirx, int row, int column);//矩阵转置矩阵
		double DeterminantOfMatrix(double** matrix, int row, int column);//矩阵行列式求值
		int InverseMatirx(double** matrix, double** inverseMatirx, int row, int column);//求逆矩阵
		int MultiplyMatrix(double** matrix1, int row1, int column1, double** matrix2, int row2, int column2, double** matrix3);//矩阵相乘
			

	private:						
		int	m_p;				//ARMA模型p值
		int	m_q;				//ARMA模型q值				
		CDataSet m_his_data;	//历史数据
		int m_data_size;		//历史数据的长度		
		CData m_thresholds;		//阈值
		CBICSet m_bic;			//BIC值

	
};






#endif




