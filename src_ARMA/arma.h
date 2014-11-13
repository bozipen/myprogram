/************************************************************************************
* File Name: arma.h
* Desc: arma�㷨��صĽṹ����,������
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


typedef double DataType;     //�������Ͷ���

typedef struct
{

	DataType	value;//���ݲ���ֵ
	char	time[32]; //���ݲ���ʱ�䣬��Ҫ��׼��ʽ yyyy-mm-dd hh24:mi:ss
	short	flag;     //�����Ƿ��쳣��1��ʾ�쳣��0��ʶ����

}Data, *pData;


typedef vector<Data> CDataSet;     //�������ݼ���


class CARMA
{

	public:
		CARMA(void);
		CARMA(CDataSet& his_data, int p, int q, Data &next_data);
		~CARMA();
	
	
	private:		
		void	get_model();	//ȷ��ģ��p,qֵ
		void	parameter_estimation(int p, int q);//����p,qֵ��������С���˲�������
		void	get_bic_value(p, q);//����p,qֵ����ȡBICֵ
		
		vector<int> p_set;
		vector<int> q_set;
		vector<DataType> bic_set;
								
		int	m_p;
		int	m_q;
		
			
};





#endif


