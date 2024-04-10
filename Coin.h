#pragma once
#include "afxdialogex.h"


// Coin 对话框

class Coin : public CDialogEx
{
	DECLARE_DYNAMIC(Coin)

public:
	Coin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Coin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    // 编辑框变量
    double m_costPerRound;
    double m_rewardPer;
    int m_condition;
    int m_experiment;

    // 输出变量（用CString便于显示）
    CString m_rewardVolume;
    CString m_experimentVolume;

    // 按钮事件处理函数
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
