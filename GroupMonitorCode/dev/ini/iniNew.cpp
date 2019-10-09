#include "iniTemplate.h"
#include "iniNew.h"
using namespace std;

Commentses::Commentses(string)
{
}

Commentses::Commentses(vector<string>)
{
}

void Commentses::setCommentses(string)
{
}

string Commentses::getCommentses()
{
	return string();
}

string Commentses::getCommentsesInIni()
{
	return string();
}

Ini loadForFile(string fileName)
{
	return Ini();
}

Ini loadForString(string ini)
{
	return Ini();
}

void save2File(Ini i)
{
}

string save2String(Ini i)
{
	return string();
}

ostream & operator<<(ostream & out, Ini & t)
{
	// TODO: 在此处插入 return 语句
}

ostream & operator<<(ostream & out, Sections & t)
{
	// TODO: 在此处插入 return 语句
}

ostream & operator<<(ostream & out, Parameters & t)
{
	// TODO: 在此处插入 return 语句
}

bool Ini::equal(Sections &, string)
{
	return false;
}

Sections Ini::New(string key)
{
	return nullptr;
}
Sections::Sections(std::string name)
{
}
Sections::Sections(std::string name, std::vector<std::string> commentses)
	: name(name), Commentses(commentses)
{}
Sections::Sections(string name, std::string commentses)
	: name(name), Commentses(commentses)
{}
bool Sections::equal(Parameters & a, std::string b)
{
	return (a.getval()) == b;
}

Parameters Sections::New(std::string key)
{
	return Parameters(key);
}
Parameters::Parameters(std::string key)
{
}
Parameters::Parameters(std::string key, std::string val)
{
}
Parameters::Parameters(std::string key, std::string val, std::string commentses)
{
}
Parameters & Parameters::operator=(std::string val)
{
	// TODO: 在此处插入 return 语句
}
std::string & Parameters::getkey()
{
	// TODO: 在此处插入 return 语句
}
std::string & Parameters::getval()
{
	return val;
}