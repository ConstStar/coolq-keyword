#include "gui.h"

#include <CQSDK.h>

#include "myJson.h"

#include <boost/regex.hpp>

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/menu.hpp>

using namespace nana;
using namespace boost;
using namespace std;


extern MyJson conf;


//��Ҫ����
class tab_page_main
	: public panel<false>
{
private:

	//д������
	void writeConf()
	{
		conf.all2json();
		conf.json2file();
	}

	//��ȡ����
	void readConf()
	{
		conf.file2json();
		conf.json2all();

		//��������
		text_prefix.reset(conf.prefix);

		text_admin.reset();
		for (auto id : conf.admin)
		{
			text_admin.append(to_string(id) + "\n", true);
		}
	}


	//��ʼ��
	void init()
	{
		place_.bind(*this);
		place_.div(

			//����߾�
			"margin = [15,15,15,15] "

			//����QQ
			"<vert <weight=25 lab_admin> <text_admin>>"
			"<weight=10>"


			"<vert"
			"<weight=25>"

			//ָ��ǰ׺
			"<weight=25 <weight=65 lab_prefix><text_prefix>>"
			"<weight=10>"

			//����Կ
			"<weight=25 lab_usingKey>"
			"<weight=25 <text_usingKey><weight=60 button_usingKey>>"
			"<>"

			//����
			"<weight=25% button_save>"

			">");

		//����QQ
		lab_admin.create(*this);
		lab_admin.caption(u8"����QQ(ÿ��һ��):");
		place_.field("lab_admin") << lab_admin;

		text_admin.create(*this);
		place_.field("text_admin") << text_admin;


		//ָ��ǰ׺
		lab_prefix.create(*this);
		lab_prefix.caption(u8"ָ��ǰ׺��");
		place_.field("lab_prefix") << lab_prefix;

		text_prefix.create(*this);
		text_prefix.line_wrapped(false);
		text_prefix.multi_lines(false);
		place_.field("text_prefix") << text_prefix;


		//ʹ����Կ
		lab_usingKey.create(*this);
		lab_usingKey.caption(u8"����רҵ�棺");
		place_.field("lab_usingKey") << lab_usingKey;

		text_usingKey.create(*this);
		text_usingKey.line_wrapped(false);
		text_usingKey.multi_lines(false);
		place_.field("text_usingKey") << text_usingKey;

		btn_usingKey.create(*this);
		btn_usingKey.caption(u8"ʹ����Կ");
		btn_usingKey.events().click([this] {

		});
		place_.field("button_usingKey") << btn_usingKey;


		//���水ť
		btn_save.create(*this);
		btn_save.caption(u8"����");
		btn_save.events().click([this] {

			bool ok = regex_match(text_admin.text(), regex("[\\n\\d\\r]*"));
			if (!ok)
			{
				msgbox m_error{ *this,u8"����" };
				m_error.icon(msgbox::icon_error);
				m_error << u8"����QQ��ʽ����";
				m_error.show();
				return;
			}

			//����QQ
			auto line = text_admin.text_line_count();
			conf.admin.clear();
			for (int i = 0; i < line; i++)
			{
				string buf;
				text_admin.getline(i, buf);
				if (!buf.empty())
					conf.admin.push_back(atoll(buf.c_str()));
			}

			//��Ϣǰ׺
			conf.prefix = text_prefix.text();

			//д������
			writeConf();

			msgbox m_save{ *this,u8"�ɹ�" };
			m_save << u8"����ɹ�";
			m_save.show();

			//������������
			readConf();
		});
		place_.field("button_save") << btn_save;

		readConf();
	}

public:
	tab_page_main(window wd)
		: panel<false>(wd)
	{
		init();
	}



private:
	place place_;
	int conf_index;

	//����QQ
	label lab_admin;
	textbox text_admin;

	//ָ��ǰ׺
	label lab_prefix;
	textbox text_prefix;

	//ʹ����Կ
	label lab_usingKey;
	textbox text_usingKey;
	button btn_usingKey;


	//����
	button btn_save;

};


//���������е���Ҫ����
class tab_page_aloneMain
	: public panel<false>
{
private:

	//д������
	void writeConf()
	{
		conf.all2json();
		conf.json2file();
	}

	//��ȡ����
	void readConf()
	{
		conf.file2json();
		conf.json2all();

		text_name.reset(conf.alone[conf_index].name);
		text_priority.from(conf.alone[conf_index].priority);
	}


	//��ʼ��
	void init()
	{
		place_.bind(*this);
		place_.div(

			//����߾�
			"margin = [15,15,15,15] "


			"<vert"
			"<weight=25>"

			//���ȼ�
			"<weight=25 <weight=65 lab_priority><weight=65 text_priority>>"
			"<weight=10>"

			//��������
			"<weight=25 <weight=65 lab_name><weight=65 text_name>>"
			"<weight=60%>"

			//����
			"<<> <button_save> <>>"

			">");


		//��������
		lab_priority.create(*this);
		lab_priority.caption(u8"���ȼ���");
		place_.field("lab_priority") << lab_priority;

		text_priority.create(*this);
		text_priority.line_wrapped(false);
		text_priority.multi_lines(false);
		place_.field("text_priority") << text_priority;

		//��������
		lab_name.create(*this);
		lab_name.caption(u8"�������ƣ�");
		place_.field("lab_name") << lab_name;

		text_name.create(*this);
		text_name.line_wrapped(false);
		text_name.multi_lines(false);
		place_.field("text_name") << text_name;


		//���水ť
		btn_save.create(*this);
		btn_save.caption(u8"����");
		btn_save.events().click([this] {

			int priority_temp = text_priority.to_int();
			if (priority_temp <= 0)
			{
				msgbox m_error{ *this,u8"����" };
				m_error.icon(msgbox::icon_error);
				m_error << u8"���ȼ�����Ϊ����";
				m_error.show();

				return;
			}


			conf.alone[conf_index].priority = priority_temp;
			conf.alone[conf_index].name = text_name.text();

			writeConf();

			msgbox m_save{ *this,u8"�ɹ�" };
			m_save << u8"����ɹ�";
			m_save.show();

			//������������
			readConf();
		});
		place_.field("button_save") << btn_save;

		readConf();
	}

public:
	tab_page_aloneMain(window wd, int index = 0)
		: panel<false>(wd)
		, conf_index(index)
	{
		init();
	}

private:
	place place_;
	int conf_index;

	//���ȼ�
	label lab_priority;
	textbox text_priority;

	//��������
	label lab_name;
	textbox text_name;

	//����
	button btn_save;

};

//ȫ������
class tab_page_overall
	: public panel<false>
{
private:
	void writeConf()
	{
		conf.all2json();
		conf.json2file();
	}

	void readConf()
	{

		conf.file2json();
		conf.json2all();


		//����Ⱥ����
		auto groupList = CQ::getGroupList();

		//���ԭ��
		listGroupList.erase();

		//��������ѡ
		for (auto temp : conf.alone[conf_index].groupList)
		{
			listGroupList.at(0).append({ groupList[temp], to_string(temp) });
			groupList.erase(temp);
		}

		//������δѡ
		for (auto temp : groupList)
		{
			listGroupList.at(0).append({ temp.second, to_string(temp.first) });
		}

		//��ѡ��ѡ��
		auto size = listGroupList.size_item(0);
		for (int i = 0; i < size; i++)
		{
			string buf = listGroupList.at(0).at(i).text(1);

			if (find(conf.alone[conf_index].groupList.begin(), conf.alone[conf_index].groupList.end(), atoll(buf.c_str())) != conf.alone[conf_index].groupList.end())
			{
				listGroupList.at(0).at(i).check(true);
			}
		}

		//����ʽ
		checkDeal.at(conf.alone[conf_index].dealType)->check(true);

		//����ʱ��
		textBenTimeLen.reset(to_string(conf.alone[conf_index].banTimeLen));

		//һЩ����
		checkDeleteMsg.check(conf.alone[conf_index].deleteMsg);
		checkGroupWarn.check(conf.alone[conf_index].keyWordGroupWarn);
		checkStreng.check(conf.alone[conf_index].streng);

		//˽����Ϣת��������
		checkRelayPrivateMsg.check(conf.relayPrivateMsg);

	}

	void init()
	{
		place_.bind(*this);
		place_.div(

			//����߾�
			"margin = [15,15,15,15] "

			//Ⱥ����
			"<vert weight=250 <list> <weight=25 <text_addgroup><weight=80 button_addgroup>><weight=5><weight=30 gap=[10,10] arrange=[40,40] button_list>>"
			"<weight=20>"

			"<vert"
			//����ʽ
			"<weight=20 labelDeal>"
			"<vert weight=25% checkDeal>"
			"<weight=20 <weight=25% labelBanTimeLen> <weight=25% TextBanTimeLen><labelBanTimeLenTip>>"
			"<weight=10>"

			//һЩ����
			"<weight=25 labelSwitch>"
			"<weight=20% vert checkSwitch>"
			"<weight=10>"

			//���水ť
			"<weight=10% btnSave>"

			">"
		);

		//�б�
		listGroupList.create(*this);
		listGroupList.checkable(true);
		listGroupList.append_header(u8"Ⱥ��");
		listGroupList.append_header(u8"Ⱥ����");
		place_.field("list") << listGroupList;


		//�ֶ����Ⱥ
		textAddGroup.create(*this);
		textAddGroup.line_wrapped(false);
		textAddGroup.multi_lines(false);
		place_.field("text_addgroup") << textAddGroup;

		btnGroupAdd.create(*this);
		btnGroupAdd.caption(u8"�ֶ����");
		btnGroupAdd.events().click([this] {

			bool ok = regex_match(textAddGroup.text(), regex("[1-9][0-9]*"));
			if (!ok)
			{
				msgbox m_error{ *this ,u8"����" };
				m_error.icon(msgbox::icon_error);
				m_error << u8"��ʽ����";
				m_error.show();
				return;
			}


			listGroupList.at(0).append({ "",textAddGroup.text() });

			//����������ѡ
			auto size = listGroupList.size_item(0);
			for (int i = 0; i < size; i++)
			{
				string buf = listGroupList.at(0).at(i).text(1);

				if (buf == textAddGroup.text())
				{
					listGroupList.at(0).at(i).check(true);
					break;
				}
			}
		});
		place_.field("button_addgroup") << btnGroupAdd;



		//�б�ť
		//ȫѡ
		btnListAll.create(*this);
		btnListAll.caption(u8"ȫѡ");
		btnListAll.events().click([this] {

			auto size = listGroupList.size_item(0);

			for (int i = 0; i < size; i++)
			{
				listGroupList.at(listbox::index_pair(0, i)).check(true);
			}
		});
		//��ѡ
		btnListReverse.create(*this);
		btnListReverse.caption(u8"��ѡ");
		btnListReverse.events().click([this] {

			auto size = listGroupList.size_item(0);

			for (int i = 0; i < size; i++)
			{
				auto p = listGroupList.at(listbox::index_pair(0, i));

				p.check(!p.checked());
			}
		});
		place_.field("button_list") << btnListAll << btnListReverse;



		//����ʽ
		labelDeal.create(*this);
		labelDeal.caption(u8"����ʽ:");
		place_.field("labelDeal") << labelDeal;

		vector<string> groupStr;
		groupStr.push_back(u8"��������");
		groupStr.push_back(u8"����");
		groupStr.push_back(u8"�߳�");
		groupStr.push_back(u8"���ڲ��߳�");
		for (int i = 0; i < groupStr.size(); i++)
		{
			auto p = std::make_shared<checkbox>(*this);
			checkDeal.push_back(p);

			//Add the checkbox to the radio group. The radio group does not
			//manage the life of checkboxs.
			groupDeal.add(*p);
			place_.field("checkDeal") << *p;

			p->caption(groupStr[i]);
			p->events().click([this]()
			{
				std::size_t index = this->groupDeal.checked();
				std::string str = this->checkDeal[index]->caption();


				if (str == u8"����")
				{
					labelBanTimeLenTip.caption(u8"����");
				}
				else
				{
					labelBanTimeLenTip.caption(u8"����(���Է�ʽ����Ч)");
				}

			});

		}

		labelBenTimeLen.create(*this);
		labelBenTimeLen.caption(u8"����ʱ��:");
		place_.field("labelBanTimeLen") << labelBenTimeLen;

		textBenTimeLen.create(*this);
		textBenTimeLen.line_wrapped(false);
		textBenTimeLen.multi_lines(false);
		place_.field("TextBanTimeLen") << textBenTimeLen;

		labelBanTimeLenTip.create(*this);
		labelBanTimeLenTip.caption(u8"����");
		place_.field("labelBanTimeLenTip") << labelBanTimeLenTip;

		//һЩ���ܿ���
		labelSwitch.create(*this);
		labelSwitch.caption(u8"һЩ���ܿ���:");
		place_.field("labelSwitch") << labelSwitch;

		checkGroupWarn.create(*this);
		checkGroupWarn.caption(u8"�����ؼ��ʷ���Ⱥ��Ϣ����");
		place_.field("checkSwitch") << checkGroupWarn;

		checkRelayPrivateMsg.create(*this);
		checkRelayPrivateMsg.caption(u8"˽����Ϣת��������");
		if (!conf_index)//ֻ��Ĭ����������ʾ
			place_.field("checkSwitch") << checkRelayPrivateMsg;

		checkStreng.create(*this);
		checkStreng.caption(u8"�ؼ���ǿ�����");
		place_.field("checkSwitch") << checkStreng;

		checkDeleteMsg.create(*this);
		checkDeleteMsg.caption(u8"���ش����ؼ�����Ϣ");
		place_.field("checkSwitch") << checkDeleteMsg;


		btnSave.create(*this);
		btnSave.caption(u8"����");
		btnSave.events().click([this]
		{

			//Ⱥ����
			conf.alone[conf_index].groupList.clear();
			for (auto temp : listGroupList.checked())
			{
				string buf = listGroupList.at(temp).text(1);

				conf.alone[conf_index].groupList.push_back(atoll(buf.c_str()));
			}

			//����ʽ
			int dealType = 0;
			for (int i = 0; i < checkDeal.size(); i++)
			{
				if (checkDeal.at(i)->checked())
				{
					dealType = i;
					break;
				}
			}
			conf.alone[conf_index].dealType = dealType;

			//����ʱ��
			conf.alone[conf_index].banTimeLen = atoll(textBenTimeLen.text().c_str());

			//һЩ����
			conf.alone[conf_index].deleteMsg = checkDeleteMsg.checked();
			conf.alone[conf_index].keyWordGroupWarn = checkGroupWarn.checked();
			conf.alone[conf_index].streng = checkStreng.checked();

			conf.relayPrivateMsg = checkRelayPrivateMsg.checked();


			//д������
			writeConf();

			msgbox m_save{ *this,u8"�ɹ�" };
			m_save << u8"����ɹ�";
			m_save.show();

			//������������
			readConf();

		});
		place_.field("btnSave") << btnSave;


		//��ȡ����
		readConf();
	}

public:

	tab_page_overall(window wd, int index = 0)
		: panel<false>(wd)
		, conf_index(index)
	{
		init();
	}


private:
	place place_;
	int conf_index;

	//Ⱥ����
	listbox listGroupList;			//Ⱥ�б�

	textbox textAddGroup;			//�ֶ����Ⱥ	�ı���
	button btnGroupAdd;				//�ֶ����Ⱥ	��ť	

	button btnListReverse;			//��ѡ
	button btnListAll;				//ȫѡ


	//����ʽ
	label labelDeal;
	std::vector<std::shared_ptr<checkbox>> checkDeal;
	radio_group groupDeal;
	label labelBenTimeLen;				//����ʱ�� ��ǩ
	label labelBanTimeLenTip;			//����ʱ�� ��ʾ��ǩ
	textbox textBenTimeLen;				//����ʱ�� �ı���

	//һЩ���ܿ���
	label labelSwitch;
	checkbox checkGroupWarn;		//�����ؼ��ʷ���Ⱥ��Ϣ����
	checkbox checkRelayPrivateMsg;	//˽����Ϣת��������
	checkbox checkStreng;			//ǿ�����
	checkbox checkDeleteMsg;		//������Ϣ

	//����
	button btnSave;
};

//�б�
class tab_page_list
	: public panel<false>
{
private:

	//д������
	void writeConf()
	{
		conf.all2json();
		conf.json2file();
	}

	//��ȡ����
	void readConf()
	{
		conf.file2json();
		conf.json2all();

		//QQ�б�
		text_QQList.reset();
		for (auto id : conf.alone[conf_index].QQList)
		{
			text_QQList.append(to_string(id) + "\n", true);
		}

		//�������ؼ���
		text_keyWordWhite.reset();
		for (auto keyword : conf.alone[conf_index].keyWordWhite)
		{
			text_keyWordWhite.append(keyword.keyWord + "\n", true);
		}

		//������������
		check_QQListType.at(conf.alone[conf_index].QQListType)->check(true);
	}


	void init()
	{
		place_.bind(*this);
		place_.div(

			//����߾�
			"margin = [15,15,15,15] "

			//���
			"<vert"

			//�������ؼ���
			"<vert <weight=25 lab_keyWordWhite> <text_keyWordWhite>>"
			"<weight=10>"

			//������������Ϊ  ������ || �������
			"<vert weight=30% <eight=30% lab_QQListType> <eight=30% check_QQListType> <lab_QQListTypeTip>>"

			">"

			"<weight=10>"

			//�ұ�
			"<vert"

			//����QQ����
			"<vert <weight=25 <lab_QQList>> <text_QQList>>"
			"<weight=10>"

			//����
			"<weight=25% button_save>"

			">");

		//�������ؼ���
		lab_keyWordWhite.create(*this);
		lab_keyWordWhite.caption(u8"�������ؼ���(ÿ��һ��):");
		place_.field("lab_keyWordWhite") << lab_keyWordWhite;

		text_keyWordWhite.create(*this);
		text_keyWordWhite.tip_string(u8"������Щ�ؼ��ʵ���Ϣ��������");
		place_.field("text_keyWordWhite") << text_keyWordWhite;


		//����QQ����
		lab_QQList.create(*this);
		lab_QQList.caption(u8"QQ ������/�������(ÿ��һ��):");
		place_.field("lab_QQList") << lab_QQList;

		text_QQList.create(*this);
		place_.field("text_QQList") << text_QQList;

		//������������Ϊ  ������ || �������
		lab_QQListType.create(*this);
		lab_QQListType.caption(u8"������������Ϊ:");
		place_.field("lab_QQListType") << lab_QQListType;

		lab_QQListTypeTip.create(*this);
		place_.field("lab_QQListTypeTip") << lab_QQListTypeTip;

		vector<string> groupStr;
		groupStr.push_back(u8"������");
		groupStr.push_back(u8"�������");


		for (int i = 0; i < groupStr.size(); i++)
		{
			auto p = std::make_shared<checkbox>(*this);
			check_QQListType.push_back(p);

			group_QQListType.add(*p);
			place_.field("check_QQListType") << *p;

			p->caption(groupStr[i]);
			p->events().click([this]()
			{
				std::size_t index = this->group_QQListType.checked();
				std::string str = this->check_QQListType[index]->caption();


				if (str == u8"������")
				{
					lab_QQListTypeTip.caption(u8"��ǰΪ������ģʽ�������������е�QQ");
					lab_QQList.caption(u8"QQ������(ÿ��һ��):");
				}
				else if (str == u8"�������")
				{
					lab_QQListTypeTip.caption(u8"��ǰΪ�������ģʽ��ֻ��������е�QQ");
					lab_QQList.caption(u8"QQ�������(ÿ��һ��):");
				}
				else
				{
					lab_QQListTypeTip.caption("");
				}

			});

		}

		//���水ť
		btn_save.create(*this);
		btn_save.caption(u8"����");
		btn_save.events().click([this] {

			//QQ��������
			auto QQList_line = text_QQList.text_line_count();
			conf.alone[conf_index].QQList.clear();
			for (int i = 0; i < QQList_line; i++)
			{
				string buf;
				text_QQList.getline(i, buf);
				if (!buf.empty())
					conf.alone[conf_index].QQList.push_back(atoll(buf.c_str()));
			}

			//�������ؼ���
			auto keyWrodWhite_line = text_keyWordWhite.text_line_count();
			conf.alone[conf_index].keyWordWhite.clear();
			for (int i = 0; i < keyWrodWhite_line; i++)
			{
				string buf;
				text_keyWordWhite.getline(i, buf);
				if (!buf.empty())
					conf.alone[conf_index].keyWordWhite.push_back(buf.c_str());
			}

			//������������  ������||�������
			int listType = 0;
			for (int i = 0; i < check_QQListType.size(); i++)
			{
				if (check_QQListType.at(i)->checked())
				{
					listType = i;
					break;
				}
			}
			conf.alone[conf_index].QQListType = listType;


			writeConf();

			msgbox m_save{ *this,u8"�ɹ�" };
			m_save << u8"����ɹ�";
			m_save.show();

			//���¶�ȡ����
			readConf();

		});

		place_.field("button_save") << btn_save;

		readConf();
	}



public:
	tab_page_list(window wd, int index = 0)
		: panel<false>(wd)
		, conf_index(index)
	{
		init();
	}


private:
	place place_;
	int conf_index;

	//�������ؼ���
	label lab_keyWordWhite;
	textbox text_keyWordWhite;

	//QQ�б�
	label lab_QQList;
	textbox text_QQList;

	//����QQ��������
	label lab_QQListType;
	std::vector<std::shared_ptr<checkbox>> check_QQListType;
	radio_group group_QQListType;
	label lab_QQListTypeTip;

	//����
	button btn_save;

};

//�ؼ���
class tab_page_keyWord
	: public panel<false>
{
private:
	//д������
	void writeConf()
	{
		conf.all2json();
		conf.json2file();
	}

	//��ȡ����
	void readConf()
	{
		conf.file2json();
		conf.json2all();

		//��ͨ�ؼ���
		text_keyWord.reset();
		for (auto keyword : conf.alone[conf_index].keyWord)
		{
			text_keyWord.append(keyword.keyWord + "\n", true);
		}

		//������ʽ�ؼ���
		text_keyWordRegex.reset();
		for (auto keyword : conf.alone[conf_index].keyWordRegex)
		{
			text_keyWordRegex.append(keyword.keyWord + "\n", true);
		}
	}


	void init()
	{
		place_.bind(*this);
		place_.div(

			//����߾�
			"margin = [15,15,15,15] "

			//��ͨ�ؼ���
			"<vert <weight=25 lab_keyWord> <text_keyWord>>"
			"<weight=10>"


			"<vert"

			//������ʽ�ؼ���
			"<vert <weight=25 <lab_keyWordRegex> <weight=30% margin = [0,0,7] btn_regexTest>> <text_keyWordRegex>>"
			"<weight=10>"

			//����
			"<weight=25% button_save>"

			">");

		//��ͨ�ؼ���
		lab_keyWord.create(*this);
		lab_keyWord.caption(u8"��ͨ�ؼ���(ÿ��һ��):");
		place_.field("lab_keyWord") << lab_keyWord;

		text_keyWord.create(*this);
		place_.field("text_keyWord") << text_keyWord;


		//������ʽ�ؼ���
		lab_keyWordRegex.create(*this);
		lab_keyWordRegex.caption(u8"������ʽ�ؼ���(ÿ��һ��):");
		place_.field("lab_keyWordRegex") << lab_keyWordRegex;

		text_keyWordRegex.create(*this);
		place_.field("text_keyWordRegex") << text_keyWordRegex;


		//������ʽ����
		btn_regexTest.create(*this);
		btn_regexTest.caption(u8"��������");
		btn_regexTest.events().click([this] {

			//openRegexTest

		});
		place_.field("btn_regexTest") << btn_regexTest;


		//���水ť
		btn_save.create(*this);
		btn_save.caption(u8"����");
		btn_save.events().click([this] {

			//�ؼ���
			auto keyWord_line = text_keyWord.text_line_count();
			conf.alone[conf_index].keyWord.clear();
			for (int i = 0; i < keyWord_line; i++)
			{
				string buf;
				text_keyWord.getline(i, buf);
				if (!buf.empty())
					conf.alone[conf_index].keyWord.push_back(buf.c_str());
			}

			//������ʽ�ؼ���
			auto keyWordRegex_line = text_keyWordRegex.text_line_count();
			conf.alone[conf_index].keyWordRegex.clear();
			for (int i = 0; i < keyWordRegex_line; i++)
			{
				string buf;
				text_keyWordRegex.getline(i, buf);
				if (!buf.empty())
					conf.alone[conf_index].keyWordRegex.push_back(buf.c_str());
			}

			//д������
			writeConf();

			msgbox m_save{ *this,u8"�ɹ�" };
			m_save << u8"����ɹ�";
			m_save.show();

			//���¶�ȡ����
			readConf();
		});
		place_.field("button_save") << btn_save;

		readConf();
	}

public:
	tab_page_keyWord(window wd, int index = 0)
		: panel<false>(wd)
		, conf_index(index)
	{
		init();
	}


private:
	place place_;
	int conf_index;

	//����QQ
	label lab_keyWord;
	textbox text_keyWord;

	//������ʽ�ؼ���
	label lab_keyWordRegex;
	textbox text_keyWordRegex;

	//������ʽ����
	button btn_regexTest;

	//����
	button btn_save;

};

//�Զ��崥������
class tab_page_groupWarnWord
	: public panel<false>
{
private:
	//д������
	void writeConf()
	{
		conf.all2json();
		conf.json2file();
	}

	//��ȡ����
	void readConf()
	{
		conf.file2json();
		conf.json2all();

		//��������
		text_warnWord.reset(conf.alone[conf_index].keyWordGroupWarnWord);
	}

	void init()
	{
		place_.bind(*this);
		place_.div(

			//����߾�
			"margin = [15,15,15,15] "

			//�Զ��崥������
			"<vert <weight=25 lab_warnWord> <text_warnWord>>"
			"<weight=10>"


			"<vert"

			//����
			"<vert <weight=25 lab_variable> <text_variable>>"
			"<weight=10>"

			//����
			"<weight=25% button_save>"

			">"
		);

		//�Զ��崥������
		lab_warnWord.create(*this);
		lab_warnWord.caption(u8"�Զ��崥������:");
		place_.field("lab_warnWord") << lab_warnWord;

		text_warnWord.create(*this);
		place_.field("text_warnWord") << text_warnWord;


		//����
		lab_variable.create(*this);
		lab_variable.caption(u8"���ñ���(���ο������ɱ༭):");
		place_.field("lab_variable") << lab_variable;

		text_variable.create(*this);
		text_variable.editable(false);
		place_.field("text_variable") << text_variable;


		//���水ť
		btn_save.create(*this);
		btn_save.caption(u8"����");
		btn_save.events().click([this] {

			conf.alone[conf_index].keyWordGroupWarnWord = text_warnWord.text();


			writeConf();

			msgbox m_save{ *this,u8"�ɹ�" };
			m_save << u8"����ɹ�";
			m_save.show();

			readConf();
		});
		place_.field("button_save") << btn_save;

		readConf();
	}

public:
	tab_page_groupWarnWord(window wd, int index = 0)
		: panel<false>(wd)
		, conf_index(index)
	{
		init();
	}


private:
	place place_;
	int conf_index;

	//�Զ��崥������
	label lab_warnWord;
	textbox text_warnWord;

	//����
	label lab_variable;
	textbox text_variable;

	//����
	button btn_save;

};

//ת����Ⱥ
class tab_page_relayGroupMsg
	: public panel<false>
{
private:
	void writeConf()
	{
		conf.all2json();
		conf.json2file();
	}

	void readConf()
	{
		conf.file2json();
		conf.json2all();



		//����Ⱥ����
		auto groupList = CQ::getGroupList();

		//���ԭ��
		list_groupList.erase();

		//��������ѡ
		for (auto temp : conf.alone[conf_index].relayGroupList)
		{
			list_groupList.at(0).append({ groupList[temp], to_string(temp) });
			groupList.erase(temp);
		}

		//������δѡ
		for (auto temp : groupList)
		{
			list_groupList.at(0).append({ temp.second, to_string(temp.first) });
		}

		//��ѡ��ѡ��
		auto size = list_groupList.size_item(0);
		for (int i = 0; i < size; i++)
		{
			string buf = list_groupList.at(0).at(i).text(1);

			if (find(conf.alone[conf_index].relayGroupList.begin(), conf.alone[conf_index].relayGroupList.end(), atoll(buf.c_str())) != conf.alone[conf_index].relayGroupList.end())
			{
				list_groupList.at(0).at(i).check(true);
			}
		}


		//ת����ʽ
		text_relayGroupWord.reset(conf.alone[conf_index].relayGroupWord);

		text_relayGroupMsg_trimFront.reset(to_string(conf.alone[conf_index].relayGroupMsg_trimFront));
		text_relayGroupMsg_trimBack.reset(to_string(conf.alone[conf_index].relayGroupMsg_trimBack));
	}

	//��ʾ�ı�
	void showText(string title, string word)
	{
		form fm_temp;
		fm_temp.caption(title);


		textbox text{ fm_temp };
		text.caption(word);
		text.editable(false);

		fm_temp.div("<text>");
		fm_temp["text"] << text;
		fm_temp.collocate();

		fm_temp.show();
		exec();
	}

	void init()
	{
		place_.bind(*this);
		place_.div(

			//����߾�
			"margin = [15,15,15,15] "

			"<vert"
			"<"
			//Ⱥ�б�
			"<vert <weight=25 lab_groupList> <list_groupList>>"
			"<weight=10>"

			//ת����ʽ
			"<vert <weight=25 <lab_relayGroupWord> <margin = [0,0,7] button_variable>> <text_relayGroupWord>>"
			"<weight=10>"
			">"

			"<weight=20>"
			"<weight=25%"
			//��Ϣ�޼�
			"<vert <weight=25 lab_relayGroupMsg_trim> <vert <weight=25 <weight=80 lab_relayGroupMsg_trimFront><text_relayGroupMsg_trimFront>> <weight=25 <weight=80 lab_relayGroupMsg_trimBack><text_relayGroupMsg_trimBack>>>>"

			"<weight=10%>"

			//����
			"<weight=40% button_save>"
			">"
			">"
		);

		//Ⱥ�б�
		lab_groupList.create(*this);
		lab_groupList.caption(u8"ת����Ⱥ:");
		place_.field("lab_groupList") << lab_groupList;

		list_groupList.create(*this);
		list_groupList.checkable(true);
		list_groupList.append_header(u8"Ⱥ��");
		list_groupList.append_header(u8"Ⱥ����");
		place_.field("list_groupList") << list_groupList;

		//��Ϣ�޼�
		lab_relayGroupMsg_trim.create(*this);
		lab_relayGroupMsg_trim.caption(u8"��Ϣ�޼���ȥ���β��ר�ã�");
		place_.field("lab_relayGroupMsg_trim") << lab_relayGroupMsg_trim;

		lab_relayGroupMsg_trimFront.create(*this);
		lab_relayGroupMsg_trimFront.caption(u8"ɾ��ǰ������:");
		place_.field("lab_relayGroupMsg_trimFront") << lab_relayGroupMsg_trimFront;

		text_relayGroupMsg_trimFront.create(*this);
		text_relayGroupMsg_trimFront.line_wrapped(false);
		text_relayGroupMsg_trimFront.multi_lines(false);
		place_.field("text_relayGroupMsg_trimFront") << text_relayGroupMsg_trimFront;

		lab_relayGroupMsg_trimBack.create(*this);
		lab_relayGroupMsg_trimBack.caption(u8"ɾ����������:");
		place_.field("lab_relayGroupMsg_trimBack") << lab_relayGroupMsg_trimBack;

		text_relayGroupMsg_trimBack.create(*this);
		text_relayGroupMsg_trimBack.line_wrapped(false);
		text_relayGroupMsg_trimBack.multi_lines(false);
		place_.field("text_relayGroupMsg_trimBack") << text_relayGroupMsg_trimBack;


		//ת����Ϣ��ʽ
		lab_relayGroupWord.create(*this);
		lab_relayGroupWord.caption(u8"ת����Ϣ��ʽ:");
		place_.field("lab_relayGroupWord") << lab_relayGroupWord;

		text_relayGroupWord.create(*this);
		text_relayGroupWord.tip_string(u8"����д��ת����Ϣԭ��ʽ �� {msg}");
		place_.field("text_relayGroupWord") << text_relayGroupWord;

		//������ť
		button_variable.create(*this);
		button_variable.caption(u8"�鿴����");
		button_variable.events().click([this]
		{
			string buf(

				u8""

			);
			showText(u8"ת��Ⱥ��Ϣ����", buf);

		});
		place_.field("button_variable") << button_variable;

		//���水ť
		btn_save.create(*this);
		btn_save.caption(u8"����");
		btn_save.events().click([this] {

			//Ⱥ����
			conf.alone[conf_index].relayGroupList.clear();
			for (auto temp : list_groupList.checked())
			{
				string buf = list_groupList.at(temp).text(1);

				conf.alone[conf_index].relayGroupList.push_back(atoll(buf.c_str()));

			}

			//ת����ʽ
			conf.alone[conf_index].relayGroupWord = text_relayGroupWord.text();

			conf.alone[conf_index].relayGroupMsg_trimFront = atoi(text_relayGroupMsg_trimFront.text().c_str());
			conf.alone[conf_index].relayGroupMsg_trimBack = atoi(text_relayGroupMsg_trimBack.text().c_str());


			writeConf();

			msgbox m_save{ *this,u8"�ɹ�" };
			m_save << u8"����ɹ�";
			m_save.show();

			readConf();

		});
		place_.field("button_save") << btn_save;

		readConf();
	}

public:
	tab_page_relayGroupMsg(window wd, int index = 0)
		: panel<false>(wd)
		, conf_index(index)
	{
		init();
	}


private:
	place place_;
	int conf_index;

	//Ⱥ�б�
	label lab_groupList;
	listbox	list_groupList;

	//��Ϣ�޽�
	label lab_relayGroupMsg_trim;
	label lab_relayGroupMsg_trimFront;
	label lab_relayGroupMsg_trimBack;
	textbox text_relayGroupMsg_trimFront;
	textbox text_relayGroupMsg_trimBack;

	//����
	label lab_relayGroupWord;
	textbox text_relayGroupWord;

	//������ť
	button button_variable;

	//����
	button btn_save;

};

//��������
class tab_page_alone
	: public panel<false>
{
private:

	void readConf()
	{

		auto groupList = CQ::getGroupList();

		vector<string> dealTypeList{ "��������","����","�߳�","�߳�������" };


		list_aloneList.erase();
		for (auto tempAlone : conf.alone)
		{
			if (tempAlone.first == 0)
				continue;

			string groupListWord;
			dealTypeList[1] = "����" + to_string(tempAlone.second.banTimeLen) + "����";

			int i = 0;
			for (auto tempGroup : tempAlone.second.groupList)
			{
				groupListWord += groupList[tempGroup] + " ";
				if (i > 5)
					break;

				i++;
			}


			list_aloneList.at(0).append({ to_string(tempAlone.second.priority),tempAlone.second.name ,groupListWord ,dealTypeList[tempAlone.second.dealType] });
		}
	}


	void openAlone(int conf_index)
	{

		form fm(*this);
		fm.caption(u8"��������");
		fm.size(nana::size(550, 400));


		place place;
		place.bind(fm);
		place.div("<vert <weight=30 tabbar> <tab_frame>>");


		std::vector<std::shared_ptr<panel<false>>> tabpages;
		tabbar<std::string> tabbar{ fm };

		//��֪��Ϊɶ �����ǰ����scroll �Ϳ���ʵ����ʾ��ǰ���scrollҳ��
		tabbar.toolbox(nana::tabbar<std::string>::kits::scroll, false);

		tabbar.push_back(u8"��Ҫ����");
		tabpages.push_back(std::make_shared<tab_page_aloneMain>(fm, conf_index));
		tabbar.push_back(u8"ȫ������");
		tabpages.push_back(std::make_shared<tab_page_overall>(fm, conf_index));
		tabbar.push_back(u8"�ؼ���");
		tabpages.push_back(std::make_shared<tab_page_keyWord>(fm, conf_index));
		tabbar.push_back(u8"��������");
		tabpages.push_back(std::make_shared<tab_page_list>(fm, conf_index));
		tabbar.push_back(u8"��������");
		tabpages.push_back(std::make_shared<tab_page_groupWarnWord>(fm, conf_index));
		tabbar.push_back(u8"ת����Ⱥ");
		tabpages.push_back(std::make_shared<tab_page_relayGroupMsg>(fm, conf_index));


		std::size_t index = 0;
		for (auto& i : tabpages)
		{
			tabbar.attach(index++, *i);
			place.field("tab_frame").fasten(*i);
		}

		tabbar.toolbox(nana::tabbar<std::string>::kits::scroll, true);
		tabbar.toolbox(nana::tabbar<std::string>::kits::list, true);
		tabbar.activated(0);
		place.field("tabbar") << tabbar;
		place.collocate();

		fm.modality();
	}


	void init()
	{
		place_.bind(*this);
		place_.div(

			//����߾�
			"margin = [15,15,15,15] "

			"<vert"

			//Ⱥ�б�
			"<list_aloneList>"

			//���ȼ���ʾ
			"<weight=25 lab_priorityTip>"

			">"
		);

		//�б�
		list_aloneList.create(*this);
		list_aloneList.checkable(true);
		list_aloneList.append_header(u8"���ȼ�");
		list_aloneList.append_header(u8"��������");
		list_aloneList.append_header(u8"����Ⱥ");
		list_aloneList.append_header(u8"������");


		place_.field("list_aloneList") << list_aloneList;

		list_aloneList.events().mouse_up([this](const arg_mouse& arg) {

			if (mouse::right_button != arg.button)
				return;

			auto index = list_aloneList.selected();


			//��ֹ�������ڳ�ͻ���ȶ�ȡһ��
			readConf();

			//�˵�
			menu menu_;


			//����˵��¼�
			//���
			auto handlerAdd = [=](menu::item_proxy& ip) {

				int add_index = conf.alone.size();

				if (conf.alone.find(add_index) != conf.alone.end())
				{
					msgbox m_error(*this, u8"����");
					m_error.icon(msgbox::icon_error);
					m_error << u8"������id��ͻ�����ʧ�ܣ��볢��������Q";
					m_error.show();
					return;
				}


				openAlone(add_index);

				readConf();
			};

			//�༭
			auto handlerEdit = [=](menu::item_proxy& ip) {
				int conf_index = index.at(0).item + 1;

				openAlone(conf_index);

				readConf();
			};

			//ɾ��
			auto handlerDelete = [=](menu::item_proxy& ip) {

				//��ȡѡ����Ŀ�����ȼ�
				int priority = atoi(list_aloneList.at(index.at(0)).text(0).c_str());

				msgbox m{ *this,u8"ȷ��",nana::msgbox::yes_no };
				m << u8"�Ƿ�ɾ������";
				auto res = m.show();

				if (res == m.pick_yes)
				{
					int conf_index = index.at(0).item + 1;
					conf.alone.erase(conf_index);

					readConf();
				}

			};


			//�һ�����
			if (index.size())
			{
				menu_.append(u8"���", handlerAdd);
				menu_.append(u8"�༭", handlerEdit);
				menu_.append(u8"ɾ��", handlerDelete);

				menu_.popup_await(*this, arg.pos.x, arg.pos.y);
			}
			//�һ��հ�
			else
			{
				menu_.append(u8"���", handlerAdd);

				menu_.popup_await(*this, arg.pos.x, arg.pos.y);
			}

		});


		//���ȼ���ʾ
		lab_priorityTip.create(*this);
		lab_priorityTip.caption(u8"��ʾ:   ���ȼ���ֵС > ���ȼ���ֵ�� > ȫ������");
		place_.field("lab_priorityTip") << lab_priorityTip;


		readConf();
	}

public:
	tab_page_alone(window wd)
		: panel<false>(wd)
	{
		init();
	}


private:
	place place_;

	//�б�
	listbox	list_aloneList;

	//���ȼ���ʾ
	label lab_priorityTip;

};



Gui::Gui() :m_writeConf(nullptr)
{
}

Gui::Gui(void (*p)()) : m_writeConf(p)
{

}


//д������
void Gui::writeConf()
{
	if (m_writeConf != nullptr)
	{
		m_writeConf();
	}
}


//����Ҫ����
void Gui::openMain()
{
	form fm;
	fm.caption(u8"����");
	fm.size(nana::size(550, 400));

	place place;
	place.bind(fm);
	place.div("<vert <weight=30 tabbar> <tab_frame>>");


	std::vector<std::shared_ptr<panel<false>>> tabpages;
	tabbar<std::string> tabbar{ fm };

	//��֪��Ϊɶ �����ǰ����scroll �Ϳ���ʵ����ʾ��ǰ���scrollҳ��
	tabbar.toolbox(nana::tabbar<std::string>::kits::scroll, false);

	tabbar.push_back(u8"��Ҫ����");
	tabpages.push_back(std::make_shared<tab_page_main>(fm));
	tabbar.push_back(u8"ȫ������");
	tabpages.push_back(std::make_shared<tab_page_overall>(fm));
	tabbar.push_back(u8"�ؼ���");
	tabpages.push_back(std::make_shared<tab_page_keyWord>(fm));
	tabbar.push_back(u8"��������");
	tabpages.push_back(std::make_shared<tab_page_list>(fm));
	tabbar.push_back(u8"��������");
	tabpages.push_back(std::make_shared<tab_page_groupWarnWord>(fm));
	tabbar.push_back(u8"ת����Ⱥ");
	tabpages.push_back(std::make_shared<tab_page_relayGroupMsg>(fm));
	tabbar.push_back(u8"��������");
	tabpages.push_back(std::make_shared<tab_page_alone>(fm));


	std::size_t index = 0;
	for (auto& i : tabpages)
	{
		tabbar.attach(index++, *i);
		place.field("tab_frame").fasten(*i);
	}

	tabbar.toolbox(nana::tabbar<std::string>::kits::scroll, true);
	tabbar.toolbox(nana::tabbar<std::string>::kits::list, true);
	tabbar.activated(0);
	place.field("tabbar") << tabbar;
	place.collocate();
	fm.show();
	exec();
}