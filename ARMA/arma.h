/************************************************************************************
* File Name: arma.h
* Desc: 预测算法相关的结构声明,包括：
       1.
       2.
       3.
* Author:
* Creation Date:2014-11-04
* Revision history:
*      1.
*      2.
*************************************************************************************/

#ifndef __ARMA_H__
#define __ARMA_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

const int SUCC=0;
const int FAIL=-1;

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


typedef vector<DataType> CData;     //数据值集合
typedef vector<Data>     CDataSet;  //序列数据集合
typedef vector<S_BIC>    CBICSet;   //BIC集合


class CARMA
{

	public:
		CARMA(void);
		~CARMA();
				
		//预测模型接口
		CARMA(CDataSet& his_data, int p, int q);	
		//预测BIC、预测误差图接口
		int get_forecast_graph(CBICSet& bic_set, CDataSet& forecast_data);     			    
	    //动态阈值接口
	    int get_forecast_region(int k, int A_m, int theta, int t, string time, DataType &forecast_value, DataType &threshold);

				
	private:	
	
		//预测模型求解相关
		int	get_model();	                                            //确定模型p,q值
		DataType    get_mean_data();                                    //获取数据均值
		void    print_his_data();                                       //打印历史数据信息
		void    update_forest_data(CData &v_next);                      //存储预测数据
		int	get_max_pq(int p, int q){ return (p > q) ? p : q; };        //p,q较大值
		int	parameter_estimation(int p, int q, CData &v_p, CData &v_q); //根据p,q值，进行最小二乘参数估计
        DataType    get_bic_value(int p, int q, CData &v_p, CData &v_q, CData &v_next);//根据p,q，参数值，获取BIC值	    
	    void    print_bic_data();                                       //打印BIC数据信息
	    void    sort_clusters(int k, vector<Data> clusters[]);         //对聚类按照元素时间进行排序
	    void get_latest_time(vector<Data> &clusters, string &latest_time);
	    
	    //区间预测
	    int getcurrdata(string time, CDataSet& data);                   //根据时间获取当前时刻所有历史数据
	    DataType get_forecast_rate(DataType forecast_value, DataType threshold, CDataSet& data);//获取当前时刻某一阈值下的预警率
		int get_threshold(int k, int A_m, int theta, int t);                //动态阈值生成，利用K均值聚类算法结合遗忘曲线模型
		DataType get_integral(DataType A_m, DataType theta, DataType a, DataType b);  //根据遗忘曲线参数求曲线积分


		//聚类相关方法
		void k_means(int k, DataType means[]);                           //K均值聚类
		int getcluster(int k, DataType means[],Data tuple);              //获取当前点属于哪个聚类
		DataType getvar(int k, vector<Data> clusters[],DataType means[]);//获得给定聚类的平方误差
		DataType getmeans(vector<Data> cluster);                         //获得当前聚类的均值
		DataType getdistXY(DataType x, Data y);                          //获取两个数据点的欧氏距离


		//矩阵运算相关
		int init_matrix(DataType*** matrix, int row, int column);                              //初始化矩阵
		void    free_matrix(DataType **matrix, int row, int column);                           //释放矩阵
		void    print_matrix(DataType** matrix, int row, int column);                          //打印矩阵
		int trans_matrix(DataType** matrix, DataType*** trans_matirx, int row, int column);    //矩阵转置矩阵
		DataType  determ_matrix(DataType** matrix, int row, int column);                       //矩阵行列式求值
		int inverse_matirx(DataType** matrix, DataType*** inverse_matirx, int row, int column);//求逆矩阵
		int multiply_matrix(DataType** matrix1, int row1, int column1, DataType** matrix2, int row2, int column2, DataType*** matrix_result);//矩阵相乘


	private:						
		int	m_p;				 //ARMA模型p值
		int	m_q;				 //ARMA模型q值				
		CDataSet m_his_data;	 //历史数据
		CDataSet m_mean_data;	 //均值处理后历史数据
		CDataSet m_forecast_data;//预测数据
		int m_data_size;		 //历史数据的长度		
		CData m_thresholds;		 //阈值
		DataType m_min_threshold;//阈值下限
		DataType m_max_threshold;//阈值上限
		DataType m_data_mean;    //均值
		DataType m_next;         //预测未知点
		CBICSet m_bic;			 //BIC值集合


	
};



#endif

