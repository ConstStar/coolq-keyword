#pragma once
#include <string>
#include <vector>

//�Զ��弯��
template<class T>
class INIvector {
protected:
	std::vector<T> list;//��������
	virtual bool equal(T&, std::string) = 0;
	virtual T New(std::string key) = 0;
public:
	virtual void add(T&);
	//�Ƿ�Ϊ��
	virtual bool empty();

	//�ж�
	virtual int has(std::string);

	//��ȡ
	//֧���ظ��ļ�
	//����֧������ظ��ļ�
	virtual T& get(std::string key, int index);
	//��ȡ
	virtual T & operator[](std::string key);

	//ɾ��
	virtual void del(std::string key, int index);

	//����
	virtual decltype(list.begin()) begin();
	virtual decltype(list.end()) end();
};

#include "iniTemplate.Cpp.h"

//����ע��
class Commentses {
	std::string commentses;//ע��
public:
	Commentses() = default;
	Commentses(std::string);
	Commentses(std::vector<std::string>);

	//����ע��
	virtual void setCommentses(std::string);
	//��ȡע��
	virtual std::string getCommentses();

	//��ȡini��ʽ��ע��
	virtual std::string getCommentsesInIni();
};