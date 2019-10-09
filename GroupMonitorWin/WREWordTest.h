#pragma once


// WREWordTest 对话框

class WREWordTest : public CDialogEx
{
	DECLARE_DYNAMIC(WREWordTest)

public:
	WREWordTest(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WREWordTest();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGREWORDTEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Word;
	CString m_REWord;
	afx_msg void OnBnClickedButton1();
	CString m_Out;
};
