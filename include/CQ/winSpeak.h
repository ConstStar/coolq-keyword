#pragma once
#include <string>

bool Speak_init();
// �˺����������ϵͳ�����ϳɽ�wantSpeak�ϳɵ� ��Q\data\record\ Ŀ¼��,�ļ���Ϊ file
// �ļ���������.wav��β
bool Speak_to_wav_file(std::string file, std::string wantSpeak);