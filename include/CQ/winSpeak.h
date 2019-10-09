#pragma once
#include <string>

bool Speak_init();
// 此函数将会调用系统语音合成将wantSpeak合成到 酷Q\data\record\ 目录下,文件名为 file
// 文件名必须以.wav结尾
bool Speak_to_wav_file(std::string file, std::string wantSpeak);