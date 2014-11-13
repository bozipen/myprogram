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

#include <vector>
#include <list>
#include <string>
#include <iostream>

using namespace std;


typedef double DataType;     //数据类型定义

typedef struct
{

	DataType	value;//数据测量值
	char	time[32]; //数据测量时间，需要标准格式 yyyy-mm-dd hh24:mi:ss
	short	flag;     //数据是否异常，1表示异常，0标识正常

}Data, *pData;


typedef vector<Data> CDataSet;     //序列数据集合


class CARMA
{

	public:
		CARMA(void);
		CARMA(CDataSet& his_data, int p, int q, Data &next_data);
		~CARMA();
	
	
	private:		
		void	get_model();	//确定模型p,q值
		void	parameter_estimation(int p, int q);//根据p,q值，进行最小二乘参数估计
		void	get_bic_value(p, q);//根据p,q值，获取BIC值
		
		vector<int> p_set;
		vector<int> q_set;
		vector<DataType> bic_set;
								
		int	m_p;
		int	m_q;
		
			
};





#endif


