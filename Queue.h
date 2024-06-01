#pragma once
#include "afxdialogex.h"
#include "mm1.h"
#include "mm1alt.h"
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
	vector<float> CalculateMean(const vector<vector<float>>& allResults);
	vector<float> CalculateVariance(const vector<vector<float>>& allResults, const vector<float>& means);
	void Queue::ShowSpecialResultsInListCtrl(CListCtrl& list, vector<float> results, CString label);
	vector<vector<float>> allResults;
	CString seed; // 用于存储种子
	CEdit output;
	CButton radio1;
	CListCtrl list;
	CEdit openTime;
	CEdit closeTime;
	CEdit max_length_q;
	CEdit length_simulation;
	CEdit seed_by_index;
	CEdit seed_by_number;
	long previousSeed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton6();
	void ClearControls(CDialogEx* pParentDlg);
	void InitListControls(CListCtrl& list);
	void ShowResultsInListCtrl(CListCtrl& list, vector<float> results, int replication);
	void UpdateColumn(CDialogEx* pParentDlg);
	void UpdateColumnDelay(CDialogEx* pParentDlg, float delay_excess);
	afx_msg void OnBnClickedButton5();
	void InitValues(CDialogEx* pParentDlg);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedButton4();

	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
};
