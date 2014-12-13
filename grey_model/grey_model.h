/************************************************************************************
* File Name: grey_model.h
* Desc: 灰色模型预测算法相关的结构声明,包括：
       1.
       2.
       3.
* Author:
* Creation Date:2014-12-12
* Revision history:
*      1.
*      2.
*************************************************************************************/

#ifndef __GREY_MODEL_H__
#define __GREY_MODEL_H__

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
	DataType fitness;		//fitness值
	DataType a_value; 		//a
	int      u_value;     	//u
}Fitness;


typedef vector<DataType> CData;       //数据值集合
typedef vector<Data>     CDataSet;    //序列数据集合
typedef vector<Fitness>  CFitnessSet; //序列数据集合

class CGreyModel
{

	public:
		CGreyModel(void);
		~CGreyModel();
				
		//灰色模型接口
		CGreyModel(CDataSet& his_data);	
		

				
	private:	
	
		//预测模型求解相关
		int	get_model();
		int	forecast_data();
		int gen_rand_solution(int num);//获取num个a，u作为解				
		DataType get_rand_a();  //获取a随机数
		int get_rand_u();       //获取u随机数
		
		DataType get_fitness(DataType a, int u);
		
		
		
		int	parameter_estimation(); 
 
 
	private:
	    int m_init_population_num;      //初始种群数目
	    int m_max_population_num;       //最大种群数目
	    int m_min_seed;                 //最小种子数
	    int m_max_seed;                 //最大种子数
	    int m_max_iteration_num;        //最大迭代次数
	    int m_dimension;                //求解维数
	    int m_nonlinear_parameter;      //非线性调制参数
	    DataType m_init_stand_dev;      //初始标准差
	    DataType m_final_stand_dev;	    //最终标准差       
	    DataType m_a_min;               //a随机范围下限
	    DataType m_a_max;	            //a随机范围上限   
	    int m_u_min;                    //u随机范围下限
	    int m_u_max;                    //u随机范围上限
	    
	    DataType m_fitness_best;
	    DataType m_a_best;
	    int m_u_best;
	    int m_curr_population_num;//当前的种群数目
	    
	    CFitnessSet fitness_set;
//	    vector<int> seed_num;
	    
				
		CDataSet m_his_data;	 //历史数据
		CDataSet m_mean_data;	 //均值处理后历史数据
		CDataSet m_forecast_data;//预测数据
		CDataSet m_curr_data;    //当前时刻所有历史数据
		int m_data_size;		 //历史数据的长度		
		CData m_thresholds;		 //阈值
		DataType m_min_threshold;//阈值下限
		DataType m_max_threshold;//阈值上限
		DataType m_data_mean;    //均值
		DataType m_next;         //预测未知点
	


	
};



#endif

