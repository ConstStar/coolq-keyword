#pragma once

#include "iniTemplate.h"
class Ini;
class Sections;
class Parameters;

//从文件读取ini
Ini loadForFile(std::string fileName);
//从字符串读取ini
Ini loadForString(std::string ini);

//保存到文件
void save2File(Ini);
//保存到字符串
std::string save2String(Ini);

//INI
//段的集合
class Ini :public INIvector <Sections> {
protected:
	virtual bool equal(Sections &, std::string) override;
	virtual Sections New(std::string key) override;
};

//段
//键值对的集合
class Sections :public INIvector<Parameters>, public Commentses {
	std::string name;//名
protected:
	virtual bool equal(Parameters &, std::string) override;
	virtual Parameters New(std::string key) override;
public:
	Sections(std::string name);
	Sections(std::string name, std::string Commentses);
	Sections(std::string name, std::vector<std::string> Commentses);
};

//键值对
class Parameters :public Commentses
{
	std::string key; //键
	std::string val; //值

public:
	Parameters(std::string key);
	Parameters(std::string key, std::string val);
	Parameters(std::string key, std::string val, std::string commentses);

	//可以用等号赋值
	Parameters&operator=(std::string val);
	//可以直接把它当值用
	operator std::string() { return val; }

	//取键
	std::string&getkey();
	//取值
	std::string&getval();
};

std::ostream & operator<<(std::ostream &out, Ini &t);
std::ostream & operator<<(std::ostream &out, Sections &t);
std::ostream & operator<<(std::ostream &out, Parameters &t);