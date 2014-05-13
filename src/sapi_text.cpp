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

#include <stdlib.h>		//char* 转 WCHAR *

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
    CComPtr<IEnumSpObjectTokens> cpEnum; //声音输出接口
    ULONG ulCount = 0;
    ISpVoice * pSpVoice = NULL;		//初始化语音接口
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
    
	//当前时间
	//speakCurrentTime();
    
	pSpVoice->SetVolume(80); //设置音量
    //pSpVoice->Speak(L"It is cloudy today", SPF_DEFAULT, NULL); //加L表示宽字节的 com处理中必须使用宽字符

    pSpVoice->Speak(L"<VOICE REQUIRED=\"NAME=Ekho Cantonese\"/><VOLUME LEVEL=\"100\"><PITCH MIDDLE=\"0\">长江大学</pitch></VOLUME>",SPF_IS_XML,NULL);
 
    /*标志语速
	  <RATE SPEED="-10">text</RATE>
      
	  *音量大小
	  <VOLUME LEVEL="100">TEXT</VOLUME> 0 ~ 100
	  
      *标志音调 -10 ~ 10
      <PITCH MIDDLE="10">text</pitch>

	  *逐个字母读
	  <SPELL>TEXT</SPELL>

	  *强调
	  <EMPH>TEXT</EMPH>

	  *停顿200 ms (最长65536 ms)
	  <SILENCE MSEC="200"/>

	  *控制发音
	  <PRON SYM="h eh -l ow 1"/>

	  *数字日期
	  <context ID="date_ymd">2014.4.15</context>

      *语种
	  <voice required="Language=409"></voice>其中409英语，804汉语，411日语
	*/
	

	
	//vector转数组
	vector<char> vec;
	vector<char> vChar=getFileContent(vec);

	char *str=new char[vChar.size()+1];
	copy(vChar.begin(),vChar.end(),str);
	str[vChar.size()]=0;
    //cout<<str<<endl;	//输出文本内容

	size_t len=strlen(str)+1; //9

    /*转换有问题
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

//获取txt文件内容到vector
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
		while (fin.get(ch))		// 以字符形式读取文本内容
		{
		  vec_char.push_back(ch);
		}
	}
	fin.close();
	/*
	*测试vec_tor内容是否正确
	vector<char>::iterator it=vec_char.begin();
	while(it!=vec_char.end())
	{
		cout<<*it;
		it++;
	}
	*/
	return vec_char;
}

//语音播报当前时间
int speakCurrentTime()
{
	ISpVoice *pVoice=NULL;

	//COM初始化
	if(FAILED(::CoInitialize(NULL))) 
    	return FALSE;
    
	SYSTEMTIME st;
    GetLocalTime(&st);

	//获取ISpVoice接口
    HRESULT hr=CoCreateInstance(CLSID_SpVoice,NULL,CLSCTX_ALL,IID_ISpVoice,(void**)&pVoice);
	if(SUCCEEDED(hr))
	{
       ISpObjectToken * pSpObjectToken = NULL;
       if (SUCCEEDED(SpFindBestToken(SPCAT_VOICES, L"language=804", NULL, &pSpObjectToken)))	//	804 Chinese
	   {
           pVoice->SetVoice(pSpObjectToken);
  
           WCHAR wsz[128];	//宽字符类型
           printf("%.4d-%.02d-%.2d %.2d:%.02d:%.02d\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

           //swprintf
		   //宽字符串格式化命令，主要功能是把格式化的数据写入某个字符串中。
	       swprintf(wsz,
			   L"现在时间是<context ID=\"CHS_Date_ymdhms\">%.4d-年%.02d-月%.2d日 %.2d时:%.02d分:%.02d秒</context>",
			   st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
           pVoice->Speak(wsz, SPF_IS_XML, NULL);
           pVoice->Release();
           pSpObjectToken->Release();
		   pVoice=NULL;
	   }
	}

	//释放
	::CoUninitialize();
	return 0;
}

/*读取文件 乱码

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