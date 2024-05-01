// Queue.cpp: 实现文件
//

#include "pch.h"
#include "MSS.h"
#include "afxdialogex.h"
#include "Queue.h"


// Queue 对话框

IMPLEMENT_DYNAMIC(Queue, CDialogEx)

Queue::Queue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent), m_nSelectRadio(IDC_RADIO1) //默认选中第一个单选按钮
{

}

Queue::~Queue()
{
}

void Queue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, list);
	DDX_Control(pDX, IDC_RADIO1, radio1);
}


BEGIN_MESSAGE_MAP(Queue, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Queue::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON6, &Queue::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &Queue::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON5, &Queue::OnBnClickedButton5)
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


	// 更改窗口标题
	SetWindowText(_T("Simulation of a Single-Server Queueing System"));

	// 初始化列表控件
	InitListControls(this->list);

	// 设置默认选中第一个单选按钮
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

	// 为了方便调试，我们在这里打开控制台
	AllocConsole();
	// 将标准输出重定向到控制台
	FILE *file;
	if (freopen_s(&file, "CONOUT$", "w", stdout) != 0)
	{
	// freopen_s 失败的处理逻辑
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Queue::OnBnClickedButton6()
{
	// 初始化控件状态
	ClearControls(this);
	// 获取每个单选按钮的状态
    int radio1State = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
    int radio2State = ((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck();
    int radio3State = ((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck();
    int radio4State = ((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck();

	// 获取编辑框中的值
	CString mean_interarrival;
	GetDlgItemText(IDC_EDIT1, mean_interarrival);
	CString mean_service;
	GetDlgItemText(IDC_EDIT2, mean_service);
	CString replication;
	GetDlgItemText(IDC_EDIT6, replication);

	// 将CString转换为float
	int replication_int = _ttoi(replication);
	float mean_interarrival_float = _ttof(mean_interarrival);
	float mean_service_float = _ttof(mean_service);

    // 判断哪一个单选按钮是选中的
    if (radio1State == BST_CHECKED)
    {
		CString num_delays_required;
		GetDlgItemText(IDC_EDIT3, num_delays_required);
		int num_delays_required_float = _ttoi(num_delays_required);
		
		for (int i = 0; i < replication_int; i++)
		{
			vector<float> results = mm1(mean_interarrival_float, mean_service_float, num_delays_required_float);
			ShowResultsInListCtrl(list, results, i);
		}
    }
    else if (radio2State == BST_CHECKED)
    {
    }
    else if (radio3State == BST_CHECKED)
    {
    }
    else if (radio4State == BST_CHECKED)
    {
    }
}


void Queue::ClearControls(CDialogEx* pParentDlg)
{
    // 清空列表控件
    CListCtrl* pListCtrl = (CListCtrl*)pParentDlg->GetDlgItem(IDC_LIST1);
    pListCtrl->DeleteAllItems();

    // 清空编辑控件
    pParentDlg->GetDlgItem(IDC_EDIT8)->SetWindowText(_T(""));
}

void Queue::InitListControls(CListCtrl& list) {
	list.InsertColumn(0, _T("Replication"), LVCFMT_LEFT, 50);
	list.InsertColumn(1, _T("Mean interarrival time"), LVCFMT_LEFT, 100);
	list.InsertColumn(2, _T("Mean service time"), LVCFMT_LEFT, 100);
	list.InsertColumn(3, _T("Number of customers"), LVCFMT_LEFT, 100);
	list.InsertColumn(4, _T("Average delay in queue"), LVCFMT_LEFT, 100);
	list.InsertColumn(5, _T("Average number	in queue"), LVCFMT_LEFT, 100);
	list.InsertColumn(6, _T("Service utilization"), LVCFMT_LEFT, 100);
	list.InsertColumn(7, _T("Time simulation ended"), LVCFMT_LEFT, 100);
}

void Queue::ShowResultsInListCtrl(CListCtrl& list, vector<float> results, int column) {
	for (int i = 0; i < results.size(); i++) {
		cout << results[i] << endl;
	}
	CString col;
	col.Format(_T("%d"), column);
	list.InsertItem(column, col);
	CString strReplication;
	strReplication.Format(_T("%d"), column + 1);
	list.SetItemText(column, 0, strReplication);
	for (int i = 1; i < results.size(); i++) {
		CString strResult;
		strResult.Format(_T("%f"), results[i]);
		list.SetItemText(column, i, strResult);
	}
}


void Queue::OnBnClickedButton5()
{
	// 清空列表控件
	list.DeleteAllItems();

	// 清空编辑控件
	SetDlgItemText(IDC_EDIT8, _T(""));

}
