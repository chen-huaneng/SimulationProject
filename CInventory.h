#pragma once
#include "afxdialogex.h"


// CInventory 对话框

class CInventory : public CDialogEx
{
	DECLARE_DYNAMIC(CInventory)

public:
	CInventory(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInventory();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	float m_initial_inv_level;
	int m_num_months;
	float m_mean_interdemand;
	float m_setup_cost;
	float m_increment_cost;
	float m_holding_cost;
	float m_shortage_cost;
	float m_minlag;
	float m_maxlag;
	CListCtrl m_inv_list;
	void report(int j);
	int m_inv_seed;
	int m_inv_rep;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	BOOL m_inv_option;
	void inv_report_mean(int j);
	float m_exp_setup_cost;
	float m_exp_incremental_cost;
	float m_exp_minlag;
	float m_exp_maxlag;
	float m_per_minlife;
	float m_per_maxlife;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit16();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
};
