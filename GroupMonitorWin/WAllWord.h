#pragma once


// WAllWord 对话框

class WAllWord : public CDialogEx
{
	DECLARE_DYNAMIC(WAllWord)

public:
	WAllWord(CString &word, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WAllWord();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGALLWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_Word;
	CString &m_temp;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClicked1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
