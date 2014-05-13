//function: text to speech
//time: 2014-4-14 10:33
//filename:sapi_text.cpp
//author  :way

//if ERROR LINK : fatal error LNK1104: cannot open file "sapi.lib"
//C:\Program Files\Microsoft Speech SDK 5.1\Lib\i386

#include <iostream>
#include <sapi.h>
#include <sphelper.h>

#include <fstream>
#include <vector>

#include <stdlib.h>		//char* ת WCHAR *

//#include <atlbase.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

CComModule _Module;

#include <atlcom.h>

using namespace std;

wchar_t* c2w(const char *str);

vector<char> getFileContent(vector<char> &vec_char);
int speakCurrentTime();

int main(int NumOfArguments, char** Argument)
{
    HRESULT hr = S_OK;
    CComPtr<IEnumSpObjectTokens> cpEnum; //��������ӿ�
    ULONG ulCount = 0;
    ISpVoice * pSpVoice = NULL;		//��ʼ�������ӿ�
    CComPtr<ISpObjectToken> cpToken;

    // Initialize COM
    CoInitialize(NULL);

    // Create the voice interface object
    if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, 
        CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice))) {
        cerr << "Fail to create instance of ISpVoice!" << endl;
        return -1;
    }

	//set voice type:
	//Ekho English/Hakka/Mandarin/Cantonese
	//Microsoft Mike(Mary)/Microsoft Simplified Chinese/Microsoft Lili
	
    if (FAILED(SpEnumTokens(SPCAT_VOICES, L"Name=Ekho Mandarin", NULL, &cpEnum))) {
        cerr << "Fail to get voices!" << endl;
        return -1;
    }

    //Get the closest token
    if (FAILED(cpEnum->Next(1, &cpToken, NULL))) {
        cerr << "Fail to get voice EkhoVoice!" << endl;
        return -1;
    }

    //set the voice 
    if (FAILED(pSpVoice->SetVoice( cpToken))) {
        cerr << "Fail to set voice EkhoVoice!" << endl;
        return -1;
    }
    
	//��ǰʱ��
	//speakCurrentTime();
    
	pSpVoice->SetVolume(80); //��������
    //pSpVoice->Speak(L"It is cloudy today", SPF_DEFAULT, NULL); //��L��ʾ���ֽڵ� com�����б���ʹ�ÿ��ַ�

    pSpVoice->Speak(L"<VOICE REQUIRED=\"NAME=Ekho Cantonese\"/><VOLUME LEVEL=\"100\"><PITCH MIDDLE=\"0\">������ѧ</pitch></VOLUME>",SPF_IS_XML,NULL);
 
    /*��־����
	  <RATE SPEED="-10">text</RATE>
      
	  *������С
	  <VOLUME LEVEL="100">TEXT</VOLUME> 0 ~ 100
	  
      *��־���� -10 ~ 10
      <PITCH MIDDLE="10">text</pitch>

	  *�����ĸ��
	  <SPELL>TEXT</SPELL>

	  *ǿ��
	  <EMPH>TEXT</EMPH>

	  *ͣ��200 ms (�65536 ms)
	  <SILENCE MSEC="200"/>

	  *���Ʒ���
	  <PRON SYM="h eh -l ow 1"/>

	  *��������
	  <context ID="date_ymd">2014.4.15</context>

      *����
	  <voice required="Language=409"></voice>����409Ӣ�804���411����
	*/
	

	
	//vectorת����
	vector<char> vec;
	vector<char> vChar=getFileContent(vec);

	char *str=new char[vChar.size()+1];
	copy(vChar.begin(),vChar.end(),str);
	str[vChar.size()]=0;
    //cout<<str<<endl;	//����ı�����

	size_t len=strlen(str)+1; //9

    /*ת��������
	wchar_t *buffer=new wchar_t[2*len+1];
	mbstowcs(buffer,str,len);	//convert char to wchar
	
    wcout<<buffer<<endl;
	buffer=(WCHAR*)malloc(len*sizeof(WCHAR)); //size=4
	swprintf(buffer,L"%s",buffer);
    delete []buffer;
	*/

	wchar_t *buffer=c2w((const char*)str);

	//pSpVoice->Speak((const wchar_t*)buffer, SPF_DEFAULT, NULL);

	delete []str;



    // Shutdown COM
    CoUninitialize ();

    return 0;

}

//��ȡtxt�ļ����ݵ�vector
vector<char> getFileContent(vector<char>& vec_char)
{
	char ch;
    fstream fin;
	fin.open("D:\\mett.txt");
	if(!fin)
	{
		cerr<<"file open failure"<<endl;
		exit(-1);
	}
	else
	{
		while (fin.get(ch))		// ���ַ���ʽ��ȡ�ı�����
		{
		  vec_char.push_back(ch);
		}
	}
	fin.close();
	/*
	*����vec_tor�����Ƿ���ȷ
	vector<char>::iterator it=vec_char.begin();
	while(it!=vec_char.end())
	{
		cout<<*it;
		it++;
	}
	*/
	return vec_char;
}

//����������ǰʱ��
int speakCurrentTime()
{
	ISpVoice *pVoice=NULL;

	//COM��ʼ��
	if(FAILED(::CoInitialize(NULL))) 
    	return FALSE;
    
	SYSTEMTIME st;
    GetLocalTime(&st);

	//��ȡISpVoice�ӿ�
    HRESULT hr=CoCreateInstance(CLSID_SpVoice,NULL,CLSCTX_ALL,IID_ISpVoice,(void**)&pVoice);
	if(SUCCEEDED(hr))
	{
       ISpObjectToken * pSpObjectToken = NULL;
       if (SUCCEEDED(SpFindBestToken(SPCAT_VOICES, L"language=804", NULL, &pSpObjectToken)))	//	804 Chinese
	   {
           pVoice->SetVoice(pSpObjectToken);
  
           WCHAR wsz[128];	//���ַ�����
           printf("%.4d-%.02d-%.2d %.2d:%.02d:%.02d\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

           //swprintf
		   //���ַ�����ʽ�������Ҫ�����ǰѸ�ʽ��������д��ĳ���ַ����С�
	       swprintf(wsz,
			   L"����ʱ����<context ID=\"CHS_Date_ymdhms\">%.4d-��%.02d-��%.2d�� %.2dʱ:%.02d��:%.02d��</context>",
			   st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
           pVoice->Speak(wsz, SPF_IS_XML, NULL);
           pVoice->Release();
           pSpObjectToken->Release();
		   pVoice=NULL;
	   }
	}

	//�ͷ�
	::CoUninitialize();
	return 0;
}

/*��ȡ�ļ� ����

	char arr[1024]={0};  
   
	ifstream infile("D:\\me.txt",ios::in);  
	int count = 0;  
	while(!infile.eof())  
	{  
	  infile>>arr[count];  
	   ++count;  
	}  
	infile.close();  
 
	for ( int j= 0; j < count; ++j)
	  cout<<arr[j]; 
	cout<<endl;
*/

wchar_t* c2w(const char *str)
{
   int length = strlen(str)+1;
   wchar_t *t = (wchar_t*)malloc(sizeof(wchar_t)*length);
   memset(t,0,length*sizeof(wchar_t));
   MultiByteToWideChar(CP_ACP,0,str,strlen(str),t,length);
   return t;
}