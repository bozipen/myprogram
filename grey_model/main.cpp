#include "grey_model.h"

//Ԥ��ģ�Ϳɵ�����
const int P=10;
const int Q=10;

//��̬��ֵ����ɵ�����
const int K=20;
const int A_M=2;
const int THETA=2;
const int T=30;

const char FILE_NAME[]="test.dat";

int get_file_data(const char* filename, CDataSet& data);//���ļ���ȡ����

char modelname[64];

int main(int argc, char** argv) 
{	
    
	CDataSet his_data;
	
//	CBICSet bic_set;
	CDataSet forecast_data;
	
	string time("2014-09-12 14:00:00");
	DataType forecast_value = 0.00;
	DataType threshold = 0.00;
	
	if (get_file_data(FILE_NAME, his_data) < 0)
	{
	    cout<<"get_file_data error"<<endl;
	}
	
	//����Ԥ��ģ��
	CGreyModel armaobj(his_data);
	
	//��ȡģ��Ԥ����
//	armaobj.get_forecast_graph(bic_set, forecast_data);	
	
	//��ȡ��̬��ֵ����
//	armaobj.get_forecast_region(K, A_M, THETA, T, time, forecast_value, threshold);
	
	
	//��ӡBIC
//	int i, len;
//	len = bic_set.size();
//	cout<<"BIC result:"<<endl;
//	for (i=0; i<len; i++)
//	{
//		cout<<i<<":"<<bic_set[i].p<<","<<bic_set[i].q<<","<<bic_set[i].bic_value<<endl;
//	}
	
	//��ӡԤ������
	int len = forecast_data.size();
//	cout<<"forecast_data result:"<<endl;
//	for (i=0; i<len; i++)
//	{
//		cout<<i<<":"<<forecast_data[i].value<<","<<forecast_data[i].time<<","<<forecast_data[i].flag<<endl;
//	}
	
	
	cout<<"time="<<time<<endl;
	cout<<"forecast_value="<<forecast_value<<endl;
	cout<<"threshold="<<threshold<<endl;
	

		
	return 0;
}


int get_file_data(const char* filename, CDataSet& data)
{
    fstream fin;
    fin.open(filename,ios::in);
		
    Data atrans;           //����	
    char linebuf[2048];    //�����ݻ�����
    char* p;               //�ַ���ָ�룬ָ����һ���ض��Ŀ�
    char* des;             //ָ��strtok���ص��ַ���
		
    while (!fin.eof())
    {	
        int index = 0;	
    	fin.getline(linebuf, 2048);
    	
//        cout<<"linebuf="<<linebuf<<endl;
    	 
        p = strtok(linebuf, ",");    
    	while(p != NULL)
    	{
            if (index == 0)
            {
                atrans.value = atof(p);
            }
            else if (index == 1)
            {
                string str_temp(p);
                atrans.time=str_temp;  
            }
            else
            {
                atrans.flag = atoi(p);			    
            }
            
            if (index == 2)
            {
                data.push_back(atrans);
                //cout<<atrans.value<<","<<atrans.time<<","<<atrans.flag<<endl;
            }
    		      
    		
    	    index++;
    		
            p = strtok(NULL, ",");
    		
    	}
    		
    }
	
    fin.close();
	
    return 0;

}