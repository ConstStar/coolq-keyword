// WREWordTest.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WREWordTest.h"
#include "afxdialogex.h"

#include <string>
#include <boost/regex.hpp>

using namespace std;

// WREWordTest 对话框

IMPLEMENT_DYNAMIC(WREWordTest, CDialogEx)

WREWordTest::WREWordTest(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGREWORDTEST, pParent)
	, m_Word(_T(""))
	, m_REWord(_T(""))
	, m_Out(_T(""))
{

}

WREWordTest::~WREWordTest()
{
}

void WREWordTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Word);
	DDX_Text(pDX, IDC_EDIT2, m_REWord);
	DDX_Text(pDX, IDC_EDIT3, m_Out);
}


BEGIN_MESSAGE_MAP(WREWordTest, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &WREWordTest::OnBnClickedButton1)
END_MESSAGE_MAP()


// WREWordTest 消息处理程序


void WREWordTest::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try
	{
		// 保护代码

		string str = CStringA(m_Word).GetBuffer();
		boost::regex re(CStringA(m_REWord).GetBuffer());
		boost::smatch REWord;

		bool rec = boost::regex_search(str, REWord, re);

		if (rec)
		{
			m_Out = "匹配成功,匹配内容为:\r\n";
			m_Out += REWord.str().c_str();

		}
		else
		{
			m_Out = "匹配失败";
		}
	}
	catch (exception & e)
	{
		m_Out = "程序崩溃，请勿使用\r\nexception 错误:";
		m_Out += e.what();
		// catch 块
	}

	UpdateData(FALSE);
}
