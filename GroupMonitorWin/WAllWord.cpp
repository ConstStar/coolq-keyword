// WAllWord.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WAllWord.h"
#include "afxdialogex.h"

#include "WREWordTest.h"
#include "WWordValue.h"

// WAllWord 对话框

IMPLEMENT_DYNAMIC(WAllWord, CDialogEx)

WAllWord::WAllWord(CString &word,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGALLWORD, pParent)
	, m_Word(word),m_temp(word)
{
	
}


WAllWord::~WAllWord()
{

}

void WAllWord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Word);
}


BEGIN_MESSAGE_MAP(WAllWord, CDialogEx)
	ON_BN_CLICKED(IDOK, &WAllWord::OnBnClickedOk)
//	ON_BN_CLICKED(1, &WAllWord::OnBnClicked1)
	ON_BN_CLICKED(IDC_BUTTON1, &WAllWord::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &WAllWord::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BUTTON4, &WAllWord::OnBnClickedButton4)
END_MESSAGE_MAP()


// WAllWord 消息处理程序

//点击保存
void WAllWord::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	m_temp = m_Word;

	MessageBox(TEXT("保存成功！"));

	CDialogEx::OnOK();
}

//初始化函数
BOOL WAllWord::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void WAllWord::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Word
	(
		"{at}\t\t艾特\t\t如:@群关键词监控\r\n"
		"{日期}\t\t当前日期\t如:2019年01月01日\r\n"
		"{时间}\t\t当前时间\t如:00:00:00.000\r\n"
		"{星期}\t\t当前星期\t如:星期1\r\n"
		"{处理方式}\t触发后处理方式\t如:禁言29天23小时59分钟\r\n"
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


void WAllWord::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	WREWordTest *p = new WREWordTest;
	p->Create(IDD_DIALOGREWORDTEST);
	p->ShowWindow(SW_SHOW);
}