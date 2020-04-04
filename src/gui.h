#pragma once
#ifdef _DEBUG
#pragma comment(lib,"nana_v142_Debug_MDd_x86.lib")
#else
#pragma comment(lib,"nana_v142_Release_MD_x86.lib")
#endif // _DEBUG

class Gui
{
public:

	Gui();
	//打开主要界面
	void openMain();

private:

};