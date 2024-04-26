// Queue.cpp: 实现文件
//

#include "pch.h"
#include "MSS.h"
#include "afxdialogex.h"
#include "Queue.h"


// Queue 对话框

IMPLEMENT_DYNAMIC(Queue, CDialogEx)

Queue::Queue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

Queue::~Queue()
{
}

void Queue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Queue, CDialogEx)
END_MESSAGE_MAP()


// Queue 消息处理程序
