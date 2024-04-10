// Coin.cpp: 实现文件
//

#include "pch.h"
#include "MSS.h"
#include "afxdialogex.h"
#include "Coin.h"


// Coin 对话框

IMPLEMENT_DYNAMIC(Coin, CDialogEx)

Coin::Coin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

    m_costPerRound = 0; // 初始化为 0.0，假设是double类型
    m_rewardPer = 0;    // 同上
    m_condition = 0;      // 初始化为 0，假设是int类型
    m_experiment = 0;      // 同上
}

Coin::~Coin()
{
}

void Coin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_costPerRound);
    DDX_Text(pDX, IDC_EDIT2, m_rewardPer);
    DDX_Text(pDX, IDC_EDIT3, m_condition);
    DDX_Text(pDX, IDC_EDIT4, m_experiment);
    DDX_Text(pDX, IDC_EDIT5, m_rewardVolume);
    DDX_Text(pDX, IDC_EDIT6, m_experimentVolume);
}

BEGIN_MESSAGE_MAP(Coin, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Coin::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Coin::OnBnClickedButton2)
END_MESSAGE_MAP()


// Coin 消息处理程序


void Coin::OnBnClickedButton1()
{
    UpdateData(TRUE); // 从界面控件获取数据

    // 初始化变量
    int totalFlips = 0; // 记录所有游戏中抛硬币的总次数
    int totalEarnings = 0; // 记录所有游戏的总收益

    // 模拟实验
    for (int i = 0; i < m_experiment; ++i) {
        int M = 0; // 抛n次硬币后的状态
        int n = 0; // 抛硬币的次数

        // 抛硬币直到正面和反面出现次数的差值绝对值达到条件
        while (abs(M) < m_condition) {
            // 产生一个随机值代表抛硬币的结果，0代表反面，1代表正面
            int coin = rand() % 2;
            M += (coin == 0) ? -1 : 1;
            ++n;
        }

        // 计算这次游戏的收益并累加到总收益中
        totalEarnings += (8 - n);
        // 累加抛硬币的次数
        totalFlips += n;
    }

    // 计算平均收益和平均抛硬币次数
    double averageEarnings = static_cast<double>(totalEarnings) / m_experiment;
    double averageFlips = static_cast<double>(totalFlips) / m_experiment;

    // 设置输出值
    m_rewardVolume.Format(_T("%.2f"), averageEarnings);
    m_experimentVolume.Format(_T("%.2f"), averageFlips);

    UpdateData(FALSE); // 更新界面控件显示
}


void Coin::OnBnClickedButton2()
{
    // 初始化操作
    // 清空所有输入和输出
    m_costPerRound = 0;
    m_rewardPer = 0;
    m_condition = 0;
    m_experiment = 0;
    m_rewardVolume.Empty();
    m_experimentVolume.Empty();
    UpdateData(FALSE); // 将数据同步回界面
}
