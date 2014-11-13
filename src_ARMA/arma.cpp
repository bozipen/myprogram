#include <arma.h>


CARMA::CARMA(void)
{
  //默认构造函数
}

CARMA::CARMA(CDataSet& his_data, int p, int q, Data &next_data)
{
	
	m_p = p;
	m_q = q;
	
}


void CARMA::get_model()
{
	int i,j;
	
	for (i=1; i<m_p, i++)
	{
		for (j=1; j<m_q, j++)
		{
			
		}
	}

}

void CARMA::get_model_parameter()
{

}

CARMA::~CARMA()
{

}












