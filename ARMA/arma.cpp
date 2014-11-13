#include <arma.h>


CARMA::CARMA(void)
{
  //默认构造函数
}

CARMA::~CARMA()
{

}

CARMA::CARMA(CDataSet& his_data, int p, int q)
{
	
	m_his_data.assign(his_data.begin(), his_data.end());
	m_data_size = m_his_data.size;
	
	print_his_data();
	
	m_p = p;
	m_q = q;
	
	
	get_model();//获取模型
	
	
}

int CARMA::get_model()
{
	if (m_p < 1 || m_q < 1)
	{
		return -1;
	}
	
	int i,j;
	
	int last_p = 0;
	int last_q = 0;
	
	DataType min_bic  = 0.00000;
	DataType temp_bic = 0.00000;
	DataType epsinon  = 0.00001;
	
	//穷举法利用p,q进行参数估计
	for (i=1; i<m_p, i++)
	{
		for (j=1; j<m_q, j++)
		{
			vector<DataType> v_p;
			vector<DataType> v_q;
					
			parameter_estimation(i, j, &v_p, &v_q);//参数估计
			
			temp_bic = get_bic_value(i, j, &v_p, &v_q);//根据当前p,q,参数估计值获取BIC值
			
			//存储所有的p,q组合的BIC值
			S_BIC s_bic_temp;
			s_bic_temp.p = i;
			s_bic_temp.q = j;
			s_bic_temp.bic_value = temp_bic;
			
			//记录最小的BIC值及p,q值
			if ( (min_bic - temp_bic > epsinon) && (min_bic - temp_bic < -epsinon) )
			{
				min_bic = temp_bic;
				last_p = i;
				last_q = j;
			}
			
			
			m_bic.push_back(s_bic_temp);
					
		}
		
	}
	
		
	return 0;

}


int CARMA::parameter_estimation(int p, int q, CData &v_p, CData &v_q)
{
	int i,j,k;
	
	if ( p >= m_data_size || q >= m_data_size)
	{
		return -1;
	}
	
	//数据均值
	double mean_data = 0.00;
	mean_data = get_mean_data();
	
	CDataSet mean_data;
	
	mean_data.assign(m_his_data.begin(), m_his_data.end());//复制数据
	
	for (i=0; i<m_data_size; i++)
	{
		mean_data[i].value = mean_data[i].value - mean_data;//数据居中处理	
	}
	
		
	//Y矩阵[n-p,1], X矩阵[n-p,p], 系数矩阵[p,1], n=m_data_size
	DataType **Y_matrix, **X_matrix, **para_matrix_p;
	DataType **X_matrix_trans, **matrix_temp1, **matrix_temp2, **matrix_temp3;
	

	InitMatrix(Y_matrix, m_data_size-p, 1);
	InitMatrix(X_matrix, m_data_size-p, p);
	InitMatrix(para_matrix_p, p, 1);
	
	InitMatrix(X_matrix_trans, p, m_data_size-p);//X矩阵转置[p, n-p]
	InitMatrix(matrix_temp1, p, p);//X_T*X
	InitMatrix(matrix_temp2, p, p);//(X_T*X)^
	InitMatrix(matrix_temp3, p, m_data_size-p);//(X_T*X)^*X_T
	
	
	//获得p,q的最大值
	int max_pq = 0;
	max_pq = get_max_pq(p, q);
	
	//YY矩阵[n-q,1], X矩阵[n-q,q], 系数矩阵[q,1], n=m_data_size
	DataType **YY_matrix, **E_matrix, **para_matrix_q;
	
	DataType **E_matrix_trans, **q_matrix_temp1, **q_matrix_temp2, **q_matrix_temp3;

	InitMatrix(YY_matrix, m_data_size-max_pq, 1);
	InitMatrix(E_matrix, m_data_size-max_pq, q);
	InitMatrix(para_matrix_q, q, 1);
	
	InitMatrix(E_matrix_trans, q, m_data_size-max_pq);//E矩阵转置[p, n-p]
	InitMatrix(q_matrix_temp1, q, q);//E_T*E
	InitMatrix(q_matrix_temp2, q, q);//(E_T*E)^
	InitMatrix(q_matrix_temp3, q, m_data_size-max_pq);//(E_T*E)^*E_T
	
	
	int n_end = 0;
	n_end = m_data_size-p;
	
	//Y矩阵赋值
	for (i=0; i<n_end; i++)
	{
		Y_matrix[i][0] = mean_data[i+p].value;
	}
	
	//X矩阵赋值
	for (i=0; i<n_end; i++)
	{
		for (j=0; j<p; j++)
		{
			X_matrix[i][j] = mean_data[i+p-1-j].value;
		}	
	}
	
	//打印矩阵的值
	cout<<"Y_matrix:"<<endl;
	printMatrix(Y_matrix, m_data_size-p, 1);
	
	cout<<"X_matrix:"<<endl;
	printMatrix(X_matrix, m_data_size-p, p);
	
	
	//求X矩阵转置
	TranspositionMatrix(X_matrix, X_matrix_trans, m_data_size-p, p);//矩阵转置
	
	//矩阵乘积  X_T*X
	MultiplyMatrix(X_matrix_trans, p, m_data_size-p, X_matrix, m_data_size-p, p, matrix_temp1);
	
	
	//求逆矩阵 (X_T*X)^
	InverseMatirx(matrix_temp1, matrix_temp2, p, p);
	
	//矩阵乘积 (X_T*X)^ * X_T
	MultiplyMatrix(matrix_temp2, p, p, X_matrix_trans, p, m_data_size-p, matrix_temp3);

	
	//矩阵乘积 (X_T*X)^ * X_T * Y
	MultiplyMatrix(matrix_temp3, p, m_data_size-p, Y_matrix, m_data_size-p, 1, para_matrix_p);
	
	
	cout<<"para_matrix_p:"<<endl;
	printMatrix(para_matrix_p, p, 1);



	n_end = m_data_size-max_pq;
	//YY矩阵赋值
	for (i=0; i<n_end; i++)
	{
		YY_matrix[i][0] = mean_data[i+max_pq].value;
	}
	
	double temp = 0.0;
	//E矩阵赋值
	for (i=0; i<n_end; i++)
	{
		for (j=0; j<q; j++)
		{
			//求E矩阵当前值对应的X的预测值
			for (k=0; k<p; k++)
			{
				temp += para_matrix_p[k][0]*mean_data[i+max_pq-1-j-k-1].value;
			}
			
			E_matrix[i][j] = mean_data[i+max_pq-1-j].value - temp;
						
		}
	}
	
	//打印矩阵的值
	cout<<"YY_matrix:"<<endl;
	printMatrix(YY_matrix, m_data_size-max_pq, 1);
	
	cout<<"E_matrix:"<<endl;
	printMatrix(E_matrix, m_data_size-max_pq, q);
	
		
	//求E矩阵转置
	TranspositionMatrix(E_matrix, E_matrix_trans, m_data_size-max_pq, q);//矩阵转置
	
	//矩阵乘积  X_T*X
	MultiplyMatrix(E_matrix_trans, q, m_data_size-max_pq, E_matrix, m_data_size-max_pq, q, q_matrix_temp1);
	
	
	//求逆矩阵 (X_T*X)^
	InverseMatirx(q_matrix_temp1, q_matrix_temp2, q, q);
	
	//矩阵乘积 (X_T*X)^ * X_T
	MultiplyMatrix(q_matrix_temp2, q, q, E_matrix_trans, q, m_data_size-max_pq, q_matrix_temp3);

	
	//矩阵乘积 (X_T*X)^ * X_T * Y
	MultiplyMatrix(q_matrix_temp3, q, m_data_size-max_pq, YY_matrix, m_data_size-max_pq, 1, para_matrix_q);
	
	
	cout<<"para_matrix_p:"<<endl;
	printMatrix(para_matrix_q, q, 1);

	
	//AR参数传递
	for (i=0; i<p; i++)
	{
		v_p.push_back(para_matrix_p[i][0]);
	}
	
	//MA参数传递
	for (i=0; i<q; i++)
	{
		v_q.push_back(para_matrix_q[i][0]);
	}
	
	//释放矩阵空间	
	FreeMatrix(Y_matrix, m_data_size-p, 1);
	FreeMatrix(X_matrix, m_data_size-p, p);
	FreeMatrix(para_matrix_p, p, 1);	
	FreeMatrix(X_matrix_trans, p, m_data_size-p);
	FreeMatrix(matrix_temp1, p, p);
	FreeMatrix(matrix_temp2, p, p);
	FreeMatrix(matrix_temp3, p, m_data_size-p);
	FreeMatrix(YY_matrix, m_data_size-max_pq, 1);
	FreeMatrix(E_matrix, m_data_size-max_pq, q);
	FreeMatrix(para_matrix_q, q, 1);	
	FreeMatrix(E_matrix_trans, q, m_data_size-max_pq);
	FreeMatrix(q_matrix_temp1, q, q);
	FreeMatrix(q_matrix_temp2, q, q);
	FreeMatrix(q_matrix_temp3, q, m_data_size-max_pq);

	
	return 0;
}

DataType CARMA::get_mean_data()
{
	int i;

	DataType sum_data = 0.00;	
	DataType mean_data = 0.00;	

	
	for (i=0; i<m_data_size; i++)
	{			
		sum_data = sum_data + m_his_data[i].value;			
	}
		
	mean_data = (double)sum_data/(double)m_data_size;

	return mean_data;
}


DataType CARMA::get_bic_value(int p, int q, CData &v_p, CData &v_q)
{
	int i,j,k;
	
	DataType bic_value = 0.00;	
	DataType delta = 0.00;	
	DataType temp = 0.00; 
	DataType temp_one = 0.00; 
	int temp_two = 0;
	
	DataType sum_t = 0.00;
	DataType eps = 0.00;
	
	temp_two = m_data_size - p - (p + q);
	
	CData v_next;//预测值
	CData v_eps;//预测误差
	
	
	for (i=0; i<m_data_size; i++)
	{
		if (i < p)
		{
			v_next.push_back(m_his_data[i].value);
			v_eps.push_back(0.00);
			continue;
		}
		
		DataType temp = 0.00;
		
		for (j=0; j<p; j++)
		{
			temp = temp + v_p[j]*(m_his_data[i-1-j].value);//AR
		}
		
		v_next.push_back(temp);
		
		temp = m_his_data[i].value - temp;
		
		v_eps.push_back(temp);
							
	}
	
	
	for (i=0; i<m_data_size; i++)
	{
		DataType temp_sum_p = 0.00;
		DataType temp_sum_q = 0.00;
		
		temp_sum_p = v_next[i];
		
		if (i < q)
		{
			temp_sum_q = v_eps[i];
		}
		else
		{
			for (j=0; j<q; j++)
			{
				temp_sum_q = temp_sum_q + v_q[j]*(v_eps[i-1-j]);
			}
		}
				
		sum_t = sum_t + pow((temp_sum_p + temp_sum_q - (m_his_data[i].value)),2);
						
	}
	
	
	delta = (double)sum_t/(double)temp_two;
	
	bic_value = m_data_size*log(delta) + (p+q)*log(m_data_size);
	
	
	
	return bic_value;

}

void CARMA::get_result()
{

	
}



void CARMA::get_threshold(int k, int A_m, int theta, int t)
{
	int i,j,ii,jj,kk;
	int curve_value;
	
	DataType integral_value;
	DataType epsinon  = 0.01;
	
	DataType point_k = 0.00;
	
	for (i=1; i<k; i++)
	{
		DataType means[i];//记录K个簇均值
		k_means(i, means);//K均值聚类算法
				
		for (ii=0; ii<A_m; ii++)
		{
			for (jj=0; jj<theta; jj++)
			{
				for (kk=0; kk<t; kk++)
				{
					integral_value = get_integral(ii, jj, (DataType)0, (DataType)kk);
					
					//判断积分与1的关系，在误差的范围内
					if ( (integral_value - 1 < epsinon) && (integral_value - 1 > -epsinon) )
					{
						//根据当前的t值获取阈值,k个聚类将0~t的区间分为k份
						
						DataType curr = 0.00;
						DataType before = 0.00;
						DataType temp = 0.00;
						DataType diff = 0.00;
						
						for (j=1; j<=i; j++)
						{							
							curr = get_integral(ii, jj, (DataType)0, (DataType)(t*j)/i);							
																									
							diff = curr - before;
							before = curr;													
														
							temp += diff*means[j-1];
						}
						
						//将当前计算的d值保存
						m_thresholds.push_back(temp);
						
						
						break;
					}
									
				}
			}
		}
					
	}



}

int CARMA::forget_curve_division(int k, int A_m, int theta, int t)
{
	int i, j, kk;
	int t_result = 0;
	
	DataType integral_value;
	DataType epsinon  = 0.01;
	
	for (i=0; i<A_m; i++)
	{
		for (j=0; j<theta; j++)
		{
			for (kk=0; kk<t; kk++)
			{
				integral_value = get_integral(i, j, (DataType)0, (DataType)t);
				
				//判断积分与1的关系，在误差的范围内
				if ( (integral_value - 1 < epsinon) && (integral_value - 1 > -epsinon) )
				{
					//根据当前的t值获取阈值
					
					
					break;
				}
								
			}
		}
	}

	
}

DataType CARMA::get_integral(int A_m, int theta, DataType a, DataType b)
{
	int i, j;
	
	DataType value;
	
	double s,x,h;
	
	int n=100;//积分和划分区间
	
	h=fab(b-a)/n;
	
	s=0;
	for(i=1;i<=n;i++)
	{
		x=a+i*h;
		s=s + A_m*(exp(-(theta*x)));	
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
	
	vector<Data> clusters[k];
//	Data means[k];
	
	int i=0;
	//默认一开始将前K个元组的值作为k个簇的质心（均值）
	for(i=0; i<k; i++)
	{
		means[i]=m_his_data[i].value;
	}
	
	int lable=0;
	//根据默认的质心给簇赋值
	for(i=0; i<m_data_size; i++)
	{
		lable=clusterOfTuple(k, means, m_his_data[i]);
		clusters[lable].push_back(m_his_data[i]);
	}
	
	//输出刚开始的簇
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
	
	DataType oldVar=-1;
	DataType newVar=getVar(k,clusters,means);
	
	DataType epsinon = 1.00;
	
	while(abs(newVar - oldVar) >= epsinon) //当新旧函数值相差不到epsinon即准则函数值不发生明显变化时，算法终止
	{
		
		for (i = 0; i < k; i++) //更新每个簇的中心点
		{
			means[i] = getMeans(clusters[i]);
			//cout<<"means["<<i<<"]:"<<means[i].attr1<<"  "<<means[i].attr2<<endl;
		}
		
		oldVar = newVar;
		newVar = getVar(k, clusters, means); //计算新的准则函数值
		for (i = 0; i < k; i++) //清空每个簇
		{
			clusters[i].clear();
		}
		
		//根据新的质心获得新的簇
//		for(i=0;i!=tuples.size();++i)
		for(i=0; i<m_data_size; i++)
		{
			lable = clusterOfTuple(k, means,m_his_data[i]);
			clusters[lable].push_back(m_his_data[i]);
		}
		
		//输出当前的簇
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
	}

	
}


//根据质心，决定当前元组属于哪个簇
int CARMA::clusterOfTuple(int k, DataType means[],Data tuple)
{
	DataType dist = getDistXY(means[0],tuple);
	DataType tmp;
	
	int label=0;//标示属于哪一个簇
	for(int i=1;i<k;i++)
	{
		tmp = getDistXY(means[i], tuple);
		if(tmp < dist) 
		{
			dist = tmp;
			label = i;
		}
	}
	
	return label;	

}
//获得给定簇集的平方误差
DataType CARMA::getVar(int k, vector<Data> clusters[],DataType means[])
{
	DataType var = 0;
	for (int i = 0; i < k; i++)
	{
		vector<Data> t = clusters[i];
		for (int j = 0; j< t.size(); j++)
		{
			var += getDistXY(means[i], t[j]);
		}
	}
	//cout<<"sum:"<<sum<<endl;
	return var;

}
//获得当前簇的均值（质心）
DataType CARMA::getMeans(vector<Data> cluster)
{
	
	int num = cluster.size();
	DataType means = 0.00;
	DataType temp;
	
	for (int i = 0; i < num; i++)
	{
		means += cluster[i].value;
	}
	
	temp = (DataType)means / (DataType)num;

	return temp;
	//cout<<"sum:"<<sum<<endl;
	

}


DataType CARMA::getdistXY(DataType x, Data y)
{
	
	return abs(x - y.vlaue);
	
}

void CARMA::print_his_data()
{
	int i=0;
	
	for (i=0; i<m_his_data; i++)
	{
		cout<<i<<":"<<m_his_data[i].value<<"|"<<m_his_data[i].time<<"|"<<m_his_data[i].flag<<endl;
	}
	
}


int CARMA::InitMatrix(double** matrix, int row, int column)
{
	int i, j;

	matrix = (double**)malloc(sizeof(double*)*row);
	if (NULL == matrix)
	{
		cout<<"malloc error.."<<endl;
		return -1;
	}
	
	for (i=0; i<row; i++)
	{
		matrix[i] = (double*)malloc(sizeof(double)*column);
		if (NULL == matrix[i])
		{
			cout<<"malloc error.."<<endl;
			return -1;
		}
	}
	
	//赋初值	
	for (i=0; i<row; i++)
	{
		for (j=0; j<column; j++)
		{
			matrix[i][j]) = 0;
		}
	}

				
	return 0;
	
}

void CARMA::FreeMatrix(double **matrix, int row, int column)
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

int CARMA::TranspositionMatrix(double** matrix, double** transpositionMatirx, int row, int column)
{	
	int i;
	int j;

	if(matrix == NULL || transpositionMatirx == NULL || row <= 0 || column <= 0)
		return FAILURE;

	for(i = 0; i < row; i++)
	{
		for(j = 0; j < column; j++)
		{
			transpositionMatirx[j][i] = matrix[i][j];
		}
	}
	return SUCCESS;	
	
}

double CARMA::DeterminantOfMatrix(double** matrix, int row, int column)
{	
	int i;
	int j;

	if(matrix == NULL || transpositionMatirx == NULL || row <= 0 || column <= 0)
		return FAILURE;

	for(i = 0; i < row; i++)
	{
		for(j = 0; j < column; j++)
		{
			transpositionMatirx[j][i] = matrix[i][j];
			transpositionMatirx[j][i] = matrix[i][j];
			transpositionMatirx[j][i] = matrix[i][j];
		}
	}
	return SUCCESS;	
	
}


int CARMA::InverseMatirx(double** matrix, double** inverseMatirx, int row, int column)
{
		int i, j, x, y, k, l;
	double **SP = NULL, **AB = NULL, **TempMatrix = NULL, X;
	if(matrix == NULL || inverseMatirx == NULL || row <= 0 || column != row)
		return FAILURE;
	
	SP = (double **)malloc(row*sizeof(double*));
	AB = (double **)malloc(row*sizeof(double*));
	TempMatrix = (double **)malloc(row*sizeof(double*));
	X = DeterminantOfMatrix(matrix, row, column);
	if(X == 0)
		return FAILURE;
	X = 1/X;
	
	for(i = 0; i < row; i++) 
	{
		SP[i] = (double *)malloc(column*sizeof(double));
		AB[i] = (double *)malloc(column*sizeof(double));
		TempMatrix[i] = (double *)malloc(column*sizeof(double));
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
				SP[(int)((i*column+j)/row)][(i*column+j)%row] = DeterminantOfMatrix(TempMatrix, row, column);
				AB[(int)((i*column+j)/row)][(i*column+j)%row] = X * SP[(int)((i*column+j)/row)][(i*column+j)%row];
			}
		}
	}
	TranspositionMatrix(AB, inverseMatirx, row, column);

	return SUCCESS;
	
}

int CARMA::MultiplyMatrix(double** matrix1, int row1, int column1, double** matrix2, int row2, int column2, double** matrix3)
{
	int i,j,k;
	if(column1 != row2)
		return FAILURE;

	for(i = 0; i < row1; i++)
	{
		for(j = 0; j < column2; j++)
		{
			for(k = 0; k < column1; k++)
			{
				matrix3[i][j] += matrix1[i][k]*matrix2[k][j];
			}
		}
	}

	return SUCCESS;
	
}


void CARMA::printMatrix(double** matrix, int row, int column)
{
	int m, n;
	for(m = 0; m < row; m++)
	{
		for(n = 0; n < column; n++)
		{
			cout<<matrix[m][n];
		}
		cout<<endl;
	}
}













