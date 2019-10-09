#pragma once
#include <string>
#include <vector>

//自定义集合
template<class T>
class INIvector {
protected:
	std::vector<T> list;//下属集合
	virtual bool equal(T&, std::string) = 0;
	virtual T New(std::string key) = 0;
public:
	virtual void add(T&);
	//是否为空
	virtual bool empty();

	//判断
	virtual int has(std::string);

	//获取
	//支持重复的键
	//但不支持添加重复的键
	virtual T& get(std::string key, int index);
	//获取
	virtual T & operator[](std::string key);

	//删除
	virtual void del(std::string key, int index);

	//遍历
	virtual decltype(list.begin()) begin();
	virtual decltype(list.end()) end();
};

#include "iniTemplate.Cpp.h"

//代表注释
class Commentses {
	std::string commentses;//注释
public:
	Commentses() = default;
	Commentses(std::string);
	Commentses(std::vector<std::string>);

	//设置注释
	virtual void setCommentses(std::string);
	//获取注释
	virtual std::string getCommentses();

	//获取ini格式的注释
	virtual std::string getCommentsesInIni();
};