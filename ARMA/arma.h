/************************************************************************************
* File Name: arma.h
* Desc: arma�㷨��صĽṹ����,������
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


typedef double           DataType;     //�������Ͷ���
typedef vector<DataType> CDataSet;     //�������ݼ���



class CARMA
{

	public:
		CARMA(void);
		CARMA(CDataSet& his_data, DataType &next_data);
		~CARMA();
	
	
	private:
		void	get_acf();		//�����ϵ��
		void	get_pacf();		//ƫ�����ϵ��
		void	get_model();	//ȷ��ģ��p,qֵ
		
		void	get_model_parameter();
				
		CDataSet	m_acf;
		CDataSet	m_pacf;
		
		int	m_p;
		int	m_q;
		

			
};





#endif
