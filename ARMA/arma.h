/************************************************************************************
* File Name: arma.h
* Desc: arma算法相关的结构声明,包括：
       1.
       2.
       3.
* Author:maxb
* Creation Date:2014-10-24
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


typedef double           DataType;     //数据类型定义
typedef vector<DataType> CDataSet;     //序列数据集合



class CARMA
{

	public:
		CARMA(void);
		CARMA(CDataSet& his_data, DataType &next_data);
		~CARMA();
	
	
	private:
		void	get_acf();		//自相关系数
		void	get_pacf();		//偏自相关系数
		void	get_model();	//确定模型p,q值
		
		void	get_model_parameter();
				
		CDataSet	m_acf;
		CDataSet	m_pacf;
		
		int	m_p;
		int	m_q;
		

			
};





#endif
