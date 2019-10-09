#pragma once
#ifndef sqlitetool
#define sqlitetool

//gbk转UTF-8
//回传的char*自行销毁
char* U(const char* strGbk);

							//UTF-8转gbk  
							//回传的char*自行销毁
char* G(const char* strUtf8);

#endif