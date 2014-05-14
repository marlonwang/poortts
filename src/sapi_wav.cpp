//function: text to speech
//time: 2014-4-14 21:47
//filename:sapi_wav.cpp
//author  :way

//if error s
//C:\Program Files\Microsoft Speech SDK 5.1\Lib\i386

#include <sapi.h>
#include <sphelper.h>
#include <iostream>

#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"sapi.lib")

int main(int argc,char* argv[])
{
	//初始化语音接口
	ISpVoice *pVoice=NULL;

	//com 初始化
	if(FAILED(::CoInitialize(NULL)))
		return FALSE;

    //获取ISpVoice接口
	HRESULT hr=CoCreateInstance(CLSID_SpVoice,NULL,CLSCTX_ALL,
		IID_ISpVoice,(void**)&pVoice);

	if(SUCCEEDED(hr))
	{
		//创建语音流
		CComPtr<ISpStream> cpWavStream;

		//格式化语音流
		CComPtr<ISpStreamFormat> cpOldStream;
		CSpStreamFormat OriginalFmt;

		pVoice->GetOutputStream(&cpOldStream);
		OriginalFmt.AssignFormat(cpOldStream);
		
		//文档绑定到输出流
		hr=SPBindToFile(L"D:\\sapi\\output.wav",SPFM_CREATE_ALWAYS,
			&cpWavStream,&OriginalFmt.FormatId(),
			OriginalFmt.WaveFormatExPtr());

		if(SUCCEEDED(hr))
		{
			//输出设定绑定文档的流上
			pVoice->SetOutput(cpWavStream,TRUE);

			//语音内容 XML 格式
			WCHAR WTX[]=L"<VOICE REQUIRED=\"NAME=Ekho Mandarin\"/>The past is gone and static. Nothing we can do will change it. Thefuture is before us and dynamic. Everything we do will affect it. ";
            // 往昔已逝，静如止水，我们无法再做改变，而前方的未来正生机勃勃，我们所做的每件事都将影响着它
			//开始朗读
			pVoice->Speak(WTX,SPF_IS_XML,NULL);

			//等待朗读结束
			pVoice->WaitUntilDone(INFINITE);
			//cpWavStream->Release();
			
			//把输出重新定位到原来的流
			pVoice->SetOutput(cpOldStream,FALSE);

			
			//释放ISpVoice接口
			pVoice->Release();

			pVoice=NULL;
			printf("生成wav文件成功\n");
		}
	}
	::CoUninitialize();
	return TRUE;
}