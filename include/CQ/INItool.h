#pragma once
#include <string>
#include <vector>

namespace INI {
	class Sections;
	class INItool;

	//ע�ͼ���
	class Comments :public std::vector<std::string>
	{
		friend std::ostream & operator<<(std::ostream &out, Comments &t);
	};
	std::ostream & operator<< (std::ostream &out, Comments &t);

	//��ֵ��
	class Parameters
	{
		std::string key; std::string val; Comments commentses; bool noshow;

		friend std::ostream & operator<<(std::ostream &out, Parameters &t);
		friend Parameters & operator>>(Parameters &t, std::string s);
		friend Sections;

	public:
		Parameters(std::string key, std::string val, Comments commentses);
		Parameters(std::string key);

		Parameters&operator=(std::string val);
		operator std::string() const
		{
			return val;
		}
	};
	std::ostream & operator<<(std::ostream &out, Parameters &t);
	Parameters & operator>>(Parameters &t, std::string s);

	//��ֵ�Լ���(���� ��)
	class Sections :
		//        public //���빫��...
		private
		std::vector<Parameters>
	{
		
		std::string name;
		Comments commentses;
		friend std::ostream & operator<<(std::ostream &out, Sections &t);
		friend INItool;

	public:
		Sections() = default;
		Sections(std::string name);
		Sections(std::string name, Comments commentses);

		std::string getName()const { return name; }

		Parameters& operator[](std::string parametersName);
		Parameters& get(std::string parametersName, int index);
	};
	std::ostream & operator<<(std::ostream &out, Sections &t);

	//����һ��ini
	class INItool
	{
		std::string filename;
		std::vector<Sections> sectionses;
		void ����(std::istream& in);

		friend std::ostream & operator<<(std::ostream &out, INItool &t);

	public:
		//��һ���ļ���ȡһ��ini
		INItool(std::string filename);
		INItool() = default;
		~INItool() = default;

		//���ַ�����ȡһ��ini
		void load(std::string string);

		//��ini���浽�ļ�
		bool save(std::string file_name);
		//��ini���浽�ļ�
		bool save();

		//�Ƿ���ָ����
		bool has(std::string sectionsName);

		//ȡ��ָ����
		Sections& operator[](std::string sectionsName);
		//ȡ��ָ����
		Sections& get(std::string sectionsName);

		//ɾ��ָ���μ����������м���ֵ
		//void del(std::string sectionsName);

		decltype(sectionses.begin()) begin() { return sectionses.begin(); }
		decltype(sectionses.end()) end() { return sectionses.end(); }
	};
	//��ini���뵽��
	std::ostream & operator<<(std::ostream &out, INItool &t);
}
