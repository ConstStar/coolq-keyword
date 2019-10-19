// WMain.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WMain.h"
#include "afxdialogex.h"

#include "WAloneList.h"
#include "WMainConfig.h"
#include "WDefConfig.h"

#include "../include/json/myJson.h"

#include <string>
#include <set>

#include <boost/timer.hpp>

using namespace std;

// WMain 对话框

IMPLEMENT_DYNAMIC(WMain, CDialogEx)

WMain::WMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGMAIN, pParent)
	, m_GroupId(0)
{

}

WMain::~WMain()
{
}

void WMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_GroupId);
}


BEGIN_MESSAGE_MAP(WMain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &WMain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &WMain::OnBnClickedButton2)
	//	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &WMain::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON4, &WMain::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &WMain::OnBnClickedButton3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &WMain::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON5, &WMain::OnBnClickedButton5)
	//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &WMain::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON9, &WMain::OnBnClickedButton9)
	//	ON_NOTIFY(NM_CLICK, IDC_LIST1, &WMain::OnNMClickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &WMain::OnLvnItemchangedList1)
	ON_WM_QUERYOPEN()
END_MESSAGE_MAP()


// WMain 消息处理程序


BOOL WMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化 

	//屏幕位置
	int x, y;
	x = GetSystemMetrics(SM_CXSCREEN); //屏幕宽度
	y = GetSystemMetrics(SM_CYSCREEN); //屏幕高度

	SetWindowPos(NULL, x / 3, y / 3, 0, 0, SWP_NOZORDER | SWP_NOSIZE);


	DWORD dwStyle = m_list.GetExtendedStyle();

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.InsertColumn(0, TEXT("群名"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(1, TEXT("群号码"), LVCFMT_LEFT, 100);

	InitData();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void WMain::InitData()
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

	int m = 0;
	for (auto GroupId : g_GroupList)
	{

		CString GroupId_str;
		GroupId_str.Format(TEXT("%lld"), GroupId);

		//存放群名称
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
		if (find(g_GroupList.begin(), g_GroupList.end(), GroupId.first) != g_GroupList.end())
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


//全选
void WMain::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		m_list.SetCheck(i, TRUE);
	}
}


//反选
void WMain::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i) == FALSE)  //未被选中的
		{
			m_list.SetCheck(i, TRUE);
		}
		else  //选中的
		{
			m_list.SetCheck(i, FALSE);
		}
	}
}


//void WMain::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	CString str;
//
//	str.Format(TEXT("%d %d"), pNMItemActivate->iItem, pNMItemActivate->iSubItem);
//
//	MessageBox(str);
//
//
//	*pResult = 0;
//}


//单独设置
void WMain::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	WAloneList* p = new WAloneList;

	p->Create(IDD_DIALOGALONELIST);
	p->ShowWindow(SW_SHOW);
}


//全局设置
void WMain::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	/*WDefConfig *p = new WDefConfig;

	p->Create(IDD_DIALOGFORWARDMSG);
	p->ShowWindow(SW_SHOW);*/
	WDefConfig w;
	w.DoModal();

}

//双击
void WMain::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//MessageBox(TEXT("查看详细信息功能 正在努力开发中。。。"));

	*pResult = 0;
}

//主要设置
void WMain::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	WMainConfig* p = new WMainConfig;

	p->Create(IDD_DIALOGMAINCFG);
	p->ShowWindow(SW_SHOW);
}

//手动添加
void WMain::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);


	UpdateData(TRUE);

	if(m_GroupId)
		g_GroupList.push_back(m_GroupId);

	m_GroupId = 0;
	UpdateData(FALSE);

	InitData();
}

//列表被单击
//void WMain::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//
//	//重新排序输出列表
//	InitData();
//
//	*pResult = 0;
//}

//列表被修改
void WMain::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//清空原有数据
	g_GroupList.clear();

	//重新添加数据
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i) == TRUE)
		{
			//获取群号码 并存入
			g_GroupList.push_back(atoll(CStringA(m_list.GetItemText(i, 1)).GetBuffer()));
		}
	}

	InitData();
	*pResult = 0;
}


BOOL WMain::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::OnCommand(wParam, lParam);
}


//HRESULT WMain::accDoDefaultAction(VARIANT varChild)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CDialogEx::accDoDefaultAction(varChild);
//}


BOOL WMain::OnQueryOpen()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnQueryOpen();
}
