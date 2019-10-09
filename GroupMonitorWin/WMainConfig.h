#pragma once


// WMainConfig 对话框

class WMainConfig : public CDialogEx
{
	DECLARE_DYNAMIC(WMainConfig)

public:
	WMainConfig(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WMainConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGMAINCFG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_root;
	BOOL m_MsgRelay;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton4();
	CString m_Prefix;
	afx_msg void OnBnClickedButton3();
};
