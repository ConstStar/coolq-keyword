#pragma once
class Gui
{
public:

	Gui();

	Gui(void (*p)());

	//д������
	void writeConf();

	//����Ҫ����
	void openMain();

private:


private:

	void (*m_writeConf)();
};