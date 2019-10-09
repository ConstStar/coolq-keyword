// WMainConfig.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WMainConfig.h"
#include "afxdialogex.h"

#include "../include/json/myJson.h"

#include <vector>


// WMainConfig 对话框

IMPLEMENT_DYNAMIC(WMainConfig, CDialogEx)

WMainConfig::WMainConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGMAINCFG, pParent)
	, m_root(_T(""))
	, m_MsgRelay(FALSE)
	, m_Prefix(_T(""))
{

}

WMainConfig::~WMainConfig()
{
}

void WMainConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_root);
	DDX_Check(pDX, IDC_CHECK1, m_MsgRelay);
	DDX_Text(pDX, IDC_EDIT2, m_Prefix);
}


BEGIN_MESSAGE_MAP(WMainConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &WMainConfig::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &WMainConfig::OnBnClickedButton3)
END_MESSAGE_MAP()

//切割子串
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

// WMainConfig 消息处理程序


BOOL WMainConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//读取 私聊消息转发
	m_MsgRelay = g_MsgRelay;

	//读取 指令前缀
	m_Prefix = g_prefix.c_str();

	//读取 主人号码
	OperateLine::line_get_ll(m_root, g_root);


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void WMainConfig::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	//写入 私聊消息转发
	g_MsgRelay = m_MsgRelay;

	//写入 指令前缀
	g_prefix = CStringA(m_Prefix).GetBuffer();

	//写入 主人号码
	OperateLine::line_put_ll(m_root, g_root);

	MessageBox(TEXT("主要设置 保存成功"));
	CDialogEx::OnOK();
}


void WMainConfig::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}
