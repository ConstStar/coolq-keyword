// WForwardMsg.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WForwardMsg.h"
#include "afxdialogex.h"

#include "WWordValue.h"


// WForwardMsg 对话框



IMPLEMENT_DYNAMIC(WForwardMsg, CDialogEx)

WForwardMsg::WForwardMsg(CString& word, int& frontLine, int& afterLine, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGFORWARDMSG, pParent)
	, m_frontLine(frontLine)
	, m_afterLine(afterLine)
	, m_word(word), m_temp_word(word), m_temp_frontLine(frontLine), m_temp_afterLine(afterLine)
{

}

WForwardMsg::~WForwardMsg()
{
}

void WForwardMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_frontLine);
	DDX_Text(pDX, IDC_EDIT3, m_afterLine);
	DDX_Text(pDX, IDC_EDIT1, m_word);
}


BEGIN_MESSAGE_MAP(WForwardMsg, CDialogEx)
	ON_BN_CLICKED(1, &WForwardMsg::OnBnClicked1)
	ON_BN_CLICKED(IDC_BUTTON1, &WForwardMsg::OnBnClickedButton1)
END_MESSAGE_MAP()


// WForwardMsg 消息处理程序


//保存
void WForwardMsg::OnBnClicked1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_temp_frontLine = m_frontLine;
	m_temp_afterLine = m_afterLine;
	m_temp_word = m_word;

	MessageBox(TEXT("群转发格式 保存成功!"));
	CDialogEx::OnOK();
}

//初始化
BOOL WForwardMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void WForwardMsg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString Word
	(
		"{msg}\t\t触发关键词的消息内容\r\n"
		"{日期}\t\t当前日期\t如:2019年01月01日\r\n"
		"{时间}\t\t当前时间\t如:00:00:00.000\r\n"
		"{星期}\t\t当前星期\t如:星期1\r\n"
		"{关键词}\t\t触发的关键词\t如:微信回收\r\n"
		"{触发的QQ号码}\t触发关键词的QQ号码\t如:1164442003\r\n"
		"{触发的QQ名称}\t触发关键词的QQ名称\t如:小小哥\r\n"
		"{触发的QQ名片}\t触发关键词的QQ名片\t如:定制插件\r\n"
		"{触发的群名称}\t触发关键词的群名称\t如:QQ机器人内测群\r\n"
		"\r\n\r\n\r\n更多变量欢迎补充"
	);


	WWordValue* p = new WWordValue(Word);

	p->Create(IDD_DIALOGWORDVALUE);
	p->ShowWindow(SW_SHOW);
}
