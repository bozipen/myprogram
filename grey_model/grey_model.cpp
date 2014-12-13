#include "grey_model.h"


CGreyModel::CGreyModel(void)
{
    
}

CGreyModel::~CGreyModel()
{
    
}

CGreyModel::CGreyModel(CDataSet& his_data)
{
	
	m_his_data.assign(his_data.begin(), his_data.end());
	m_forecast_data.assign(his_data.begin(), his_data.end());
	
	m_data_size = m_his_data.size();
	cout<<"输入数据大小：m_data_size="<<m_data_size<<endl;

	
//	print_his_data();
		
    m_init_population_num = 5;  //初始种群数目  
    m_max_population_num = 100; //最大种群数目  
    m_min_seed = 0;             //最小种子数    
    m_max_seed = 5;             //最大种子数    
    m_max_iteration_num = 300;  //最大迭代次数  
    m_dimension = 2;            //求解维数      
    m_nonlinear_parameter = 2;  //非线性调制参数
    m_init_stand_dev = 50.00;   //初始标准差    
    m_final_stand_dev = 0.01;   //最终标准差       
    m_a_min = -0.30;            //a随机范围下限 
    m_a_max = 2.00;	            //a随机范围上限 
    m_u_min = 1.00;             //u随机范围下限 
    m_u_max = 20.00;            //u随机范围上限
    
    m_curr_population_num = 0; 


   
    get_model();
    
    forecast_data();
    


}



int CGreyModel::get_model()
{
	int i,j;
		
	int fitness_size = 0;	
	DataType fitness_max = 0.0000;
	DataType last_a = 0.00;
	int last_u = 0;
	DataType epsinon  = 0.000000000001;
	
	//多次迭代
	for (i=0; i<m_max_iteration_num; i++)
	{
	    gen_rand_solution(m_init_population_num);
	    	    
	    fitness_size = fitness_set.size();
	    
	    for (j=0; j<fitness_size; j++)
	    {
    		//记录最大的fitness
    		if ( fitness_set[j].fitness - fitness_max >= epsinon )
    		{
    			fitness_max = fitness_set[j].fitness;
    			last_a = 	fitness_set[j].a_value;
    			last_u = 	fitness_set[j].u_value;   					         	
    		}
	    }
	    	    	 
	    fitness_set.clear();
	    m_curr_population_num = 0;
	    
	}
	
	cout<<"last_a="<<last_a<<endl;
	cout<<"last_u="<<last_u<<endl;
	cout<<"fitness_max="<<fitness_max<<endl;
	
	m_fitness_best = fitness_max;
	m_a_best = last_a;
	m_u_best = last_u;
	
			
	return SUCC;

}

int CGreyModel::forecast_data()
{
    int i;
             
    for (i=0; i<m_data_size; i++)
    {		
        DataType temp = 0.00;
    
        if (i == 0)
        {			    			    
            temp = m_his_data[0].value;//利用数据值作为预测值      
        }
        else if (i<m_data_size)
        {
        	//利用预测公式进行预测
        	temp = (1-exp(m_a_best))*(m_his_data[0].value - (DataType)m_u_best/m_a_best)*(exp(-m_a_best*i));
        	m_forecast_data[i].value = temp;              
        }

              
        cout<<"【"<<i<<"】:"<<m_his_data[i].value<<","<<temp<<endl;
        
           					
    }
    
    //利用预测公式进行预测下一个点值
    m_next = (1-exp(m_a_best))*(m_his_data[0].value - (DataType)m_u_best/m_a_best)*(exp(-m_a_best*m_data_size));
    
    cout<<"m_next="<<m_next<<endl;
    
    return SUCC;
}

int CGreyModel::gen_rand_solution(int num)
{
    if (num < 1)
    {
        return FAIL;
    }
    
    if (m_curr_population_num >= m_max_population_num)
    {
        return FAIL;
    }
    
    
    DataType temp_a = 0.00;
    int temp_u = 0;
    
    DataType fitness = 0.00;
    DataType fitness_min = 100000.00;
    DataType fitness_max = 0.00;
    DataType epsinon  = 0.000000000001;
    
//    CData fitness_set;
    
//    vector<int> seed_num;
    
    srand((unsigned)time(NULL)); /*随机数初始化*/
    
    int i;
    for (i=0; i<num; i++)
    {
        temp_a = get_rand_a();
        temp_u = get_rand_u();  
        
//        cout<<"temp_a="<<temp_a<<endl;
//        cout<<"temp_u="<<temp_u<<endl;
        
        
//        a.push_back(temp_a);
//        u.push_back(temp_u);
             
        fitness = get_fitness(temp_a, temp_u);
        
        Fitness temp;
        temp.fitness = fitness;
        temp.a_value = temp_a;
        temp.u_value = temp_u;
        
        
        fitness_set.push_back(temp);
        
//        seed_num.push_back(0);
        
    	//记录最小的fitness
		if ( fitness_min - fitness >= epsinon )
		{
			fitness_min = fitness;				         	
		}
		
		//记录最大的fitness
		if ( fitness - fitness_max >= epsinon )
		{
			fitness_max = fitness;				         	
		}
		
		
		cout<<"fitness="<<fitness<<endl;
		                
    }
    
//    cout<<"fitness_max="<<fitness_max<<endl;
//    cout<<"fitness_min="<<fitness_min<<endl;
    
    
    DataType coefficient = 0.00;   
    coefficient = (DataType)(m_max_seed-m_min_seed)/(DataType)(fitness_max-fitness_min);
    
    int seed_num_temp = 0;
    for (i=0; i<num; i++)
    {
        seed_num_temp = (int)(coefficient*(fitness_set[i].fitness - fitness_min)+m_min_seed);
//        cout<<"seed_num_temp="<<seed_num_temp<<endl;
        
        if (seed_num_temp > 1)
        {
           m_curr_population_num += seed_num_temp;
           gen_rand_solution(seed_num_temp);
           
           cout<<"m_curr_population_num="<<m_curr_population_num<<endl;
            
        }
        
        
    }
    
    
    

		
	return SUCC;
}

DataType CGreyModel::get_rand_a()
{   
    DataType temp = 0.00;
    
    temp = m_a_min + rand() * 1.0 * (m_a_max - m_a_min) / RAND_MAX;
		
	return temp;
}

int CGreyModel::get_rand_u()
{
    int temp = 0;
      
    temp = (rand()%(m_u_max - m_u_min)) + m_u_min;
				
	return temp;
}

DataType CGreyModel::get_fitness(DataType a, int u)
{
    int i;
    
    CData v_next_temp;
    DataType sum_eps = 0.00;
      
    for (i=0; i<m_data_size; i++)
    {		
        DataType temp = 0.00;
    
        if (i == 0)
        {			    			    
            temp = m_his_data[0].value;//利用数据值作为预测值      
        }
        else
        {
        	//利用预测公式进行预测
        	temp = (1-exp(a))*(m_his_data[0].value - (DataType)u/a)*(exp(-a*i));
        	              
        }
        		
        v_next_temp.push_back(temp);
        
        sum_eps += pow((m_his_data[i].value - temp),2);
           					
    }
    
    sum_eps = (DataType)1/(sum_eps/m_data_size);
   
				
	return sum_eps;
}















