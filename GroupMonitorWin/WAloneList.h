#pragma once


// WAloneList 对话框

class WAloneList : public CDialogEx
{
	DECLARE_DYNAMIC(WAloneList)

public:
	WAloneList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WAloneList();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGALONELIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void InitData();


	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLvnOdstatechangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	long long m_GroupId;
	afx_msg void OnBnClickedButton9();
};
