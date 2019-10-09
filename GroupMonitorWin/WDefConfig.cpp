// WDefConfig.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WDefConfig.h"
#include "afxdialogex.h"

#include "WAllWord.h"
#include "WForwardMsg.h"

#include "../include/json/myJson.h"

#include <vector>
// WDefConfig 对话框



IMPLEMENT_DYNAMIC(WDefConfig, CDialogEx)

WDefConfig::WDefConfig(long long GroupId, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGDEFCFG, pParent), GroupId(GroupId)
	, m_Way(::g_conf[GroupId].way)
	, m_WayTime(::g_conf[GroupId].wayTime)
	, m_GroupWarn(::g_conf[GroupId].GroupWarn)
	, m_Streng(::g_conf[GroupId].Streng)
	, m_Revoke(::g_conf[GroupId].Revoke)
	, m_typeQQList(::g_conf[GroupId].typeQQList)
{

}


WDefConfig::~WDefConfig()
{
}

void WDefConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Way);
	DDX_Text(pDX, IDC_EDIT1, m_WayTime);
	DDX_Check(pDX, IDC_CHECK1, m_GroupWarn);
	DDX_Check(pDX, IDC_CHECK3, m_Streng);
	DDX_Check(pDX, IDC_CHECK4, m_Revoke);
	DDX_Radio(pDX, IDC_RADIO7, m_typeQQList);
}


BEGIN_MESSAGE_MAP(WDefConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &WDefConfig::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &WDefConfig::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &WDefConfig::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &WDefConfig::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &WDefConfig::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &WDefConfig::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &WDefConfig::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &WDefConfig::OnBnClickedButton8)
END_MESSAGE_MAP()


// WDefConfig 消息处理程序


//默认关键词
void WDefConfig::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str_temp;
	OperateLine::line_get_str(str_temp, g_keyword[GroupId]);

	////创建窗口
	//WAllWord *p = new WAllWord(str_temp);
	WAllWord w(str_temp);

	////窗口标题获取
	//CString WindowsText("默认关键词");

	////打开窗口
	//p->Create(IDD_DIALOGALLWORD);
	//p->SetWindowTextW(WindowsText);
	//p->ShowWindow(SW_SHOW);
	w.DoModal();


	OperateLine::line_put_str(str_temp, g_keyword[GroupId]);
}

//白名单关键词
void WDefConfig::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str_temp;
	OperateLine::line_get_str(str_temp, g_KeyWordWhite[GroupId]);

	////创建窗口
	//WAllWord *p = new WAllWord(str_temp);
	WAllWord w(str_temp);

	////窗口标题获取
	//CString WindowsText("白名单关键词");

	////打开窗口
	//p->Create(IDD_DIALOGALLWORD);
	//p->SetWindowTextW(WindowsText);
	//p->ShowWindow(SW_SHOW);
	w.DoModal();

	OperateLine::line_put_str(str_temp, g_KeyWordWhite[GroupId]);
}

//白名单QQ / 监控QQ
void WDefConfig::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str_temp;
	OperateLine::line_get_ll(str_temp, g_qqlist[GroupId]);


	////创建窗口
	//WAllWord *p = new WAllWord(str_temp);
	WAllWord w(str_temp);

	////窗口标题获取
	//CString WindowsText("监控QQ/白名单QQ");

	////打开窗口
	//p->Create(IDD_DIALOGALLWORD);
	//p->SetWindowTextW(WindowsText);
	//p->ShowWindow(SW_SHOW);
	w.DoModal();

	OperateLine::line_put_ll(str_temp, g_qqlist[GroupId]);
}

//打开自定义触发提醒
void WDefConfig::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str_temp(g_conf[GroupId].WordKeyWarn.c_str());

	////创建窗口
	//WAllWord *p = new WAllWord(str_temp);
	WAllWord w(str_temp);

	////窗口标题获取
	//CString WindowsText("自定义触发关键词提醒");

	////打开窗口
	//p->Create(IDD_DIALOGALLWORD);
	//p->SetWindowTextW(WindowsText);
	//p->ShowWindow(SW_SHOW);
	w.DoModal();

	g_conf[GroupId].WordKeyWarn = CStringA(str_temp).GetBuffer();
}

//保存
void WDefConfig::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//触发关键词提醒
	g_conf[GroupId].GroupWarn = (bool)m_GroupWarn;

	//撤回
	g_conf[GroupId].Revoke = (bool)m_Revoke;

	//强力检测
	g_conf[GroupId].Streng = (bool)m_Streng;

	//处理方法
	g_conf[GroupId].way = m_Way;

	//禁言时间
	g_conf[GroupId].wayTime = m_WayTime;

	//是否为 监控名单 或者 白名单
	g_conf[GroupId].typeQQList = m_typeQQList;


	MessageBox(TEXT("保存成功"));
	CDialogEx::OnOK();
}

//初始化
BOOL WDefConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//正则表达式关键词
void WDefConfig::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str_temp;
	OperateLine::line_get_str(str_temp, g_REKeyWord[GroupId]);

	////创建窗口
	//WAllWord *p = new WAllWord(str_temp);
	WAllWord w(str_temp);

	////窗口标题获取
	//CString WindowsText("正则表达式关键词");

	////打开窗口
	//p->Create(IDD_DIALOGALLWORD);
	//p->SetWindowTextW(WindowsText);
	//p->ShowWindow(SW_SHOW);
	w.DoModal();

	OperateLine::line_put_str(str_temp, g_REKeyWord[GroupId]);
}


//转发到群
void WDefConfig::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码


	CString str_temp;
	OperateLine::line_get_ll(str_temp, g_sendGroupList[GroupId]);


	WAllWord w(str_temp);


	w.DoModal();

	OperateLine::line_put_ll(str_temp, g_sendGroupList[GroupId]);

}

//转发到群格式
void WDefConfig::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码


	CString temp_word(g_conf[GroupId].sendGroupMsg_word.c_str());

	//WForwardMsg *p = new WForwardMsg(
	//	temp_word,
	//	g_conf[GroupId].sendGroupMsg_frontLine, 
	//	g_conf[GroupId].sendGroupMsg_afterLine);


	////打开窗口
	//p->Create(IDD_DIALOGFORWARDMSG);
	//p->ShowWindow(SW_SHOW);
	WForwardMsg w(
		temp_word,
		g_conf[GroupId].sendGroupMsg_frontLine,
		g_conf[GroupId].sendGroupMsg_afterLine);

	w.DoModal();

	g_conf[GroupId].sendGroupMsg_word = CStringA(temp_word).GetBuffer();
}
