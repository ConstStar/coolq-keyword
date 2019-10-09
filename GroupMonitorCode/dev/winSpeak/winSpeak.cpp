#include "..\..\inculde\winSpeak.h"
#include "..\..\inculde\CQLogger.h"

#include "sapi.h"
#include "sphelper.h"
#pragma comment(lib, "sapi.lib")

#include <iostream>

using namespace std;

wchar_t*TW(string file) {
	USES_CONVERSION;
	wchar_t*t = A2W(file.c_str());
	wchar_t*w = new wchar_t[wcslen(t) + 1];
	wcscpy(w, t);
	return w;
}

ISpVoice *pISpVoice;//�����˽ӿ�

bool Speak_init() {
	if (!pISpVoice)
	{
		// ��ҪCOM�ӿ�
		::CoInitialize(NULL);

		// ��ȡISpVoice�ӿ�
		CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&pISpVoice);

		return pISpVoice;
	}
	return true;
}

bool Speak_to_wav_file(string file, string wantSay) {
	static CQ::Logger log("SDK-Speak");
	auto m = file.length() - 4;
	if (
		m < 0 ||
		file[m] != '.' ||
		(file[m + 1] != 'w' && file[m + 1] != 'W') ||
		(file[m + 2] != 'a' && file[m + 2] != 'A') ||
		(file[m + 3] != 'v' && file[m + 3] != 'V')
		) {
		log.Debug("�ļ���������.wav��β");
		return false;
	}
	if (!Speak_init()) { log.Debug("Speak��ʼ��ʧ��"); return false; }

	char pBuf[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, pBuf);

	string s(pBuf);
	s += "\\data\\record\\";
	s += file;
	//log.Debug(string("��Ŀ¼Ϊ:") + pBuf);
	//log.Debug("·��Ϊ:" + s);

	bool res = false;

	auto _file = TW(s.c_str());
	auto _wantSay = TW(wantSay);

	CSpStreamFormat OriginalFmt;//����һ����������󶨵�wav�ļ�
	CComPtr<ISpStream> cpWavStream;//wav��Ƶ��
	CComPtr<ISpStreamFormat> cpOldStream;//�ɵ���

	HRESULT hr = pISpVoice->GetOutputStream(&cpOldStream);
	if (hr == S_OK)
	{
		hr = OriginalFmt.AssignFormat(cpOldStream);

		if (SUCCEEDED(hr))
		{
			// ʹ��sphelper.h���ṩ�ĺ������� wav �ļ�
			hr = SPBindToFile(_file, SPFM_CREATE_ALWAYS, &cpWavStream, &OriginalFmt.FormatId(), OriginalFmt.WaveFormatExPtr());

			if (SUCCEEDED(hr))
			{
				//��������������� wav �ļ��������� speakers
				pISpVoice->SetOutput(cpWavStream, TRUE);

				//��ʼ�ʶ�
				pISpVoice->Speak(_wantSay, SPF_IS_NOT_XML, 0);

				//�������û�ȥ
				pISpVoice->SetOutput(cpOldStream, FALSE);

				cpWavStream.Release();

				res = true;
			}
			else {
				log.Debug("SPBindToFile(...)����ִ��ʧ��,���ܵ�ԭ�����ļ�������ȷ:" + s);
			}
		}
		else {
			log.Debug("OriginalFmt.AssignFormat(cpOldStream)����ִ��ʧ��,���ĵ��Կ��ܲ�֧�ִ˹���.");
		}
	}
	else {
		log.Debug("ISpVoice->GetOutputStream����ִ��ʧ��,���ĵ��Կ��ܲ�֧�ִ˹���.");
	}

	return res;
}

