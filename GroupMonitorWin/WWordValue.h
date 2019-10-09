#pragma once


// WWordValue 对话框

class WWordValue : public CDialogEx
{
	DECLARE_DYNAMIC(WWordValue)

public:
	WWordValue(CString Word ,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WWordValue();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGWORDVALUE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString Word;

public:
	CEdit m_Word;
	virtual BOOL OnInitDialog();
};
