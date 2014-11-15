/************************************************************************************
* File Name: arma.h
* Desc: armaԤ���㷨��صĽṹ����,������
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

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

const int SUCC=0;
const int FAIL=-1;

typedef double DataType;     //�������Ͷ���

typedef struct
{
	DataType value;		//���ݲ���ֵ
	string   time; 		//���ݲ���ʱ�䣬��Ҫ��׼��ʽ yyyy-mm-dd hh24:mi:ss
	short    flag;     	//�����Ƿ��쳣��1:�쳣��0:����
}Data;

typedef struct
{
    int p;
    int q;
    DataType bic_value;

}S_BIC;


typedef vector<DataType> CData;     //�������ݼ���
typedef vector<Data>     CDataSet;  //�������ݼ���
typedef vector<S_BIC>    CBICSet;   //BIC����


class CARMA
{

	public:
		CARMA(void);
		~CARMA();
		
		//�ӿ�
		CARMA(CDataSet& his_data, int p, int q);            //Ԥ��ģ�ͽӿ�
		int get_threshold(int k, int A_m, int theta, int t);//��̬��ֵ�ӿ�
		
		//�������ӿ�
		

	private:	
	
		//Ԥ��ģ��������
		int	get_model();	                                            //ȷ��ģ��p,qֵ
		DataType    get_mean_data();                                    //��ȡ���ݾ�ֵ
		void    print_his_data();                                       //��ӡ��ʷ������Ϣ
		void    update_forest_data(CData &v_next);                      //�洢Ԥ������
		int get_forest_segment();                                       //����Ԥ������ֵ�붯̬��ֵd������������Ԥ��
		int	get_max_pq(int p, int q){ return (p > q) ? p : q; };        //��ȡp,q�ϴ�ֵ
		int	parameter_estimation(int p, int q, CData &v_p, CData &v_q); //����p,qֵ��������С���˲�������
        DataType    get_bic_value(int p, int q, CData &v_p, CData &v_q, CData &v_next);//����p,q������ֵ����ȡBICֵ	    
	
	
	    //����Ԥ��
	    int get_forest_segment(string time);                           //����ʱ���ȡԤ������
	    int getcurrdata(string time, CDataSet& data);                  //����ʱ���ȡ��ǰʱ��������ʷ����
	    DataType get_forest_rate(DataType forest_value, DataType threshold, CDataSet& data);//��ȡ��ǰʱ��ĳһ��ֵ�µ�Ԥ����
	
		//��������������
		DataType get_integral(int A_m, int theta, DataType a, DataType b);//�����������߲��������߻���


		//������ط���
		void k_means(int k, DataType means[]);                           //K��ֵ����
		int getcluster(int k, DataType means[],Data tuple);              //��ȡ��ǰ�������ĸ�����
		DataType getvar(int k, vector<Data> clusters[],DataType means[]);//��ø��������ƽ�����
		DataType getmeans(vector<Data> cluster);                         //��õ�ǰ����ľ�ֵ
		DataType getdistXY(DataType x, Data y);                          //��ȡ�������ݵ��ŷ�Ͼ���


		//�����������
		int init_matrix(double*** matrix, int row, int column);                              //��ʼ������
		void    free_matrix(double **matrix, int row, int column);                          //�ͷž���
		void    print_matrix(double** matrix, int row, int column);                         //��ӡ����
		int trans_matrix(double** matrix, double*** trans_matirx, int row, int column);      //����ת�þ���
		double  determ_matrix(double** matrix, int row, int column);                        //��������ʽ��ֵ
		int inverse_matirx(double** matrix, double*** inverse_matirx, int row, int column);  //�������
		int multiply_matrix(double** matrix1, int row1, int column1, double** matrix2, int row2, int column2, double*** matrix_result);//�������



	private:						
		int	m_p;				 //ARMAģ��pֵ
		int	m_q;				 //ARMAģ��qֵ				
		CDataSet m_his_data;	 //��ʷ����
		CDataSet m_forest_data;	 //Ԥ������
		int m_data_size;		 //��ʷ���ݵĳ���		
		CData m_thresholds;		 //��ֵ
		DataType m_min_threshold;//��ֵ����
		DataType m_max_threshold;//��ֵ����
		CBICSet m_bic;			 //BICֵ����


	
};



#endif

