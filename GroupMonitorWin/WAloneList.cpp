// WAloneList.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WAloneList.h"
#include "afxdialogex.h"

#include "WDefConfig.h"

#include "../include/json/myJson.h"

#include <set>

#include <boost/timer.hpp>
//using namespace std;

// WAloneList 对话框

IMPLEMENT_DYNAMIC(WAloneList, CDialogEx)

WAloneList::WAloneList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGALONELIST, pParent)
	, m_GroupId(0)
{
}

WAloneList::~WAloneList()
{
}

void WAloneList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_GroupId);
}


BEGIN_MESSAGE_MAP(WAloneList, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &WAloneList::OnNMDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &WAloneList::OnLvnItemchangedList1)
	//	ON_NOTIFY(LVN_ODSTATECHANGED, IDC_LIST1, &WAloneList::OnLvnOdstatechangedList1)
	ON_BN_CLICKED(IDC_BUTTON9, &WAloneList::OnBnClickedButton9)
END_MESSAGE_MAP()


// WAloneList 消息处理程序


BOOL WAloneList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	DWORD dwStyle = m_list.GetExtendedStyle();

	//选中某行使整行高亮（只适用于报表风格的listctrl）
	//dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
	//m_list.SetExtendedStyle(dwStyle); //设置扩展风格

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.InsertColumn(0, TEXT("群名"), LVCFMT_LEFT, 170);
	m_list.InsertColumn(1, TEXT("群号码"), LVCFMT_LEFT, 100);


	//数据初始化

	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void WAloneList::InitData()
{
	static boost::timer initdata_time;//记录上次更新的时间 防止更新太快导致崩溃
	static bool startOne = false;

	if (startOne)
		if (initdata_time.elapsed() < 0.3)
			return;

	startOne = true;
	initdata_time.restart();
	//清空原来的列表
	this->m_list.DeleteAllItems();


	//先放置已经开启的群
	int m = 0;
	for (auto GroupId : g_AlGroupList)
	{
		CString GroupId_str;
		GroupId_str.Format(TEXT("%u"), GroupId);

		m_list.InsertItem(m, CString(WinGroupList[GroupId].c_str()));

		//存放群号码
		m_list.SetItemText(m, 1, GroupId_str);

		//存放是否启用
		m_list.SetCheck(m, true);

		m++;

	}

	//放置其他群
	for (auto GroupId : WinGroupList)
	{
		//排除已经开启的群
		if (find(g_AlGroupList.begin(), g_AlGroupList.end(), GroupId.first) != g_AlGroupList.end())
		{
			//MessageBox(CString(to_string(it.first).c_str()));  //for test
			continue;
		}

		//添加群名称
		CString GrouoNameCStr(GroupId.second.c_str());
		m_list.InsertItem(m, GrouoNameCStr);

		//添加群号码
		CString GroupIdCStr(to_string(GroupId.first).c_str());
		m_list.SetItemText(m, 1, GroupIdCStr);

		//设置是否启用
		m_list.SetCheck(m, false);
		m++;

	}
}


void WAloneList::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取群号码
	CString str;
	str = m_list.GetItemText(pNMItemActivate->iItem, 1);

	//将群号码转成longlong
	CStringA str2(str);
	long long GroupId = atoll(str2.GetBuffer());

	//获取群名称
	CString GroupName;
	GroupName = m_list.GetItemText(pNMItemActivate->iItem, 0);

	//创建窗口
	WDefConfig* p = new WDefConfig(GroupId);

	//窗口标题获取
	CString WindowsText("单独设置 ");
	WindowsText.Append(GroupName);
	WindowsText.AppendFormat(L"(%lld)", GroupId);

	//打开窗口
	p->Create(IDD_DIALOGDEFCFG);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);

	//编辑后自动勾选
	m_list.SetCheck(pNMItemActivate->iItem, TRUE);

	*pResult = 0;
}


//项目内容更改后
void WAloneList::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	g_AlGroupList.clear();
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i))
		{
			unsigned int temp = (unsigned int)atoll(CStringA(m_list.GetItemText(i, 1)).GetBuffer());
			//获取群号码 并存入
			g_AlGroupList.push_back(temp);
		}

	}

	//重新排序输出列表
	InitData();

	*pResult = 0;
}


//void WAloneList::OnLvnOdstatechangedList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLVODSTATECHANGE pStateChanged = reinterpret_cast<LPNMLVODSTATECHANGE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//
//	MessageBox(TEXT("TEST2"));
//	*pResult = 0;
//}


void WAloneList::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_GroupId)
		g_AlGroupList.push_back(m_GroupId);

	m_GroupId = 0;
	UpdateData(FALSE);
	InitData();
}
