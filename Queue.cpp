// Queue.cpp: 实现文件
//

#include "pch.h"
#include "MSS.h"
#include "afxdialogex.h"
#include "Queue.h"
#include "lcgrand.h"


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
	DDX_Control(pDX, IDC_EDIT8, output);
	DDX_Control(pDX, IDC_EDIT10, openTime);
	DDX_Control(pDX, IDC_EDIT11, closeTime);
}


BEGIN_MESSAGE_MAP(Queue, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Queue::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON6, &Queue::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &Queue::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON5, &Queue::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &Queue::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &Queue::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_RADIO2, &Queue::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &Queue::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &Queue::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &Queue::OnBnClickedRadio4)
END_MESSAGE_MAP()


// Queue 消息处理程序


void Queue::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}


BOOL Queue::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 更改窗口标题
	SetWindowText(_T("Simulation of a Single-Server Queueing System"));

	// 初始化列表控件
	InitListControls(this->list);
	InitValues(this);

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

	// 获取编辑控件的指针
	CString strTemp;
	output.GetWindowText(strTemp);
	if (!strTemp.IsEmpty()) {
		// 追加新的文本
		strTemp += _T("\r\n") + seed;
	}
	else {
		strTemp = seed;
	}

	// 将新的文本设置到文本框中
	output.SetWindowText(strTemp);

	// 将CString转换为float
	int replication_int = _ttoi(replication);
	float mean_interarrival_float = _ttof(mean_interarrival);
	float mean_service_float = _ttof(mean_service);

	// 获取列表控件的行数
	rowCount = list.GetItemCount();

    // 判断哪一个单选按钮是选中的
    if (radio1State == BST_CHECKED)
    {
		CString num_delays_required;
		GetDlgItemText(IDC_EDIT3, num_delays_required);
		int num_delays_required_float = _ttoi(num_delays_required);
		
		for (int i = rowCount; i < replication_int + rowCount; i++)
		{
			vector<float> results = mm1(mean_interarrival_float, mean_service_float, num_delays_required_float);
			ShowResultsInListCtrl(list, results, i);
		}
    }
    else if (radio2State == BST_CHECKED)
    {
		printf("openTime:");
		CString replication;
		GetDlgItemText(IDC_EDIT6, replication);

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
    //CListCtrl* pListCtrl = (CListCtrl*)pParentDlg->GetDlgItem(IDC_LIST1);
    //pListCtrl->DeleteAllItems();

    // 清空编辑控件
    //pParentDlg->GetDlgItem(IDC_EDIT8)->SetWindowText(_T(""));
}

void Queue::InitListControls(CListCtrl& list) {
	// 设置列表的每列
	int col = 0;
	list.InsertColumn(col++, _T("Replication"), LVCFMT_LEFT, 50);
	//list.InsertColumn(col++, _T("Mean interarrival time"), LVCFMT_LEFT, 100);
	//list.InsertColumn(col++, _T("Mean service time"), LVCFMT_LEFT, 100);
	//list.InsertColumn(col++, _T("Number of customers"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Average delay in queue"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Average number in queue"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Service utilization"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Time simulation ended"), LVCFMT_LEFT, 100);

	list.InsertColumn(col++, _T("Time-average number in system"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Average total time in the system of customers"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Maximum queue length"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Maximum delay in queue"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Maximum time in the system"), LVCFMT_LEFT, 100);
	list.InsertColumn(col++, _T("Proportion of customers delay in queue in excess of 1 minute"), LVCFMT_LEFT, 100);
	
	// 设置列宽
	CHeaderCtrl* pHdrCtrl = list.GetHeaderCtrl();
	for (int i = 0; i < pHdrCtrl->GetItemCount(); ++i) {
		list.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}

	// 重置行数
	rowCount = 0;
}


void Queue::ShowResultsInListCtrl(CListCtrl& list, vector<float> results, int column) {
	// 插入一行
	CString col;
	col.Format(_T("%d"), column);
	list.InsertItem(column, col);

	// 设置第一列的值
	CString strReplication;
	strReplication.Format(_T("%d"), column + 1);
	list.SetItemText(column, 0, strReplication);

	// 设置其他列的值
	for (int i = 0; i < results.size(); i++) {
		CString strResult;
		strResult.Format(_T("%f"), results[i]);
		list.SetItemText(column, i + 1, strResult);
	}
}


void Queue::OnBnClickedButton5()
{
	// 清空列表控件
	list.DeleteAllItems();

	// 清空编辑控件
	SetDlgItemText(IDC_EDIT8, _T(""));
}

void Queue::InitValues(CDialogEx* pParentDlg) {
	// 初始化编辑框的值
	SetDlgItemText(IDC_EDIT1, _T("1.0"));
	SetDlgItemText(IDC_EDIT2, _T("0.5"));
	SetDlgItemText(IDC_EDIT3, _T("1000"));
	SetDlgItemText(IDC_EDIT4, _T("0"));
	SetDlgItemText(IDC_EDIT5, _T("0"));
	SetDlgItemText(IDC_EDIT6, _T("5"));
	SetDlgItemText(IDC_EDIT7, _T("1973272912"));
	SetDlgItemText(IDC_EDIT10, _T("9"));
	SetDlgItemText(IDC_EDIT11, _T("17"));
	
	seed = "1973272912"; // 默认种子值

	// 设置编辑控件为只读
	openTime.SetReadOnly(TRUE);
	closeTime.SetReadOnly(TRUE);
}


void Queue::OnBnClickedButton1()
{
	// 获取种子值
	GetDlgItemText(IDC_EDIT7, seed);
	CWnd* pEditCtrl1 = GetDlgItem(IDC_EDIT7); // 获取编辑框的指针
	long seed_long = _wtol(seed);

	// 更改种子值
	if (pEditCtrl1 != nullptr && seed_long > 0) {
		lcgrandst(seed_long, 1);
	}
}


void Queue::OnBnClickedButton3()
{
	// 清空编辑控件
	SetDlgItemText(IDC_EDIT8, _T(""));
}


void Queue::OnBnClickedRadio1()
{
	// 设置编辑控件为只读
	openTime.SetReadOnly(TRUE);
	closeTime.SetReadOnly(TRUE);
}


void Queue::OnBnClickedRadio2()
{
	// 设置编辑控件为可编辑
	openTime.SetReadOnly(FALSE);
	closeTime.SetReadOnly(FALSE);
}


void Queue::OnBnClickedRadio3()
{
	// 设置编辑控件为只读
	openTime.SetReadOnly(TRUE);
	closeTime.SetReadOnly(TRUE);
}


void Queue::OnBnClickedRadio4()
{
	// 设置编辑控件为只读
	openTime.SetReadOnly(TRUE);
	closeTime.SetReadOnly(TRUE);
}
