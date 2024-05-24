// CInventory.cpp: 实现文件
//

#include "pch.h"
#include "MSS.h"
#include "afxdialogex.h"
#include "CInventory.h"
#include "lcgrand.h"

#include <stdio.h>
#include <math.h>
#include <cstdlib>

int   amount, bigs, initial_inv_level, inv_level, next_event_type, num_events,
num_months, num_values_demand, smalls, inv_seeds[5], inv_flag, shortage_order, k, discard_amount, total_amount;

int s[9] = { 20,20,20,20,40,40,40,60,60 }, S[9] = { 40,60,80,100,60,80,100,80,100 };
int order[120];
float order_arrival_time[120], order_life[120];
float prob_distrib_demand[5] = { 0,0.16667,0.5,0.8333333,1.0 };

float area_holding, area_shortage, holding_cost, incremental_cost, maxlag,
mean_interdemand, minlag, setup_cost, time_shortage,
shortage_cost, sim_time, time_last_event, time_next_event[5],
total_ordering_cost,
exp_setup_cost, exp_incremental_cost, exp_maxlag, exp_minlag, per_minlife, per_maxlife;
CString inv_str;

int inv_j = 0;
float inv_mean;
float inv_variance;
int inv_seed;
int inv_rep;

void  inv_initialize(int inv_s);
void  inv_timing(void);
void  inv_order_arrival(int inv_s);
void  inv_demand(int inv_s);
void  inv_evaluate(int inv_s);
void  check_perishable(void);

void  inv_update_time_avg_stats(void);
float inv_expon(float mean, int inv_s);
int   inv_random_integer(float prob_distrib[], int inv_s);
float inv_uniform(float a, float b, int inv_s);
int temp = 0;

void inv_clear_seed(int seed[]);
float inv_cal_mean(float arr[], int len);
float inv_cal_std(float arr[], int len);

// CInventory 对话框

IMPLEMENT_DYNAMIC(CInventory, CDialogEx)

CInventory::CInventory(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_initial_inv_level(60)
	, m_num_months(120)
	, m_mean_interdemand(0.1)
	, m_setup_cost(32)
	, m_increment_cost(3)
	, m_holding_cost(1)
	, m_shortage_cost(5)
	, m_minlag(0.5)
	, m_maxlag(1)
	, m_inv_seed(0)
	, m_inv_rep(0)
	, m_inv_option(FALSE)
	, m_exp_setup_cost(48)
	, m_exp_incremental_cost(4)
	, m_exp_minlag(0.25)
	, m_exp_maxlag(0.5)
	, m_per_minlife(1.5)
	, m_per_maxlife(2.5)
{

}

CInventory::~CInventory()
{
}

void CInventory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_initial_inv_level);
	DDX_Text(pDX, IDC_EDIT2, m_num_months);
	DDX_Text(pDX, IDC_EDIT5, m_mean_interdemand);
	DDX_Text(pDX, IDC_EDIT6, m_setup_cost);
	DDX_Text(pDX, IDC_EDIT7, m_increment_cost);
	DDX_Text(pDX, IDC_EDIT8, m_holding_cost);
	DDX_Text(pDX, IDC_EDIT9, m_shortage_cost);
	DDX_Text(pDX, IDC_EDIT10, m_minlag);
	DDX_Text(pDX, IDC_EDIT11, m_maxlag);
	DDX_Control(pDX, IDC_LIST1, m_inv_list);
	DDX_Text(pDX, IDC_EDIT3, m_inv_seed);
	DDX_Text(pDX, IDC_EDIT4, m_inv_rep);
	DDX_Radio(pDX, IDC_RADIO1, m_inv_option);
	DDX_Text(pDX, IDC_EDIT12, m_exp_setup_cost);
	DDX_Text(pDX, IDC_EDIT13, m_exp_incremental_cost);
	DDX_Text(pDX, IDC_EDIT14, m_exp_minlag);
	DDX_Text(pDX, IDC_EDIT15, m_exp_maxlag);
	DDX_Text(pDX, IDC_EDIT16, m_per_minlife);
	DDX_Text(pDX, IDC_EDIT17, m_per_maxlife);
}


BEGIN_MESSAGE_MAP(CInventory, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CInventory::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CInventory::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CInventory::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CInventory::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO2, &CInventory::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CInventory::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO1, &CInventory::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON5, &CInventory::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT2, &CInventory::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT5, &CInventory::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT11, &CInventory::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT16, &CInventory::OnEnChangeEdit16)
	ON_LBN_SELCHANGE(IDC_LIST1, &CInventory::OnLbnSelchangeList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CInventory::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CInventory 消息处理程序


BOOL CInventory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_inv_list.InsertColumn(0, _T("Simulation"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(1, _T("Seed"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(2, _T("Small s"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(3, _T("Big S"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(4, _T("Average ordering cost"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(5, _T("Average holding cost"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(6, _T("Average shortage cost"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(7, _T("Total cost"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(8, _T("Proportion of shortage time"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(9, _T("Number of express orders placed"), LVCFMT_CENTER, 80);
	m_inv_list.InsertColumn(10, _T("Proportion of discarded amount"), LVCFMT_CENTER, 80);


	return TRUE;  // return TRUE unless you set the focus to a control

}

void CInventory::OnBnClickedButton1()
{
	UpdateData(1);
	// TODO: 在此添加控件通知处理程序代码
	int i;

	/* Specify the number of events for the timing function. */

	num_events = 4;

	initial_inv_level = m_initial_inv_level;
	num_months = m_num_months;
	mean_interdemand = m_mean_interdemand;
	setup_cost = m_setup_cost;
	incremental_cost = m_increment_cost;
	holding_cost = m_holding_cost;
	shortage_cost = m_shortage_cost;
	minlag = m_minlag;
	maxlag = m_maxlag;
	exp_setup_cost = m_exp_setup_cost;
	exp_incremental_cost = m_exp_incremental_cost;
	exp_maxlag = m_exp_maxlag;
	exp_minlag = m_exp_minlag;
	per_maxlife = m_per_maxlife;
	per_minlife = m_per_minlife;

	inv_rep = m_inv_rep;

	/* Run the simulation varying the inventory policy. */

	switch (m_inv_option) {
	case 0:
		inv_flag = 0;
		break;
	case 1:
		inv_flag = 1;
		break;
	case 2:
		inv_flag = 2;
		break;
	}

	for (int h = 0; h < m_inv_rep; h++)
	{
		inv_seed = inv_seeds[h];
		for (i = 0; i <= 8; ++i) {

			/* Read the inventory policy, and initialize the simulation. */

			/*fscanf(infile, "%d %d", &smalls, &bigs);*/
			smalls = s[i];
			bigs = S[i];
			inv_initialize(inv_seed);

			/* Run the simulation until it terminates after an end-simulation event
			(type 3) occurs. */
			do {

				/* Determine the next event. */

				inv_timing();

				/* Update time-average statistical accumulators. */

				inv_update_time_avg_stats();

				/* Invoke the appropriate event function. */

				switch (next_event_type) {
				case 1:
					inv_order_arrival(inv_seed);
					break;
				case 2:
					inv_demand(inv_seed);
					break;
				case 4:
					inv_evaluate(inv_seed);
					break;
				case 3:
					report(m_inv_rep + i - 1 - temp);
					break;
				}

				/* If the event just executed was not the end-simulation event (type 3),
				continue simulating.  Otherwise, end the simulation for the current
				(s,S) pair and go on to the next pair (if any). */

			} while (next_event_type != 3);
		}
	}



	temp++;
	/* End the simulations. */



}

void inv_initialize(int inv_s)  /* Initialization function. */
{
	/* Initialize the simulation clock. */

	sim_time = 0.0;

	/* Initialize the state variables. */

	inv_level = initial_inv_level;
	time_last_event = 0.0;

	/* Initialize the statistical counters. */

	total_ordering_cost = 0.0;
	area_holding = 0.0;
	area_shortage = 0.0;
	time_shortage = 0.0;
	shortage_order = 0;
	k = 0;
	discard_amount = 0;
	total_amount = initial_inv_level;

	/* Initialize the event list.  Since no order is outstanding, the order-
	arrival event is eliminated from consideration. */

	time_next_event[1] = 1.0e+30;
	time_next_event[2] = sim_time + inv_expon(mean_interdemand, inv_s);
	time_next_event[3] = num_months;
	time_next_event[4] = 0.0;
}


void inv_timing(void)  /* Timing function. */
{
	int   i;
	float min_time_next_event = 1.0e+29;

	next_event_type = 0;

	/* Determine the event type of the next event to occur. */

	for (i = 1; i <= num_events; ++i)
		if (time_next_event[i] < min_time_next_event) {
			min_time_next_event = time_next_event[i];
			next_event_type = i;
		}

	/* Check to see whether the event list is empty. */

	if (next_event_type == 0) {

		/* The event list is empty, so stop the simulation */

		/*  fprintf(outfile, "\nEvent list empty at time %f", sim_time);*/
		exit(1);
	}

	/* The event list is not empty, so advance the simulation clock. */

	sim_time = min_time_next_event;
}


void inv_order_arrival(int inv_s)  /* Order arrival event function. */
{
	/* Increment the inventory level by the amount ordered. */

	inv_level += amount;

	/* Since no order is now outstanding, eliminate the order-arrival event from
	consideration. */

	if (inv_flag == 2) {
		float life;
		life = inv_uniform(per_minlife, per_maxlife, inv_s);
		order_life[k] = life;
		order_arrival_time[k] = sim_time;
		k++;

	}

	time_next_event[1] = 1.0e+30;
}


void inv_demand(int inv_s)  /* Demand event function. */
{
	int random_demand = inv_random_integer(prob_distrib_demand, inv_s);
	int sum_order_amount = 0;
	int i = 0;
	/* Decrement the inventory level by a generated demand size. */

	inv_level -= random_demand;

	if (inv_flag == 2) {
		check_perishable();

		for (i; sum_order_amount < random_demand && i < k; i++) {
			sum_order_amount += order[i];
		}
		for (int j = 0; j < i - 1; j++) {    //将前i-1个记录覆盖
			for (int p = 0; p < k; p++) {
				order[p] = order[p + 1];
				order_arrival_time[p] = order_arrival_time[p + 1];
				order_life[p] = order_life[p + 1];
			}
			k--;
		}

		if (random_demand - sum_order_amount == 0) { //将第i个记录覆盖
			order[0] = order[1];
			order_arrival_time[0] = order_arrival_time[1];
			order_life[0] = order_life[1];
			k--;
		}
		else if (sum_order_amount > random_demand) {
			order[0] = sum_order_amount - random_demand;
		}
	}


	/* Schedule the time of the next demand. */

	time_next_event[2] = sim_time + inv_expon(mean_interdemand, inv_s);
}


void inv_evaluate(int inv_s)  /* Inventory-evaluation event function. */
{
	/* Check whether the inventory level is less than smalls. */

	if (inv_level < 0 && inv_flag == 1) {
		amount = bigs - inv_level;
		total_ordering_cost += exp_setup_cost + exp_incremental_cost * amount;
		time_next_event[1] = sim_time + inv_uniform(exp_minlag, exp_maxlag, inv_s);
		shortage_order++;

	}
	else if (inv_level < smalls) {

		/* The inventory level is less than smalls, so place an order for the
		appropriate amount. */

		amount = bigs - inv_level;
		total_ordering_cost += setup_cost + incremental_cost * amount;

		/* Schedule the arrival of the order. */

		time_next_event[1] = sim_time + inv_uniform(minlag, maxlag, inv_s);

	}

	/* Regardless of the place-order decision, schedule the next inventory
	evaluation. */

	order[k] = amount;
	total_amount += amount;
	time_next_event[4] = sim_time + 1.0;
}

void inv_update_time_avg_stats(void)  /* Update area accumulators for time-average
									  statistics. */
{
	float time_since_last_event;

	/* Compute time since last event, and update last-event-time marker. */

	time_since_last_event = sim_time - time_last_event;
	time_last_event = sim_time;

	/* Determine the status of the inventory level during the previous interval.
	If the inventory level during the previous interval was negative, update
	area_shortage.  If it was positive, update area_holding.  If it was zero,
	no update is needed. */

	if (inv_level < 0) {
		area_shortage -= inv_level * time_since_last_event;
		time_shortage += time_since_last_event;
	}
	else if (inv_level > 0)
		area_holding += inv_level * time_since_last_event;
}

float inv_expon(float mean, int inv_s)  /* Exponential variate generation function. */
{
	/* Return an exponential random variate with mean "mean". */

	return -mean * log(lcgrand(inv_s));
}

int inv_random_integer(float prob_distrib[], int inv_s)  /* Random integer generation
														 function. */
{
	int   i;
	float u;

	/* Generate a U(0,1) random variate. */

	u = lcgrand(inv_s);

	/* Return a random integer in accordance with the (cumulative) distribution
	function prob_distrib. */

	for (i = 1; u >= prob_distrib[i]; ++i)
		;
	return i;
}


float inv_uniform(float a, float b, int inv_s)  /* Uniform variate generation function. */
{
	/* Return a U(a,b) random variate. */

	return a + lcgrand(inv_s) * (b - a);
}



void CInventory::report(int j)
{

	float avg_holding_cost, avg_ordering_cost, avg_shortage_cost;
	avg_holding_cost = total_ordering_cost / num_months;
	avg_ordering_cost = holding_cost * area_holding / num_months;
	avg_shortage_cost = shortage_cost * area_shortage / num_months;
	float proportion_shortage;
	proportion_shortage = 1.0 * time_shortage / num_months;

	//m_inv_list.InsertItem(inv_j,_T("1"));

	inv_str.Format(TEXT("%d"), smalls);
	m_inv_list.SetItemText(j, 2, inv_str);

	inv_str.Format(TEXT("%d"), bigs);
	m_inv_list.SetItemText(j, 3, inv_str);

	inv_str.Format(TEXT("%.4f"), total_ordering_cost / num_months);
	m_inv_list.SetItemText(j, 4, inv_str);

	inv_str.Format(TEXT("%.4f"), holding_cost * area_holding / num_months);
	m_inv_list.SetItemText(j, 5, inv_str);

	inv_str.Format(TEXT("%.4f"), shortage_cost * area_shortage / num_months);
	m_inv_list.SetItemText(j, 6, inv_str);

	inv_str.Format(TEXT("%.4f"), avg_holding_cost + avg_ordering_cost + avg_shortage_cost);
	m_inv_list.SetItemText(j, 7, inv_str);

	inv_str.Format(TEXT("%.4f"), proportion_shortage);
	m_inv_list.SetItemText(j, 8, inv_str);

	inv_str.Format(TEXT("%d"), shortage_order);
	m_inv_list.SetItemText(j, 9, inv_str);

	inv_str.Format(TEXT("%.4f"), 1.0 * discard_amount / total_amount);
	m_inv_list.SetItemText(j, 10, inv_str);

}


void CInventory::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < 101; i++)
	{
		static long zrng[] =
		{ 1,
		1973272912, 281629770,  20006270,1280689831,2096730329,1933576050,
		913566091, 246780520,1363774876, 604901985,1511192140,1259851944,
		824064364, 150493284, 242708531,  75253171,1964472944,1202299975,
		233217322,1911216000, 726370533, 403498145, 993232223,1103205531,
		762430696,1922803170,1385516923,  76271663, 413682397, 726466604,
		336157058,1432650381,1120463904, 595778810, 877722890,1046574445,
		68911991,2088367019, 748545416, 622401386,2122378830, 640690903,
		1774806513,2132545692,2079249579,  78130110, 852776735,1187867272,
		1351423507,1645973084,1997049139, 922510944,2045512870, 898585771,
		243649545,1004818771, 773686062, 403188473, 372279877,1901633463,
		498067494,2087759558, 493157915, 597104727,1530940798,1814496276,
		536444882,1663153658, 855503735,  67784357,1432404475, 619691088,
		119025595, 880802310, 176192644,1116780070, 277854671,1366580350,
		1142483975,2026948561,1053920743, 786262391,1792203830,1494667770,
		1923011392,1433700034,1244184613,1147297105, 539712780,1545929719,
		190641742,1645390429, 264907697, 620389253,1502074852, 927711160,
		364849192,2049576050, 638580085, 547070247 };
		lcgrandst(zrng[i], i);
	}
}




void CInventory::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(0, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(0, 1, inv_str);

	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(1, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(1, 1, inv_str);

	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(2, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(2, 1, inv_str);

	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(3, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(3, 1, inv_str);

	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(4, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(4, 1, inv_str);

	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(5, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(5, 1, inv_str);

	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(6, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(6, 1, inv_str);

	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(7, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(7, 1, inv_str);

	inv_str.Format(TEXT("%d"), inv_j + 1);
	m_inv_list.InsertItem(8, inv_str);
	inv_str.Format(TEXT("%d"), m_inv_seed);
	m_inv_list.SetItemText(8, 1, inv_str);

	inv_seeds[inv_j] = m_inv_seed;
	inv_j++;
	m_inv_rep = inv_j;
	UpdateData(0);
}


void CInventory::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_inv_list.DeleteAllItems();
	inv_j = 0;
	m_inv_seed = 0;
	m_inv_rep = 0;
	temp = 0;
	inv_clear_seed(inv_seeds);
	UpdateData(0);
}


void check_perishable(void) {

	for (int p = 0; p < k; p++)
	{
		if (sim_time - order_arrival_time[p] > order_life[p]) {

			inv_level -= order[p];

			discard_amount += order[p];

			for (int i = 0; i < k; i++) {
				order[i] = order[i + 1];
				order_arrival_time[i] = order_arrival_time[i + 1];
				order_life[i] = order_life[i + 1];
			}
			k--;
		}
	}
}




float inv_cal_std(float arr[], int len) {

	float sum = 0;
	float avg = 0;
	inv_variance = 0;

	for (int i = 0; i < len; i++)
	{

		sum += arr[i];
	}


	avg = sum * 1.0 / len;

	for (int i = 0; i < len; i++)
	{
		inv_variance += pow((arr[i] - avg), 2);
	}

	inv_variance = inv_variance / len;

	return inv_variance;
}


void inv_clear_arr(float arr[]) {

	for (int i = 0; i < 100; i++)
	{
		arr[i] = 0.0;
	}

}

void CInventory::inv_report_mean(int j)
{
}


void inv_clear_seed(int seed[]) {

	for (int i = 0; i < 10; i++)
	{
		seed[i] = 0.0;
	}

}

void CInventory::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_inv_list.DeleteAllItems();
	inv_j = 0;
	m_inv_seed = 0;
	m_inv_rep = 0;
	temp = 0;
	inv_clear_seed(inv_seeds);

	(CButton*)GetDlgItem(IDC_EDIT12)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_EDIT13)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_EDIT14)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_EDIT15)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_EDIT16)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT17)->EnableWindow(FALSE);

}


void CInventory::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_inv_list.DeleteAllItems();
	inv_j = 0;
	m_inv_seed = 0;
	m_inv_rep = 0;
	temp = 0;
	inv_clear_seed(inv_seeds);
	(CButton*)GetDlgItem(IDC_EDIT12)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT13)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT14)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT15)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT16)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_EDIT17)->EnableWindow(TRUE);
}


void CInventory::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_inv_list.DeleteAllItems();
	inv_j = 0;
	m_inv_seed = 0;
	m_inv_rep = 0;
	temp = 0;
	inv_clear_seed(inv_seeds);
	(CButton*)GetDlgItem(IDC_EDIT12)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT13)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT14)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT15)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT16)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_EDIT17)->EnableWindow(FALSE);
}


void CInventory::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_inv_seed = 0;
	m_inv_rep = 0;
	inv_j = 0;
	inv_clear_seed(inv_seeds);

	UpdateData(0);
}


void CInventory::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CInventory::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CInventory::OnEnChangeEdit11()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CInventory::OnEnChangeEdit16()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CInventory::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CInventory::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
