#pragma once
#include "afxdialogex.h"
#include "mm1.h"
#include <string>


// Queue 对话框

class Queue : public CDialogEx
{
	DECLARE_DYNAMIC(Queue)

public:
	Queue(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Queue();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

private:
	int m_nSelectRadio; // 用于跟踪当前选中的单选按钮
	int rowCount; // 用于跟踪列表控件的行数
	CString seed; // 用于存储种子

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl list;
	virtual BOOL OnInitDialog();
	CButton radio1;
	afx_msg void OnBnClickedButton6();
	void ClearControls(CDialogEx* pParentDlg);
	void InitListControls(CListCtrl& list);
	void ShowResultsInListCtrl(CListCtrl& list, vector<float> results, int replication);
	afx_msg void OnBnClickedButton5();
	void InitValues(CDialogEx* pParentDlg);
	CEdit output;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
};
