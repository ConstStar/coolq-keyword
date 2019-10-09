// WWordValue.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WWordValue.h"
#include "afxdialogex.h"


// WWordValue 对话框

IMPLEMENT_DYNAMIC(WWordValue, CDialogEx)

WWordValue::WWordValue(CString Word, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGWORDVALUE, pParent)
{
	this->Word = Word;
}

WWordValue::~WWordValue()
{
}

void WWordValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Word);
}


BEGIN_MESSAGE_MAP(WWordValue, CDialogEx)
END_MESSAGE_MAP()


// WWordValue 消息处理程序


BOOL WWordValue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Word.SetWindowTextW(Word);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
