#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

class balk{
private:
	#define Q_LIMIT 100 /* Limit on queue length. */
	#define BUSY 1      /* Mnemonics for server's being busy */
	#define IDLE 0      /* and idle. */

	int q_limit; // ���ڶ��г��ȵ�����

	int next_event_type, num_custs_delayed, num_events, num_in_q, server_status;
	float area_num_in_q, area_server_status, mean_interarrival, mean_service,
		sim_time, time_arrival[Q_LIMIT + 1], time_end, time_last_event,
		time_next_event[4], total_of_delays;

	vector<float> results; // ���ڴ洢���

	float total_num_in_system; // �����ۻ�ϵͳ�е�����
	float total_time_in_system; // �����ۻ�ϵͳ�е���ʱ��
	int max_queue_length; // ���ڼ�¼�����г���
	float max_delay = 0.0; // ��ȫ�ֶ������������ӳٱ���
	float max_time_in_system; // ���ϵͳʱ��

	/* Initialize the count for customers delayed over 1 minute. */
	int num_custs_delayed_over_1_min = 0;

	int num_custs_balked; //���ڸ����뿪�Ĺ˿���

	float delay_excess; // ���ڸ����ӳٳ���

public:

	vector<float> mm1Balk(float m_l, float m_s, float open_time, float close_time, int max_length, float delay_excess);
	void initialize();
	int timing();
	int arrive();
	void depart();
	void report();
	void update_time_avg_stats();
	float expon(float mean);
};
