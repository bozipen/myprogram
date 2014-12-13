/************************************************************************************
* File Name: grey_model.h
* Desc: ��ɫģ��Ԥ���㷨��صĽṹ����,������
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

typedef double DataType;     //�������Ͷ���

typedef struct
{
	DataType value;		//���ݲ���ֵ
	string   time; 		//���ݲ���ʱ�䣬��Ҫ��׼��ʽ yyyy-mm-dd hh24:mi:ss
	short    flag;     	//�����Ƿ��쳣��1:�쳣��0:����
}Data;

typedef struct
{
	DataType fitness;		//fitnessֵ
	DataType a_value; 		//a
	int      u_value;     	//u
}Fitness;


typedef vector<DataType> CData;       //����ֵ����
typedef vector<Data>     CDataSet;    //�������ݼ���
typedef vector<Fitness>  CFitnessSet; //�������ݼ���

class CGreyModel
{

	public:
		CGreyModel(void);
		~CGreyModel();
				
		//��ɫģ�ͽӿ�
		CGreyModel(CDataSet& his_data);	
		

				
	private:	
	
		//Ԥ��ģ��������
		int	get_model();
		int	forecast_data();
		int gen_rand_solution(int num);//��ȡnum��a��u��Ϊ��				
		DataType get_rand_a();  //��ȡa�����
		int get_rand_u();       //��ȡu�����
		
		DataType get_fitness(DataType a, int u);
		
		
		
		int	parameter_estimation(); 
 
 
	private:
	    int m_init_population_num;      //��ʼ��Ⱥ��Ŀ
	    int m_max_population_num;       //�����Ⱥ��Ŀ
	    int m_min_seed;                 //��С������
	    int m_max_seed;                 //���������
	    int m_max_iteration_num;        //����������
	    int m_dimension;                //���ά��
	    int m_nonlinear_parameter;      //�����Ե��Ʋ���
	    DataType m_init_stand_dev;      //��ʼ��׼��
	    DataType m_final_stand_dev;	    //���ձ�׼��       
	    DataType m_a_min;               //a�����Χ����
	    DataType m_a_max;	            //a�����Χ����   
	    int m_u_min;                    //u�����Χ����
	    int m_u_max;                    //u�����Χ����
	    
	    DataType m_fitness_best;
	    DataType m_a_best;
	    int m_u_best;
	    int m_curr_population_num;//��ǰ����Ⱥ��Ŀ
	    
	    CFitnessSet fitness_set;
//	    vector<int> seed_num;
	    
				
		CDataSet m_his_data;	 //��ʷ����
		CDataSet m_mean_data;	 //��ֵ�������ʷ����
		CDataSet m_forecast_data;//Ԥ������
		CDataSet m_curr_data;    //��ǰʱ��������ʷ����
		int m_data_size;		 //��ʷ���ݵĳ���		
		CData m_thresholds;		 //��ֵ
		DataType m_min_threshold;//��ֵ����
		DataType m_max_threshold;//��ֵ����
		DataType m_data_mean;    //��ֵ
		DataType m_next;         //Ԥ��δ֪��
	


	
};



#endif

