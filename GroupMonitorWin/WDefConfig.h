#pragma once


// WDefConfig 对话框

class WDefConfig : public CDialogEx
{
	DECLARE_DYNAMIC(WDefConfig)

public:
	WDefConfig(long long GroupId = 0, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WDefConfig();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGDEFCFG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	long long GroupId;

	afx_msg void OnBnClickedButton1();
	int m_Way;
	int m_WayTime;
	BOOL m_GroupWarn;
	BOOL m_Streng;
	BOOL m_Revoke;

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	int m_typeQQList;
};
