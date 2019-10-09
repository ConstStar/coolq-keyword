#pragma once
#include <string>
#include <vector>

namespace INI {
	class Sections;
	class INItool;

	//注释集合
	class Comments :public std::vector<std::string>
	{
		friend std::ostream & operator<<(std::ostream &out, Comments &t);
	};
	std::ostream & operator<< (std::ostream &out, Comments &t);

	//键值对
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

	//键值对集合(又名 段)
	class Sections :
		//        public //不想公开...
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

	//代表一个ini
	class INItool
	{
		std::string filename;
		std::vector<Sections> sectionses;
		void 解析(std::istream& in);

		friend std::ostream & operator<<(std::ostream &out, INItool &t);

	public:
		//从一个文件读取一个ini
		INItool(std::string filename);
		INItool() = default;
		~INItool() = default;

		//从字符串读取一个ini
		void load(std::string string);

		//将ini保存到文件
		bool save(std::string file_name);
		//将ini保存到文件
		bool save();

		//是否有指定段
		bool has(std::string sectionsName);

		//取出指定段
		Sections& operator[](std::string sectionsName);
		//取出指定段
		Sections& get(std::string sectionsName);

		//删除指定段及其下属所有键和值
		//void del(std::string sectionsName);

		decltype(sectionses.begin()) begin() { return sectionses.begin(); }
		decltype(sectionses.end()) end() { return sectionses.end(); }
	};
	//将ini输入到流
	std::ostream & operator<<(std::ostream &out, INItool &t);
}
