#pragma once

#include "iniTemplate.h"
class Ini;
class Sections;
class Parameters;

//���ļ���ȡini
Ini loadForFile(std::string fileName);
//���ַ�����ȡini
Ini loadForString(std::string ini);

//���浽�ļ�
void save2File(Ini);
//���浽�ַ���
std::string save2String(Ini);

//INI
//�εļ���
class Ini :public INIvector <Sections> {
protected:
	virtual bool equal(Sections &, std::string) override;
	virtual Sections New(std::string key) override;
};

//��
//��ֵ�Եļ���
class Sections :public INIvector<Parameters>, public Commentses {
	std::string name;//��
protected:
	virtual bool equal(Parameters &, std::string) override;
	virtual Parameters New(std::string key) override;
public:
	Sections(std::string name);
	Sections(std::string name, std::string Commentses);
	Sections(std::string name, std::vector<std::string> Commentses);
};

//��ֵ��
class Parameters :public Commentses
{
	std::string key; //��
	std::string val; //ֵ

public:
	Parameters(std::string key);
	Parameters(std::string key, std::string val);
	Parameters(std::string key, std::string val, std::string commentses);

	//�����õȺŸ�ֵ
	Parameters&operator=(std::string val);
	//����ֱ�Ӱ�����ֵ��
	operator std::string() { return val; }

	//ȡ��
	std::string&getkey();
	//ȡֵ
	std::string&getval();
};

std::ostream & operator<<(std::ostream &out, Ini &t);
std::ostream & operator<<(std::ostream &out, Sections &t);
std::ostream & operator<<(std::ostream &out, Parameters &t);