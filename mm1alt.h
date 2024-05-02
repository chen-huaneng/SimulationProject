#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "mm1.h"

using namespace std;

class mm1alt : public mm1{
private:
	#define Q_LIMIT 100 /* Limit on queue length. */
	#define BUSY 1      /* Mnemonics for server's being busy */
	#define IDLE 0      /* and idle. */
	#define ARRAY_SIZE 10 /* Size of array to store statistics. */

	int next_event_type, num_custs_delayed, num_delays_required, num_events,
		num_in_q, server_status;
	float area_num_in_q, area_server_status, mean_interarrival, mean_service,
		sim_time, time_arrival[Q_LIMIT + 1], time_last_event, time_next_event[3],
		total_of_delays;

	vector<float> results; // ���ڴ洢���

	float total_num_in_system; // �����ۻ�ϵͳ�е�����
	float total_time_in_system; // �����ۻ�ϵͳ�е���ʱ��
	int max_queue_length; // ���ڼ�¼�����г���
	float max_delay = 0.0; // ��ȫ�ֶ������������ӳٱ���
	float max_time_in_system; // ���ϵͳʱ��

	/* Initialize the count for customers delayed over 1 minute. */
	int num_custs_delayed_over_1_min = 0;

public:

	vector<float> mm1Alt(float m_l, float m_s, int n_d_r, float open_time, float close_time);
	void initializeAlt(float open_time, float close_time);
	int timing();
	int arrive();
	void depart();
	void report();
	void update_time_avg_stats();
};
