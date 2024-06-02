// Queue.cpp: 实现文件
//

#include "pch.h"
#include "MSS.h"
#include "afxdialogex.h"
#include "Queue.h"
#include "lcgrand.h"
#include <comdef.h> // For BSTR conversion
#include <shlobj.h> // For SHGetFolderPath
#include <atlstr.h> // For CString conversion


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
	DDX_Control(pDX, IDC_EDIT9, max_length_q);
	DDX_Control(pDX, IDC_EDIT4, length_simulation);
	DDX_Control(pDX, IDC_EDIT12, seed_by_index);
	DDX_Control(pDX, IDC_EDIT7, seed_by_number);
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
	ON_BN_CLICKED(IDC_BUTTON4, &Queue::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT11, &Queue::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT10, &Queue::OnEnChangeEdit10)
	ON_BN_CLICKED(IDC_RADIO5, &Queue::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &Queue::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_BUTTON2, &Queue::OnBnClickedButton2)
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

	// 设置默认选中第一个单选按钮
	((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);

	//// 为了方便调试，我们在这里打开控制台
	//AllocConsole();
	//// 将标准输出重定向到控制台
	//FILE *file;
	//if (freopen_s(&file, "CONOUT$", "w", stdout) != 0)
	//{
	//// freopen_s 失败的处理逻辑
	//}
	allResults.clear();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Queue::OnBnClickedButton1() // 添加种子
{
	// 获取编辑控件的样式
    LONG style = GetWindowLong(seed_by_index.GetSafeHwnd(), GWL_STYLE);

	long seed_long = 0; // 存储种子
	CWnd* pEditCtrl;

    // 检查是否包含 ES_READONLY 样式
    if (style & ES_READONLY) // 直接添加种子
    {
		// 获取种子值
		GetDlgItemText(IDC_EDIT7, seed);
		pEditCtrl = GetDlgItem(IDC_EDIT7); // 获取编辑框的指针
		seed_long = _wtol(seed);

		// 更改种子值
		if (pEditCtrl != nullptr && seed_long > 0) {
			lcgrandst(seed_long, 1);
		}
	}
    else // 通过索引添加种子
    {
		CString index;
		GetDlgItemText(IDC_EDIT12, index);

		pEditCtrl = GetDlgItem(IDC_EDIT12); // 获取编辑框的指针
		int index_num = _wtol(index);

		// 更改种子值
		if (pEditCtrl != nullptr && index_num > 0 && index_num < getlength()) {
			resetStream(); // 重置随机数流
			seed_long = lcgrandgt(index_num);
			seed.Format(_T("%ld"), seed_long); // 设置显示在编辑框中的种子值
			lcgrandst(seed_long, 1);
		}
		else {
			AfxMessageBox(_T("The index is not valid.")); // 如果索引超出了随机数流的范围，则弹出消息框
		}
    }
	previousSeed = seed_long; // 保存上一次的种子值
}


void Queue::OnBnClickedButton3() // 清空种子列表
{
	// 清空编辑控件
	SetDlgItemText(IDC_EDIT8, _T(""));
}


void Queue::OnBnClickedButton4() // 重置种子
{
	// 获取编辑控件的样式
    LONG style = GetWindowLong(seed_by_index.GetSafeHwnd(), GWL_STYLE);

    // 检查是否包含 ES_READONLY 样式
    if (style & ES_READONLY) // 直接重置种子
    {
		lcgrandst(previousSeed, 1);
    }
    else // 通过索引重置种子
    {
		seed.Format(_T("%ld"), previousSeed);
		resetStream();
		lcgrandst(previousSeed, 1);
    }
}


void Queue::OnBnClickedButton5() // 清空结果列表
{
	// 清空列表控件
	list.DeleteAllItems();

	// 清空编辑控件
	SetDlgItemText(IDC_EDIT8, _T(""));

	// 清空 allResults
	allResults.clear();
}


void Queue::OnBnClickedButton6() // 运行模拟
{
	// 暂存上一次的按钮ID
	int tmp = m_nSelectRadio;

	// 定义一个存储单选按钮 ID 的数组
	int radioButtons[] = { IDC_RADIO1, IDC_RADIO2, IDC_RADIO3, IDC_RADIO4 };

	// 定义一个存储单选按钮状态的数组
	int radioStates[4];

	// 使用循环遍历单选按钮数组
	for (int i = 0; i < 4; ++i) {
		// 获取当前单选按钮的状态
		radioStates[i] = ((CButton*)GetDlgItem(radioButtons[i]))->GetCheck();
		if (radioStates[i] == BST_CHECKED) {
			m_nSelectRadio = radioButtons[i];
		}
	}
	
	// 判断是否需要更新列表的列
	if (tmp != m_nSelectRadio) {
		// 初始化控件状态
		ClearControls(this);
		UpdateColumn(this);
	}

	// 获取编辑框中的值
	CString mean_interarrival;
	GetDlgItemText(IDC_EDIT1, mean_interarrival);
	CString mean_service;
	GetDlgItemText(IDC_EDIT2, mean_service);
	CString replication;
	GetDlgItemText(IDC_EDIT6, replication);
	CString delay;
	GetDlgItemText(IDC_EDIT5, delay);

	// 在列表中显示种子
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
	float delay_excess = _ttof(delay);

	// 更新延迟列的标题
	UpdateColumnDelay(this, delay_excess);

	// 获取列表控件的行数
	rowCount = list.GetItemCount();

	// 设置仿真参数
	int max_length_queue = INFINITY;
	bool balk_or_not = FALSE;

    // 判断哪一个单选按钮是选中的，根据相应的单选按钮调用相应的仿真程序
	switch (m_nSelectRadio) {
	case IDC_RADIO1: // Fixed Customers
	{
		CString num_delays_required;
		GetDlgItemText(IDC_EDIT3, num_delays_required);
		int num_delays_required_float = _ttoi(num_delays_required);

		mm1 m;
		for (int i = rowCount; i < replication_int + rowCount; i++) // 进行多次仿真
		{
			vector<float> results = m.mm1function(mean_interarrival_float, mean_service_float, num_delays_required_float, delay_excess);
			ShowResultsInListCtrl(list, results, i);
		}
		break;
	}
	case IDC_RADIO2: // Fixed Time
	{
		// 获取编辑框中的值
		CString open_time_value;
		GetDlgItemText(IDC_EDIT10, open_time_value);
		CString close_time_value;
		GetDlgItemText(IDC_EDIT11, close_time_value);

		// 将CString转换为float
		float time_simulation_ended = (_ttof(close_time_value) - _ttof(open_time_value)) * 60;

		mm1alt m;
		for (int i = rowCount; i < replication_int + rowCount; i++)
		{
			vector<float> results = m.mm1Alt(mean_interarrival_float, mean_service_float, time_simulation_ended, delay_excess, max_length_queue, balk_or_not);
			ShowResultsInListCtrl(list, results, i);
		}
		break;
	}
	case IDC_RADIO3: // Limited Service Time
	{
		// 获取编辑框中的值
		CString time_simulation_ended;
		GetDlgItemText(IDC_EDIT4, time_simulation_ended);

		// 将CString转换为float
		float t_e = _ttof(time_simulation_ended);

		mm1alt m;
		for (int i = rowCount; i < replication_int + rowCount; i++)
		{
			vector<float> results = m.mm1Alt(mean_interarrival_float, mean_service_float, t_e, delay_excess, max_length_queue, balk_or_not);
			ShowResultsInListCtrl(list, results, i);
		}
		break;
	}
	case IDC_RADIO4: // Limited Queue Length
	{
		// 获取编辑框中的值
		CString max_length;
		GetDlgItemText(IDC_EDIT9, max_length);
		CString open_time_value;
		GetDlgItemText(IDC_EDIT10, open_time_value);
		CString close_time_value;
		GetDlgItemText(IDC_EDIT11, close_time_value);

		// 将CString转换为float
		float time_simulation_ended = (_ttof(close_time_value) - _ttof(open_time_value)) * 60;

		// balk仿真参数
		max_length_queue = _ttoi(max_length);
		balk_or_not = TRUE;

		mm1alt m;
		for (int i = rowCount; i < replication_int + rowCount; i++)
		{
			vector<float> results = m.mm1Alt(mean_interarrival_float, mean_service_float, time_simulation_ended, delay_excess, max_length_queue, balk_or_not);
			ShowResultsInListCtrl(list, results, i);
		}
		break;
	}
	}

	// 计算均值和方差
	vector<float> means = CalculateMean(allResults);
	vector<float> variances = CalculateVariance(allResults, means);

	// 插入均值行
	ShowSpecialResultsInListCtrl(list, means, _T("mean"));

	// 插入方差行
	ShowSpecialResultsInListCtrl(list, variances, _T("variance"));
}


void Queue::OnBnClickedRadio1()
{
	// 设置编辑控件为只读
	openTime.SetReadOnly(TRUE);
	closeTime.SetReadOnly(TRUE);
	max_length_q.SetReadOnly(TRUE);
	length_simulation.SetReadOnly(TRUE);
}


void Queue::OnBnClickedRadio2()
{
	// 设置编辑控件为可编辑
	openTime.SetReadOnly(FALSE);
	closeTime.SetReadOnly(FALSE);
	max_length_q.SetReadOnly(TRUE);
	length_simulation.SetReadOnly(TRUE);
}


void Queue::OnBnClickedRadio3()
{
	// 设置编辑控件为只读
	openTime.SetReadOnly(TRUE);
	closeTime.SetReadOnly(TRUE);
	max_length_q.SetReadOnly(TRUE);
	length_simulation.SetReadOnly(FALSE);
}


void Queue::OnBnClickedRadio4()
{
	// 设置编辑控件为可编辑
	openTime.SetReadOnly(FALSE);
	closeTime.SetReadOnly(FALSE);
	max_length_q.SetReadOnly(FALSE);
	length_simulation.SetReadOnly(TRUE);
}


void Queue::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	seed_by_index.SetReadOnly(FALSE);
	seed_by_number.SetReadOnly(TRUE);
}


void Queue::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	seed_by_index.SetReadOnly(TRUE);
	seed_by_number.SetReadOnly(FALSE);
}


void Queue::OnEnChangeEdit10()
{
	CString strEdit10;
	GetDlgItemText(IDC_EDIT10, strEdit10);
	
	CString strEdit11;
	GetDlgItemText(IDC_EDIT11, strEdit11);

	float tmp = (_ttof(strEdit11) - _ttof(strEdit10)) * 60;

	CString strEdit4;
	strEdit4.Format(_T("%f"), tmp);
	SetDlgItemText(IDC_EDIT4, strEdit4);
}


void Queue::OnEnChangeEdit11()
{
	CString strEdit10;
	GetDlgItemText(IDC_EDIT10, strEdit10);
	
	CString strEdit11;
	GetDlgItemText(IDC_EDIT11, strEdit11);

	float tmp = (_ttof(strEdit11) - _ttof(strEdit10)) * 60;

	CString strEdit4;
	strEdit4.Format(_T("%f"), tmp);
	SetDlgItemText(IDC_EDIT4, strEdit4);
}


void Queue::UpdateColumnDelay(CDialogEx* pDarentDlg, float delay_excess) { // 更改列名
	int colIndex = 10; // 更改的列的索引

	CString columnName;
	columnName.Format(_T("Proportion of customers delay in queue in excess of %f minute"), delay_excess);

	CHeaderCtrl* pHdrCtrl = list.GetHeaderCtrl();

	if (pHdrCtrl) {
		// 设置列标题
		HDITEM hdItem;
		hdItem.mask = HDI_TEXT;
		hdItem.pszText = (LPTSTR)(LPCTSTR)columnName;
		pHdrCtrl->SetItem(colIndex, &hdItem);
	}
}


void Queue::UpdateColumn(CDialogEx* pParentDlg) { // 根据单选按钮的选择更新列名
	// 创建一个LVCOLUMNW结构，设置新的列名
	LVCOLUMNW column;
	column.mask = LVCF_TEXT; // 表示只设置列的文本
	switch (m_nSelectRadio) {
	case IDC_RADIO1:
		column.pszText = (LPWSTR)(LPCTSTR)_T("Time simulation ended");
		break;
	case IDC_RADIO2:
		column.pszText = (LPWSTR)(LPCTSTR)_T("Number of delays completed");
		break;
	case IDC_RADIO3:
		column.pszText = (LPWSTR)(LPCTSTR)_T("Number of customers delayed");
		break;
	case IDC_RADIO4:
		column.pszText = (LPWSTR)(LPCTSTR)_T("Number of delays completed");
		break;
	}

	// 调用SetColumn函数设置新的列名
	list.SetColumn(4, &column);
}


vector<float> Queue::CalculateMean(const vector<vector<float>>& allResults) {
	vector<float> means;
	int numItems = allResults.size();
	int numCols = allResults[0].size();

	for (int col = 0; col < numCols; ++col) {
		float sum = 0.0;
		for (int row = 0; row < numItems; ++row) {
			sum += allResults[row][col];
		}
		means.push_back(sum / numItems);
	}
	return means;
}


vector<float> Queue::CalculateVariance(const vector<vector<float>>& allResults, const vector<float>& means) {
	vector<float> variances;
	int numItems = allResults.size();
	int numCols = allResults[0].size();

	for (int col = 0; col < numCols; ++col) {
		float sum = 0.0;
		for (int row = 0; row < numItems; ++row) {
			sum += pow(allResults[row][col] - means[col], 2);
		}
		variances.push_back(sum / numItems);
	}
	return variances;
}


void Queue::ClearControls(CDialogEx* pParentDlg) // 清空控件
{
    // 清空列表控件
    CListCtrl* pListCtrl = (CListCtrl*)pParentDlg->GetDlgItem(IDC_LIST1);
    pListCtrl->DeleteAllItems();
}


void Queue::InitListControls(CListCtrl& list) {
	// 设置列表的每列
	int col = 0;
	list.InsertColumn(col++, _T("Replication"), LVCFMT_LEFT, 50);
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
	list.InsertColumn(col++, _T("Balk of customers"), LVCFMT_LEFT, 100);
	
	// 设置列宽
	CHeaderCtrl* pHdrCtrl = list.GetHeaderCtrl();
	for (int i = 0; i < pHdrCtrl->GetItemCount(); ++i) {
		list.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}

	// 重置行数
	rowCount = 0;
}


void Queue::InitValues(CDialogEx* pParentDlg) {
	// 初始化编辑框的值
	SetDlgItemText(IDC_EDIT1, _T("1.0"));
	SetDlgItemText(IDC_EDIT2, _T("0.5"));
	SetDlgItemText(IDC_EDIT3, _T("1000"));
	SetDlgItemText(IDC_EDIT4, _T("480"));
	SetDlgItemText(IDC_EDIT5, _T("1"));
	SetDlgItemText(IDC_EDIT6, _T("10"));
	SetDlgItemText(IDC_EDIT7, _T("1973272912"));
	SetDlgItemText(IDC_EDIT9, _T("2"));
	SetDlgItemText(IDC_EDIT10, _T("9"));
	SetDlgItemText(IDC_EDIT11, _T("17"));
	SetDlgItemText(IDC_EDIT12, _T("1"));
	
	seed = "1973272912"; // 默认种子值
	previousSeed = 1973272912;

	// 设置编辑控件为只读
	openTime.SetReadOnly(TRUE);
	closeTime.SetReadOnly(TRUE);
	max_length_q.SetReadOnly(TRUE);
	length_simulation.SetReadOnly(TRUE);
	seed_by_number.SetReadOnly(TRUE);
}


void Queue::ShowResultsInListCtrl(CListCtrl& list, vector<float> results, int column) { // 在列表控件中显示结果
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

	// 将结果保存到 allResults 中
	allResults.push_back(results);
}


void Queue::ShowSpecialResultsInListCtrl(CListCtrl& list, vector<float> results, CString label) {
	int column = list.GetItemCount();
	list.InsertItem(column, label);

	for (int i = 0; i < results.size(); i++) {
		CString strResult;
		strResult.Format(_T("%f"), results[i]);
		list.SetItemText(column, i + 1, strResult);
	}
}


//void Queue::WriteResultsToExcel(const std::string& filename)
//{
//    // Create a new workbook
//    lxw_workbook  *workbook  = workbook_new(filename.c_str());
//    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
//
//    // Write headers
//    std::vector<std::string> headers = {
//        "Replication", "Average delay in queue", "Average number in queue",
//        "Service utilization", "Time simulation ended", "Time-average number in system",
//        "Average total time in the system of customers", "Maximum queue length",
//        "Maximum delay in queue", "Maximum time in the system",
//        "Proportion of customers delay in queue in excess of 1 minute", "Balk of customers"
//    };
//
//    for (size_t i = 0; i < headers.size(); ++i) {
//        worksheet_write_string(worksheet, 0, i, headers[i].c_str(), NULL);
//    }
//
//    // Write data
//    for (size_t row = 0; row < allResults.size(); ++row) {
//        const std::vector<float>& results = allResults[row];
//
//        // Write replication number
//        worksheet_write_number(worksheet, row + 1, 0, row + 1, NULL);
//
//        // Write result data
//        for (size_t col = 0; col < results.size(); ++col) {
//            worksheet_write_number(worksheet, row + 1, col + 1, results[col], NULL);
//        }
//    }
//
//    // Close the workbook
//    workbook_close(workbook);
//}
//
//
//void Queue::OnBnClickedButton2()
//{
//	// 将结果写入Excel
//    WriteResultsToExcel("simulation_results.xlsx");
//}

void Queue::WriteResultsToExcel(const std::string& filename)
{
    // Create a new workbook
    lxw_workbook  *workbook  = workbook_new(filename.c_str());
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    // Write headers
    std::vector<std::string> headers = {
        "Replication", "Average delay in queue", "Average number in queue",
        "Service utilization", "Time simulation ended", "Time-average number in system",
        "Average total time in the system of customers", "Maximum queue length",
        "Maximum delay in queue", "Maximum time in the system",
        "Proportion of customers delay in queue in excess of 1 minute", "Balk of customers"
    };

    for (size_t i = 0; i < headers.size(); ++i) {
        worksheet_write_string(worksheet, 0, i, headers[i].c_str(), NULL);
    }

    // Write data
    for (size_t row = 0; row < allResults.size(); ++row) {
        const std::vector<float>& results = allResults[row];

        // Write replication number
        worksheet_write_number(worksheet, row + 1, 0, row + 1, NULL);

        // Write result data
        for (size_t col = 0; col < results.size(); ++col) {
            worksheet_write_number(worksheet, row + 1, col + 1, results[col], NULL);
        }
    }

    // Close the workbook
    workbook_close(workbook);
}


void Queue::OnBnClickedButton2()
{
    // 打开文件保存对话框
    CFileDialog fileDlg(FALSE, _T("xlsx"), _T("simulation_results.xlsx"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Excel Files (*.xlsx)|*.xlsx||"));
    if (fileDlg.DoModal() == IDOK) {
        CString filePath = fileDlg.GetPathName();
        CT2CA pszConvertedAnsiString(filePath);
        std::string strStd(pszConvertedAnsiString);

        // 将结果写入Excel
        WriteResultsToExcel(strStd);
    }
}
