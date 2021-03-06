#include <arma.h>


CARMA::CARMA(void)
{
    
}

CARMA::~CARMA()
{
    
}

CARMA::CARMA(CDataSet& his_data, int p, int q)
{
	
	m_his_data.assign(his_data.begin(), his_data.end());
	m_forecast_data.assign(his_data.begin(), his_data.end());
	
	m_data_size = m_his_data.size();
	cout<<"输入数据大小：m_data_size="<<m_data_size<<endl;

	
//	print_his_data();
		
	m_p = p;
	m_q = q;

    cout<<"p="<<m_p<<","<<"q="<<m_q<<endl;
    
    get_model();
    
//    print_bic_data();


}



int CARMA::get_forecast_graph(CBICSet& bic_set, CDataSet& forecast_data)
{
    
    bic_set.assign(m_bic.begin(), m_bic.end());
     
    forecast_data.assign(m_forecast_data.begin(), m_forecast_data.end());
       
    return 0;
}

int CARMA::get_forecast_region(int k, int A_m, int theta, int t, string time, DataType &forecast_value, DataType &threshold)
{
    
    int i=0; 
    for (i=0; i<m_data_size; i++)
    {
        //具体时间格式需要确定
        if (time.compare(m_forecast_data[i].time) == 0)
        {
            forecast_value = m_forecast_data[i].value;
            break;            
        }
    }
    
    cout<<"time="<<time<<endl;
    cout<<"forecast_value="<<forecast_value<<endl;
    
    
    int data_size = 0;
//    CDataSet currdata;
    
    //获取当前时刻所有历史数据
    m_curr_data.clear();
    
    data_size = getcurrdata(time);
    if (data_size < 1)
    {
        cout<<"getcurrdata fail"<<endl;
        return FAIL;
    }
    
    
    //打印当前数据
    cout<<"当前阈值数据为："<<endl;
	for (i=0; i<data_size; i++)
	{
		cout<<i<<":"<<m_curr_data[i].value<<","<<m_curr_data[i].time<<","<<m_curr_data[i].flag<<endl;
	}
	
       
    //利用K均值聚类结合遗忘曲线生成所有动态阈值
    get_threshold(k, A_m, theta, t);
    
     
    DataType forecast_rate = 0.00;
    DataType temp_rate = 0.00;
    DataType epsinon = 0.0001;
    DataType best_threshold = 0.00;
    
    //获取预警率最大的阈值
    int len = m_thresholds.size(); 
    for (i=0; i<len; i++)
    {
        temp_rate = get_forecast_rate(forecast_value, m_thresholds[i], m_curr_data);
        
        cout<<"预测值="<<forecast_value<<"阈值="<<m_thresholds[i]<<"预警率="<<temp_rate<<endl;
        
        
        if ( (temp_rate - forecast_rate) >= epsinon )
        {
            forecast_rate = temp_rate;
            best_threshold = m_thresholds[i];
        }
               
    }
    
    
    threshold = best_threshold;
    
    cout<<"best_threshold="<<best_threshold<<endl;
    
    m_min_threshold = forecast_value - (DataType)best_threshold;
    m_max_threshold = forecast_value + (DataType)best_threshold;
    
       
    return SUCC;    
}

int CARMA::get_model()
{
	if (m_p < 1 || m_q < 1)
	{
		return FAIL;
	}
	
	int i,j;
	
	int last_p = 0;
	int last_q = 0;
		
	DataType min_bic  = 100000.00;
	DataType temp_bic = 0.00000;
	DataType epsinon  = 0.00001;
		
	//穷举法利用p,q进行参数估计
	for (i=1; i<=m_p; i++)
	{
		for (j=1; j<=m_q; j++)
		{
			vector<DataType> v_p;
			vector<DataType> v_q;
			
//			cout<<"parameter_estimation"<<endl;
			
			if (parameter_estimation(i, j, v_p, v_q) == FAIL)	//参数估计
			{
			    continue;
			}	
//			parameter_estimation(i, j, v_p, v_q);
						
			CData v_next;//预测值			
			temp_bic = get_bic_value(i, j, v_p, v_q, v_next);//根据当前p,q,参数估计值获取BIC值
			cout<<"temp_bic="<<temp_bic<<endl;
			
//			getchar();	
			//记录最小的BIC值及p,q值
			if ( min_bic - temp_bic >= epsinon )
			{
				min_bic = temp_bic;
				last_p = i;
				last_q = j;
				
                update_forest_data(v_next);		
			}
			
			//存储所有的p,q组合的BIC值
			S_BIC s_bic_temp;
			s_bic_temp.p = i;
			s_bic_temp.q = j;
			s_bic_temp.bic_value = temp_bic;
						
			m_bic.push_back(s_bic_temp);
			
//			cout<<"p="<<i<<","<<"q="<<j<<","<<"bic_value="<<temp_bic<<endl;
					
		}
		
	}
	
	//记录最优的p,q值
	m_p = last_p;
	m_q = last_q;
	
	cout<<"last_p="<<last_p<<","<<"last_q="<<last_q<<endl;
    
    cout<<"预测结果"<<endl;
    for (i=0; i<m_data_size; i++)
	{
	    cout<<i<<":"<<m_his_data[i].value<<","<<m_forecast_data[i].value<<endl;
	}
	
		
	return SUCC;

}


int CARMA::parameter_estimation(int p, int q, CData &v_p, CData &v_q)
{
	if ( p >= m_data_size || q >= m_data_size)
	{
		return FAIL;
	}
	
	int i,j,k;
	
	//数据均值
//	DataType mean_value = 0.00;
	m_data_mean = get_mean_data();
	
	cout<<"数据均值：m_data_mean="<<m_data_mean<<endl;
	
	cout<<"p="<<p<<","<<"q="<<q<<endl;
	
//	CDataSet m_mean_data;
	
	m_mean_data.assign(m_his_data.begin(), m_his_data.end());//复制历史数据
	
	//数据居中处理	
	for (i=0; i<m_data_size; i++)
	{
		m_mean_data[i].value = m_mean_data[i].value - m_data_mean;
//		cout<<i<<":"<<m_mean_data[i].value<<","<<m_mean_data[i].time<<","<<m_mean_data[i].flag<<endl;
	}
	

    //构造AR参数估计相关矩阵
	DataType **Y_matrix, **X_matrix, **para_matrix_p;
	DataType **X_matrix_trans, **matrix_temp1, **matrix_temp2, **matrix_temp3;
	
	int p_matrix_size = m_data_size - p;
	
	init_matrix(&Y_matrix, p_matrix_size, 1);         //Y矩阵[n-p,1]，n=m_data_size	
	init_matrix(&X_matrix, p_matrix_size, p);         //X矩阵[n-p,p]
	init_matrix(&para_matrix_p, p, 1);                //AR系数矩阵[p,1]
	init_matrix(&X_matrix_trans, p, p_matrix_size);   //X矩阵转置[p, n-p]
	init_matrix(&matrix_temp1, p, p);                 //X_T*X
	init_matrix(&matrix_temp2, p, p);                 //(X_T*X)^
	init_matrix(&matrix_temp3, p, p_matrix_size);     //(X_T*X)^*X_T
	
//	cout<<"p_matrix_size="<<p_matrix_size<<endl;
//	cout<<"p="<<p<<endl;
//	cout<<"q="<<q<<endl;
	
	//Y矩阵赋值
	for (i=0; i<p_matrix_size; i++)
	{
		Y_matrix[i][0] = m_mean_data[i+p].value;
	}
	
//	cout<<"Y_matrix:"<<endl;
//	print_matrix(Y_matrix, p_matrix_size, 1);

		
	//X矩阵赋值
	for (i=0; i<p_matrix_size; i++)
	{
		for (j=0; j<p; j++)
		{
			X_matrix[i][j] = m_mean_data[i+p-1-j].value;
		}	
	}

//	cout<<"X_matrix:"<<endl;
//	print_matrix(X_matrix, p_matrix_size, p);
	
	
	

	//最小二乘矩阵求解AR参数
	trans_matrix(X_matrix, &X_matrix_trans, p_matrix_size, p);                                    //X_T

//	cout<<"X_matrix_trans:"<<endl;
//	print_matrix(X_matrix_trans, p, p_matrix_size);
//    
	
	multiply_matrix(X_matrix_trans, p, p_matrix_size, X_matrix, p_matrix_size, p, &matrix_temp1); //X_T*X	
	
//	cout<<"matrix_temp1:"<<endl;
//	print_matrix(matrix_temp1, p, p);
     
	
	inverse_matirx(matrix_temp1, &matrix_temp2, p, p);                                            //(X_T*X)^
	
//  cout<<"matrix_temp2:"<<endl;
//	print_matrix(matrix_temp2, p, p);
    
 
	
	multiply_matrix(matrix_temp2, p, p, X_matrix_trans, p, p_matrix_size, &matrix_temp3);         //(X_T*X)^ * X_T	
	
//	cout<<"matrix_temp3:"<<endl;
//	print_matrix(matrix_temp3, p, p_matrix_size);
    
 	
	multiply_matrix(matrix_temp3, p, p_matrix_size, Y_matrix, p_matrix_size, 1, &para_matrix_p);  //(X_T*X)^ * X_T * Y
	
	cout<<"para_matrix_p:"<<endl;
	print_matrix(para_matrix_p, p, 1);   
   
	
	//构造MA参数估计相关矩阵
	DataType **YY_matrix, **E_matrix, **para_matrix_q;
	DataType **E_matrix_trans, **q_matrix_temp1, **q_matrix_temp2, **q_matrix_temp3;

	int max_pq = get_max_pq(p, q);
	int q_matrix_size = m_data_size - max_pq;
	
	init_matrix(&YY_matrix, q_matrix_size, 1);      //YY矩阵[n-q,1],n=m_data_size-max_pq  
	init_matrix(&E_matrix, q_matrix_size, q);       //X矩阵[n-q,q]
	init_matrix(&para_matrix_q, q, 1);              //MA系数矩阵[q,1]
	init_matrix(&E_matrix_trans, q, q_matrix_size); //E矩阵转置[p, n-p]
	init_matrix(&q_matrix_temp1, q, q);             //E_T*E
	init_matrix(&q_matrix_temp2, q, q);             //(E_T*E)^
	init_matrix(&q_matrix_temp3, q, q_matrix_size); //(E_T*E)^*E_T
	
	//YY矩阵赋值
	for (i=0; i<q_matrix_size; i++)
	{
		YY_matrix[i][0] = m_mean_data[i+max_pq].value;
	}
	
	//E矩阵赋值	
	for (i=0; i<q_matrix_size; i++)
	{
		for (j=0; j<q; j++)
		{
			
			DataType temp = 0.0;//求E矩阵当前值对应的X的AR预测值
						
			int index = i+max_pq-1-j;//对应索引
			
			if (index < p)
			{			    			    
			    if (index==0)
			    {
			        temp = m_mean_data[0].value;//无法计算eps，直接利用数据值
			    }
			    else
			    {
    			    //利用部分值预测
    			    for (k=0; k<index; k++)
        			{
        				temp += para_matrix_p[k][0]*m_mean_data[i+max_pq-1-j-k-1].value;
        			}
			    }
		
			}
			else
			{
    			//利用完整值预测
    			for (k=0; k<p; k++)
    			{
    				temp += para_matrix_p[k][0]*m_mean_data[i+max_pq-1-j-k-1].value;
    			}
    			

     
			}
			
//			cout<<"temp="<<temp<<endl;
    			
    	    E_matrix[i][j] = m_mean_data[i+max_pq-1-j].value - temp;
													
		}
		
	}
	
	
//	cout<<"YY_matrix:"<<endl;
//	print_matrix(YY_matrix, q_matrix_size, 1);
	
//	cout<<"E_matrix:"<<endl;
//	print_matrix(E_matrix, q_matrix_size, q);
	

		
	//最小二乘矩阵求解MA参数
	trans_matrix(E_matrix, &E_matrix_trans, q_matrix_size, q);                                        //E_T
	multiply_matrix(E_matrix_trans, q, q_matrix_size, E_matrix, q_matrix_size, q, &q_matrix_temp1);   //E_T*E
	inverse_matirx(q_matrix_temp1, &q_matrix_temp2, q, q);                                            //(E_T*E)^
	multiply_matrix(q_matrix_temp2, q, q, E_matrix_trans, q, q_matrix_size, &q_matrix_temp3);         //(E_T*E)^ * E_T
	multiply_matrix(q_matrix_temp3, q, q_matrix_size, YY_matrix, q_matrix_size, 1, &para_matrix_q);   //(E_T*E)^ * E_T * YY
	
	
	cout<<"para_matrix_q:"<<endl;
	print_matrix(para_matrix_q, q, 1);

	
	for (i=0; i<p; i++)
	{
		v_p.push_back(para_matrix_p[i][0]);
	}
	
	for (i=0; i<q; i++)
	{
		v_q.push_back(para_matrix_q[i][0]);
	}
	

	//释放矩阵空间	
	free_matrix(Y_matrix, p_matrix_size, 1);
	free_matrix(X_matrix, p_matrix_size, p);
	free_matrix(para_matrix_p, p, 1);	
	free_matrix(X_matrix_trans, p, p_matrix_size);
	free_matrix(matrix_temp1, p, p);
	free_matrix(matrix_temp2, p, p);
	free_matrix(matrix_temp3, p, p_matrix_size);
	free_matrix(YY_matrix, q_matrix_size, 1);
	free_matrix(E_matrix, q_matrix_size, q);
	free_matrix(para_matrix_q, q, 1);	
	free_matrix(E_matrix_trans, q, q_matrix_size);
	free_matrix(q_matrix_temp1, q, q);
	free_matrix(q_matrix_temp2, q, q);
	free_matrix(q_matrix_temp3, q, q_matrix_size);

	
	return SUCC;
}

DataType CARMA::get_mean_data()
{
	int i;
	
	DataType mean_data = 0.00;	
	
	for (i=0; i<m_data_size; i++)
	{			
		mean_data = mean_data + m_his_data[i].value;			
	}
		
	mean_data = (DataType)mean_data/(DataType)m_data_size;

	return mean_data;
}


DataType CARMA::get_bic_value(int p, int q, CData &v_p, CData &v_q, CData &v_next)
{
	int i,j,k;
	
	DataType bic_value = 0.00;	
	DataType delta = 0.00;	 
	DataType temp_one = 0.00; 
	DataType error_ret = 10000.00;	
	DataType sum_t = 0.00;
	DataType eps = 0.00;
	
	CData v_next_temp;
	
	int temp_two = m_data_size - p - (p + q);
	
	CData v_eps;//预测误差值 X^-x
	
//	cout<<"m_data_size="<<m_data_size<<","<<"p="<<p<<","<<"q="<<q<<endl;

    cout<<"v_p.size="<<v_p.size()<<","<<"v_q.size="<<v_q.size()<<endl;
	
	for (i=0; i<=m_data_size; i++)
	{		
        DataType temp = 0.00;
        
        if (i < p)
        {			    			    
            if (i == 0)
            {
                temp = m_mean_data[0].value;//利用数据值作为预测值
            }
            else
            {
        	    //利用部分值预测
        	    for (j=0; j<i; j++)
        		{
        			temp += v_p[j]*m_mean_data[i-1-j].value;
        		}
            }
        
        }
        else if (i<m_data_size)
        {
        	//利用完整值预测
        	for (j=0; j<p; j++)
        	{
        		temp += v_p[j]*m_mean_data[i-1-j].value;
        	}
        	              
        }
        else
        {
            //利用完整值预测
        	for (j=0; j<p; j++)
        	{
        		temp += v_p[j]*m_mean_data[i-1-j].value;
        	}
        }
				
		v_next_temp.push_back(temp);
		
		temp = m_mean_data[i].value - temp;
		
		v_eps.push_back(temp);
							
	}
	
	
	for (i=0; i<m_data_size; i++)
	{
		DataType temp_sum_p = 0.00;
		DataType temp_sum_q = 0.00;
		
		temp_sum_p = v_next_temp[i];
		
		if (i < q)
		{
			if (i == 0)
			{
			    temp_sum_q = v_eps[0];
			}
			else
			{
			    //利用部分值预测
        	    for (j=0; j<i; j++)
        		{
        			temp_sum_q += v_q[j]*v_eps[i-1-j];
        		}
			}
		
		}
		else
		{
			for (j=0; j<q; j++)
			{
				temp_sum_q += v_q[j]*(v_eps[i-1-j]);
			}
		}
		
		v_next.push_back(temp_sum_p - temp_sum_q + m_data_mean);//最终的预测值
				
		sum_t += pow((temp_sum_p - temp_sum_q - (m_mean_data[i].value)),2);
						
	}
	
    cout<<"sum_t:"<<sum_t<<endl;
    
    if (temp_two <= 0)
	{
	    cout<<"temp_two="<<temp_two<<endl;
	    return 10000;
	}

	delta = (DataType)sum_t/(DataType)temp_two;
		
	cout<<"delta:"<<delta<<endl;
	
	bic_value = m_data_size*log(delta) + (p+q)*log(m_data_size);
	
	cout<<"bic_value:"<<bic_value<<endl;
	
	
	//预测未来值
	DataType sum_p = 0;
	DataType sum_q = 0;
    for (j=0; j<p; j++)
	{
		sum_p += v_p[j]*m_mean_data[m_data_size-1-j].value;
	}
	
	for (j=0; j<q; j++)
	{
		sum_q += v_q[j]*(v_eps[m_data_size-1-j]);
	}
	
	m_next = sum_p + sum_q + m_data_mean;
	
	cout<<"11111111111111111-----------m_next="<<m_next<<endl;
	
	return bic_value;

}

void CARMA::update_forest_data(CData &v_next)
{   
    int i=0;
	for (i=0; i<m_data_size; i++)
	{
	    m_forecast_data[i].value = v_next[i];
	}
  
}



int CARMA::get_threshold(int k, int A_m, int theta, int t)
{
	int i,j,ii,jj,kk;
	
	int A_m_end = A_m*10;//刻度为1/10
	int theta_end = theta*10;
	
	DataType curr_A_m = 0.1;
	DataType curr_theta = 0.1;
	
	
	DataType integral_value = 0.00;
	DataType epsinon  = 0.001;
	DataType point_k = 0.00;
	
	//首先获取m_curr_data的均值
	DataType temp_mean = 0.00;
	
	for (i=2; i<=k; i++)
	{
		DataType means[i];//记录K个聚类均值
		k_means(i, means);//K均值聚类算法
		
		for (ii=0; ii<i; ii++)
		{
		    temp_mean += means[ii];
		    cout<<"means="<<means[ii]<<endl;
		}
		
		cout<<"after"<<endl;
		
		temp_mean = (DataType)temp_mean/(DataType)i;
		
		cout<<"temp_mean="<<temp_mean<<endl;
		
		for (ii=0; ii<i; ii++)
		{
		    means[ii] = fabs(means[ii] - temp_mean);
		    cout<<"means="<<means[ii]<<endl;
		}
				
		for (ii=0; ii<=A_m_end; ii++)
		{
			curr_A_m =  0.1 + (DataType)ii/(DataType)10;
			
			for (jj=0; jj<=theta_end; jj++)
			{				
				curr_theta = 0.1 + (DataType)jj/(DataType)10;
				
				for (kk=1; kk<t; kk++)
				{								
										
					integral_value = get_integral(curr_A_m, curr_theta, (DataType)0, (DataType)kk);
					
//					cout<<"kk="<<kk<<","<<"integral_value="<<integral_value<<endl;
//					cout<<"kk="<<kk<<"curr_A_m="<<curr_A_m<<","<<"curr_theta="<<curr_theta<<endl;
				
					//判断积分与1的关系，在误差的范围内
					if ( fabs(integral_value - 1) <= epsinon )
					{
						//根据当前的t值获取阈值,k个聚类将0~t的区间分为k份				
						DataType curr = 0.00;
						DataType before = 0.00;
						DataType temp = 0.00;
						DataType diff = 0.00;
						
						for (j=1; j<=i; j++)
						{
						    //(kk*j)/i  (k-1)*t/k				
							curr = get_integral(curr_A_m, curr_theta, (DataType)0, (DataType)(kk*j)/i);							
																									
							diff = curr - before;
							before = curr;	
							
//							cout<<"j="<<j<<","<<"diff="<<diff<<","<<"X^="<<means[j-1]<<endl;											
													
							temp += diff*means[j-1];
							
							
						}
						
						temp = temp/2;
						
						//将当前计算的d值保存
						m_thresholds.push_back(temp);
						
						cout<<"k="<<i<<","<<"curr_A_m="<<curr_A_m<<","<<"curr_theta="<<curr_theta<<","<<"t="<<kk<<","<<"threshold="<<temp<<endl;
						
//						cout<<"temp="<<temp<<endl;
											
						break;

                     
                        
					}
				
									
				}
				
				
			}
			
		}
					
	}
	
	cout<<"生成的动态阈值个数为:"<<m_thresholds.size()<<endl;


	return SUCC;

}


int CARMA::getcurrdata(string time)
{
    int i=0;
    int data_size = 0;
    
    for (i=0; i<m_data_size; i++)
    {
        //具体时间格式需要确定
        if (time.compare(10, 8, m_his_data[i].time, 10, 8) == 0)
        {
            if (m_his_data[i].flag == 0)
            {
                m_curr_data.push_back(m_his_data[i]);
                data_size++;
            }

        }
        
    }
    
    
    return data_size;
}

DataType CARMA::get_forecast_rate(DataType forecast_value, DataType threshold, CDataSet& data)
{    
    DataType min_value = 0.00;
    DataType max_value = 0.00;
    DataType epsinon = 0.0001;
    DataType rate = 0.0000;
    
    int num_a, num_b, num_c, num_d; 
    
    num_a = num_b = num_c = num_d = 0;
    
    min_value = forecast_value - threshold;
    max_value = forecast_value + threshold;
    
    int i;
    int data_size = data.size();
    for (i=0; i<data_size; i++)
    {
        if ( (data[i].value-min_value >= epsinon) && (max_value-data[i].value >= epsinon) )
        {
            if (data[i].flag == 0)
            {
                num_a++;
            }
            else
            {
                num_b++;
            }
        }
        else
        {
            if (data[i].flag == 0)
            {
                num_c++;
            }
            else
            {
                num_d++;
            }
        }
                               
    }
    
    rate = (DataType)(num_a+num_d)/(DataType)(num_a+num_b+num_c+num_d);
    
//    cout<<"num_a,num_b,num_c,num_d分别为："<<num_a<<","<<num_b<<","<<num_c<<","<<num_d<<endl;
    
    return rate;
    
}



DataType CARMA::get_integral(DataType A_m, DataType theta, DataType a, DataType b)
{
	int i, j;
	
	DataType value;	
	DataType s,x,h;
	
	int n=200;//积分和划分区间
	
	h=(DataType)(b-a)/(DataType)n;
		
	s=0;
	for(i=0;i<n;i++)
	{
		x=a+i*h;		
		s += A_m*(exp(-(theta*x)));
	}
	
	s=s*h;
	
	
	return s;
	
}


void CARMA::k_means(int k, DataType means[])
{
	//聚类K值不能大于数据长度
	if (k >= m_data_size)
	{
		return;
	}
	
	DataType epsinon = 0.01;

	
	cout<<"K均值聚类算法:"<<k<<endl;
	
	vector<Data> clusters[k];
	
	//K均值聚类需要从数据中选取K个不同的值作为初始均值
	int i, j, m;
	means[0] = m_curr_data[0].value;
	for(i=1; i<k; i++)
	{		
		DataType temp = 0.00;
	    for (j=1; j<m_data_size; j++)
	    {
	        temp = m_curr_data[j].value;
//	        cout<<"temp="<<temp<<endl;
	        
	        for (m=0; m<i; m++)
	        {
	            if (fabs(temp - means[m]) <= epsinon)//有重复的
	            {
	                break;
	            }
	        }
	        
	        if (m == i)
	        {
	            means[i] = temp;
	            break;
	        }
	    }	
	    
//	    cout<<"means["<<i<<"]"<<means[i]<<endl;

	}
	
	
	int lable=0;
	//聚类
	for(i=0; i<m_curr_data.size(); i++)
	{
		lable=getcluster(k, means, m_curr_data[i]);
		clusters[lable].push_back(m_curr_data[i]);
	}
	
	//输出刚开始的簇
//	for(lable=0;lable<k;lable++)
//	{
//		cout<<"第"<<lable+1<<"个簇："<<endl;
//		vector<Data> t = clusters[lable];
//		for (i = 0; i< t.size(); i++)
//		{
//			cout<<"("<<t[i].value<<","<<t[i].time<<")"<<"   ";
//		}	
//		cout<<endl;
//	}
	
	DataType oldVar=-1;
	DataType newVar=getvar(k,clusters,means);
		
	while(fabs(newVar - oldVar) >= epsinon) //当新旧函数值相差不到epsinon即准则函数值不发生明显变化时，算法终止
	{
		
//		cout<<"排序前："<<endl;
//		for(lable=0;lable<k;lable++)
//		{
//			cout<<"第"<<lable+1<<"个簇："<<endl;
//			vector<Data> t = clusters[lable];
//			for (i = 0; i< t.size(); i++)
//			{
//				cout<<"("<<t[i].value<<","<<t[i].time<<")"<<"   ";
//			}	
//			cout<<endl;
//		}
		//按照时间字段调整各个簇的先后顺序
		sort_clusters(k, clusters);
		
//		cout<<"排序后："<<endl;
//		
		for(lable=0;lable<k;lable++)
		{
			cout<<"第"<<lable+1<<"个簇："<<endl;
			vector<Data> t = clusters[lable];
			for (i = 0; i< t.size(); i++)
			{
				cout<<"("<<t[i].value<<","<<t[i].time<<")"<<"   ";
			}	
			cout<<endl;
		}
		//更新每个簇的中心点
		for (i = 0; i < k; i++) 
		{
			means[i] = getmeans(clusters[i]);
//			cout<<"means["<<i<<"]:"<<means[i]<<endl;
		}
		
		oldVar = newVar;
		newVar = getvar(k, clusters, means); //计算新的准则函数值
		for (i = 0; i < k; i++) 
		{
			clusters[i].clear();
		}
		
//		cout<<"newVar="<<newVar<<endl;
		
		//根据新的质心获得新的簇
//		for(i=0;i!=tuples.size();++i)
		for(i=0; i<m_curr_data.size(); i++)
		{
			lable = getcluster(k, means,m_curr_data[i]);
			clusters[lable].push_back(m_curr_data[i]);
		}
		
		//输出当前的簇
//		for(lable=0;lable<k;lable++)
//		{
//			cout<<"第"<<lable+1<<"个簇："<<endl;
//			vector<Data> t = clusters[lable];
//			for (i = 0; i< t.size(); i++)
//			{
//				cout<<"("<<t[i].value<<","<<t[i].time<<")"<<"   ";
//			}	
//			cout<<endl;
//		}
	}

	
}

//根据质心，决定当前元组属于哪个簇
int CARMA::getcluster(int k, DataType means[],Data tuple)
{
	DataType dist = getdistXY(means[0],tuple);
	DataType tmp;
	DataType eps = 0.0001;
	
//	cout<<"dist="<<dist<<endl;
	
	int label=0;//标示属于哪一个簇
	for(int i=1;i<k;i++)
	{
		tmp = getdistXY(means[i], tuple);
//		cout<<"tmp="<<tmp<<endl;
		if(dist - tmp >= eps) 
		{
			dist = tmp;
			label = i;
		}
	}
	
	return label;	

}
//获得给定簇集的平方误差
DataType CARMA::getvar(int k, vector<Data> clusters[],DataType means[])
{
	DataType var = 0.00;
	for (int i = 0; i < k; i++)
	{
		vector<Data> t = clusters[i];
		for (int j = 0; j< t.size(); j++)
		{
			var += getdistXY(means[i], t[j]);
		}
	}
//	cout<<"var:"<<var<<endl;
	return var;

}

//获得当前簇的均值（质心）
DataType CARMA::getmeans(vector<Data> cluster)
{
	DataType means = 0.00;
	DataType temp;
		
	int num = cluster.size();
	if (num == 0)
	{
	    return means;
	}
	
	for (int i = 0; i < num; i++)
	{
		means += cluster[i].value;
	}
	
	temp = (DataType)means / (DataType)num;


	return temp;

}


DataType CARMA::getdistXY(DataType x, Data y)
{
	
	return fabs(x - y.value);
	
}

//
void CARMA::sort_clusters(int k, vector<Data> clusters[])
{
    int i;
    
    vector<Data> clusters_temp[k];
    
    for (i=0; i<k; i++)
    {
        clusters_temp[i].assign(clusters[i].begin(), clusters[i].end());
    }
    
    string temp_time[k];
    int flag[k];
    
   
    for (i=0; i<k; i++)
    {
        flag[i] = i;
        get_latest_time(clusters_temp[i], temp_time[i]);
    }
    
    //降序
	string temp;
	int flag_temp = 0;
	for (int i = 1; i < k; i++)
	{
		for ( int j = (k-1); j >= i; j--)
		{
			if (temp_time[j-1] < temp_time[j])
			{
				temp = temp_time[j-1];
				temp_time[j-1] = temp_time[j];
				temp_time[j] = temp;
				
				flag_temp = flag[j-1];
				flag[j-1] = flag[j];
				flag[j] = flag_temp;
				
			}
		}
	}
	
	
	for (i = 0; i < k; i++) 
	{
		clusters[i].clear();
	}
	
	for (i = 0; i < k; i++) 
	{
		int index = flag[i];
		clusters[i].assign(clusters_temp[index].begin(), clusters_temp[index].end());
	}
	
    
    
    
    
    
	
}

void CARMA::get_latest_time(vector<Data> &clusters, string &latest_time)
{
    int i;
    int len = clusters.size();
    
    if (len <= 0)
    {
        return;
    }
    
    latest_time = clusters[0].time;
    
    for (i=1; i<len; i++)
    {
//        cout<<"latest_time="<<latest_time<<","<<"clusters_time="<<clusters[i].time<<endl;
        if (clusters[i].time > latest_time)
        {
            latest_time = clusters[i].time;           
        }       
    }
    
//    cout<<"latest_time======="<<latest_time<<endl<<endl;
   
}

void CARMA::print_his_data()
{
	int i=0;
	
//	for (i=0; i<m_data_size; i++)
//	{
//		cout<<i<<":"<<m_his_data[i].value<<","<<m_his_data[i].time<<","<<m_his_data[i].flag<<endl;
//	}
	
	//打印预测数据
//	for (i=0; i<m_data_size; i++)
//	{
//		cout<<i<<":"<<m_his_data[i].value<<","<<m_his_data[i].time<<","<<m_his_data[i].flag<<endl;
//	}
	
}

void CARMA::print_bic_data()
{
	int i=0;
	int bic_num = m_bic.size();
	
	cout<<"BIC output:"<<endl;
	for (i=0; i<bic_num; i++)
	{
		cout<<i<<":"<<m_bic[i].p<<","<<m_bic[i].q<<","<<m_bic[i].bic_value<<endl;
	}
		
}


int CARMA::init_matrix(DataType*** matrix, int row, int column)
{
	int i, j;

	*matrix = (DataType**)malloc(sizeof(DataType*)*row);
	if (NULL == *matrix)
	{
		cout<<"malloc error.."<<endl;
		return -1;
	}
	
	for (i=0; i<row; i++)
	{
		(*matrix)[i] = (DataType*)malloc(sizeof(DataType)*column);
		if (NULL == (*matrix)[i])
		{
			cout<<"malloc error.."<<endl;
			return FAIL;
		}
	}
	
	//赋初值	
	for (i=0; i<row; i++)
	{
		for (j=0; j<column; j++)
		{
			(*matrix)[i][j] = 0.00;
		}
	}

				
	return SUCC;
	
}

void CARMA::free_matrix(DataType **matrix, int row, int column)
{
	int i=0;
	for (i=0; i<row; i++) 
	{
		free(matrix[i]);
		matrix[i]=NULL;
	}
	
	free(matrix);
	matrix=NULL;

}

int CARMA::trans_matrix(DataType** matrix, DataType*** trans_matirx, int row, int column)
{
	int i, j;

	if(matrix == NULL || (*trans_matirx) == NULL || row <= 0 || column <= 0)
	{
	    return FAIL;
	}

	for(i=0; i<row; i++)
	{
		for(j=0; j<column; j++)
		{
			(*trans_matirx)[j][i] = matrix[i][j];
		}
	}
	return SUCC;	
	
}

DataType CARMA::determ_matrix(DataType** matrix, int row, int column)
{	
	int i,j,k,p,r; 
	DataType X, temp = 1, temp1 = 1, s = 0, s1 = 0;
	
	if (row == column && column == 1)
	{
	    X = matrix[0][0];
	    return X;
	}
	
	
	if(column == 2)
	{
		for(i=0;i < row;i++)
			for(j=0;j <column;j++)
				if((i+j)%2) 
					temp1 *= matrix[i][j];
				else 
					temp *= matrix[i][j];
				X = temp - temp1;
	}
	else
	{
		for(k = 0; k < column; k++)
		{

			for(i=0,j=k; i<row && j<column; i++,j++)
			{
			    temp *= matrix[i][j];
			}
				
			if(row - i)
			{
				for(p = row-i, r = row-1; p > 0; p--,r--)
				{
				    temp *= matrix[r][p-1];
				}
					
			}
			s += temp;
			temp = 1;
		}
		for(k = column-1; k >= 0; k--)
		{
			for(i=0, j=k; i<row && j>=0; i++,j--)
				temp1 *= matrix[i][j];
			if(row - i)
			{
				for(p = row-1, r=i; r <row; p--,r++)
					temp1 *= matrix[r][p];
			}
			s1 += temp1;
			temp1 = 1;
		}
		X = s-s1;
	}
	
	return X;
	
}


int CARMA::inverse_matirx(DataType** matrix, DataType*** inverse_matirx, int row, int column)
{
	
	int i, j, x, y, k, l;
	DataType **SP = NULL, **AB = NULL, **TempMatrix = NULL, X;
	
	if(matrix == NULL || *inverse_matirx == NULL || row <= 0 || column != row)
	{
	    return FAIL;
	}
		
	SP = (DataType **)malloc(row*sizeof(DataType*));
	AB = (DataType **)malloc(row*sizeof(DataType*));
	TempMatrix = (DataType **)malloc(row*sizeof(DataType*));
	
	X = determ_matrix(matrix, row, column);
	if(X == 0)
	{
	    return FAIL;
	}
	
		
	X = 1/X;
	
	for(i = 0; i < row; i++) 
	{
		SP[i] = (DataType *)malloc(column*sizeof(DataType));
		AB[i] = (DataType *)malloc(column*sizeof(DataType));
		TempMatrix[i] = (DataType *)malloc(column*sizeof(DataType));
	}

	//求矩阵伴随矩阵
	for(i = 0; i < row; i++)
	{
		for(j = 0; j < column; j++)
		{
			for(k = 0; k < row; k++)
				for(l = 0; l <column; l++)
					TempMatrix[k][l] = matrix[k][l];	//TempMatrix变成matrix;
			{
				for(x = 0; x < column; x++)
					TempMatrix[(i*column+x)/row][(i*column+x)%row] = 0;
				for(y = 0; y < row; y++)
					TempMatrix[y][j]=0;
				TempMatrix[(int)((i*column+j)/row)][(i*column+j)%row] = 1;
				SP[(int)((i*column+j)/row)][(i*column+j)%row] = determ_matrix(TempMatrix, row, column);
				AB[(int)((i*column+j)/row)][(i*column+j)%row] = X * SP[(int)((i*column+j)/row)][(i*column+j)%row];
			}
		}
	}
	
	trans_matrix(AB, inverse_matirx, row, column);

	return SUCC;
	
}

int CARMA::multiply_matrix(DataType** matrix1, int row1, int column1, DataType** matrix2, int row2, int column2, DataType*** matrix_result)
{
	if(column1 != row2)
	{
	    return FAIL;
	}
		
    int i,j,k;
	for(i = 0; i < row1; i++)
	{
		for(j = 0; j < column2; j++)
		{
			for(k = 0; k < column1; k++)
			{
				(*matrix_result)[i][j] += matrix1[i][k]*matrix2[k][j];
			}
		}
	}

	return SUCC;
	
}


void CARMA::print_matrix(DataType** matrix, int row, int column)
{
	int m, n;
	for(m = 0; m < row; m++)
	{
		for(n = 0; n < column; n++)
		{
			cout<<matrix[m][n]<<",";
		}
		cout<<endl;
	}
	
}













