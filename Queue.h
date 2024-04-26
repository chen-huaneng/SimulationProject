#pragma once
#include "afxdialogex.h"


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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
