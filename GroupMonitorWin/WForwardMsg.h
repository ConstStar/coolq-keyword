#pragma once


// WForwardMsg 对话框

class WForwardMsg : public CDialogEx
{
	DECLARE_DYNAMIC(WForwardMsg)

public:
	WForwardMsg(CString &word, int &frontLine, int &afterLine, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WForwardMsg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGFORWARDMSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked1();

public:
	CString &m_temp_word;
	int &m_temp_frontLine;
	int &m_temp_afterLine;

	int m_frontLine;
	int m_afterLine;
	CString m_word;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
