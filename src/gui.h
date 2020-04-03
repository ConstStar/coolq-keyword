#pragma once
class Gui
{
public:

	Gui();

	Gui(void (*p)());

	//写入配置
	void writeConf();

	//打开主要界面
	void openMain();

private:


private:

	void (*m_writeConf)();
};