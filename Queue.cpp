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
	DDX_Control(pDX, IDC_LIST1, list);
}


BEGIN_MESSAGE_MAP(Queue, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Queue::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// Queue 消息处理程序


void Queue::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL Queue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	list.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	list.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 100);
	list.InsertColumn(2, _T("Time"), LVCFMT_LEFT, 100);
	list.InsertColumn(3, _T("Type"), LVCFMT_LEFT, 100);
	list.InsertColumn(4, _T("Status"), LVCFMT_LEFT, 100);
	list.InsertColumn(5, _T("Window"), LVCFMT_LEFT, 100);
	list.InsertItem(0, _T("1"));
	list.SetItemText(0, 1, _T("2"));
	list.InsertItem(1, _T("2"));
	list.SetItemText(1, 1, _T("3"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
