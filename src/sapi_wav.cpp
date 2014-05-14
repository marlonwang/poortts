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
	//��ʼ�������ӿ�
	ISpVoice *pVoice=NULL;

	//com ��ʼ��
	if(FAILED(::CoInitialize(NULL)))
		return FALSE;

    //��ȡISpVoice�ӿ�
	HRESULT hr=CoCreateInstance(CLSID_SpVoice,NULL,CLSCTX_ALL,
		IID_ISpVoice,(void**)&pVoice);

	if(SUCCEEDED(hr))
	{
		//����������
		CComPtr<ISpStream> cpWavStream;

		//��ʽ��������
		CComPtr<ISpStreamFormat> cpOldStream;
		CSpStreamFormat OriginalFmt;

		pVoice->GetOutputStream(&cpOldStream);
		OriginalFmt.AssignFormat(cpOldStream);
		
		//�ĵ��󶨵������
		hr=SPBindToFile(L"D:\\sapi\\output.wav",SPFM_CREATE_ALWAYS,
			&cpWavStream,&OriginalFmt.FormatId(),
			OriginalFmt.WaveFormatExPtr());

		if(SUCCEEDED(hr))
		{
			//����趨���ĵ�������
			pVoice->SetOutput(cpWavStream,TRUE);

			//�������� XML ��ʽ
			WCHAR WTX[]=L"<VOICE REQUIRED=\"NAME=Ekho Mandarin\"/>The past is gone and static. Nothing we can do will change it. Thefuture is before us and dynamic. Everything we do will affect it. ";
            // �������ţ�����ֹˮ�������޷������ı䣬��ǰ����δ������������������������ÿ���¶���Ӱ������
			//��ʼ�ʶ�
			pVoice->Speak(WTX,SPF_IS_XML,NULL);

			//�ȴ��ʶ�����
			pVoice->WaitUntilDone(INFINITE);
			//cpWavStream->Release();
			
			//��������¶�λ��ԭ������
			pVoice->SetOutput(cpOldStream,FALSE);

			
			//�ͷ�ISpVoice�ӿ�
			pVoice->Release();

			pVoice=NULL;
			printf("����wav�ļ��ɹ�\n");
		}
	}
	::CoUninitialize();
	return TRUE;
}