#include "arma.h"


#define FILE_NAME "test.dat"

int get_file_data(char *filename, CDataSet& data);//���ļ���ȡ����

char modelname[64];

int main(int argc, char** argv) 
{	
    
	CDataSet his_data;
	
	if (get_file_data(FILE_NAME, his_data) < 0)
	{
	    cout<<"get_file_data error"<<endl;
	}
	
	
	CARMA armaobj(his_data, P, Q);
	
	string time("2014-10-20 12:00:00");
	DataType forecast_value;
	DataType threshold;
	
	armaobj.get_forecast_region(time, forecast_value, threshold);
	
	cout<<"forecast_value="<<forecast_value<<endl;
	cout<<"threshold="<<threshold<<endl;
	

		
	return 0;
}


int get_file_data(char *filename, CDataSet& data)
{
    fstream fin;
	fin.open(filename,ios::in);
		
	Data atrans;          //����	
	char linebuf[2048];    //�����ݻ�����
	char* p;               //�ַ���ָ�룬ָ����һ���ض��Ŀ�
	char* des;			   //ָ��strtok���ص��ַ���
		
	while (!fin.eof())
	{	
	    int index = 0;	
		fin.getline(linebuf, 2048);
		
//		cout<<"linebuf="<<linebuf<<endl;
		 
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
//			   cout<<atrans.value<<","<<atrans.time<<","<<atrans.flag<<endl;
			}
			        
			
			index++;
			
			p = strtok(NULL, ",");
			
		}
			
	}
	
	fin.close();
	
	return 0;

}