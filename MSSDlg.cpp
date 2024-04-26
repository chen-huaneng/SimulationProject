
// MSSDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MSS.h"
#include "MSSDlg.h"
#include "afxdialogex.h"
#include "Coin.h"
#include "Queue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMSSDlg 对话框



CMSSDlg::CMSSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MSS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMSSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, Coin_Button);
	DDX_Control(pDX, IDC_BUTTON2, Queue_Button);
	DDX_Control(pDX, IDC_BUTTON3, Inventory);
}

BEGIN_MESSAGE_MAP(CMSSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMSSDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMSSDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMSSDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMSSDlg 消息处理程序

BOOL CMSSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	SetWindowText(_T("陈华能 17420212200999  张正贤 17420212201039"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 设置主窗口的按钮文本
void CMSSDlg::SetButtonText()
{
	// 获取按钮对象指针
	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON1);
	// 检查指针是否有效
	if (pButton != nullptr) {
		 // 设置按钮文本
		pButton->SetWindowText(_T("Coin Flipping"));
	}

	// 获取按钮对象指针
	pButton = (CButton*)GetDlgItem(IDC_BUTTON2);
	// 检查指针是否有效
	if (pButton != nullptr) {
		 // 设置按钮文本
		pButton->SetWindowText(_T("Queue"));
	}

	// 获取按钮对象指针
	pButton = (CButton*)GetDlgItem(IDC_BUTTON3);
	// 检查指针是否有效
	if (pButton != nullptr) {
		 // 设置按钮文本
		pButton->SetWindowText(_T("Inventory"));
	}
}

void CMSSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMSSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMSSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMSSDlg::OnBnClickedButton1() {
	// TODO: 在此添加控件通知处理程序代码
	Coin c;
	if (c.DoModal() == IDOK) {
		// 设置对话框的文本
		c.SetWindowText(_T("Coin Flipping Game"));
	}
	

}


void CMSSDlg::OnBnClickedButton3() {
	// TODO: 在此添加控件通知处理程序代码
}


void CMSSDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	Queue q;
	if (q.DoModal() == IDOK) {
		// 设置对话框的文本
		q.SetWindowText(_T("Queue"));
	}
}
