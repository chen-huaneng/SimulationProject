/* External definitions for single-server queueing system. */
#include "pch.h"
#include "mm1.h"
#include "lcgrand.h" /* Header file for random-number generator. */

vector<float> mm1::mm1function(float m_l, float m_s, int n_d_r, float d_e) /* Main function. */
{
    /* Specify the number of events for the timing function. */

    num_events = 2; // 事件数

    /* Read input parameters. */

	mean_interarrival = m_l; // 平均到达时间
	mean_service = m_s; // 平均服务时间
	num_delays_required = n_d_r; //所需要服务的顾客数
	delay_excess = d_e; // 顾客延迟时间

    /* Initialize the simulation. */

    initialize();

    /* Run the simulation while more delays are still needed. */

    while (num_custs_delayed < num_delays_required)
    {
        /* Determine the next event. */

        if (timing() == 1) {
            return results;
        }

        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        switch (next_event_type)
        {
        case 1:
            if (arrive() == 2) {
                return results;
            }
            break;
        case 2:
            depart();
            break;
        }
    }

    /* Invoke the report generator and end the simulation. */

    report();

    return results;
}

void mm1::initialize(void) /* Initialization function. */
{
	/* Initialize the results vector. */

    results.clear(); // 清空结果向量

    /* Initialize the simulation clock. */

    sim_time = 0.0; // 初始化模拟时间

    /* Initialize the state variables. */

    server_status = IDLE; // 初始化服务器状态
    num_in_q = 0; // 初始化队列中的人数
    time_last_event = 0.0; // 初始化上一个事件的时间

    max_queue_length = 0; // 初始化最大队列长度
    max_delay = 0.0;    // 初始化最大延迟变量
    max_time_in_system = 0.0; // 初始化最大系统时间

    /* Initialize the statistical counters. */

    num_custs_delayed = 0; // 初始化延迟的顾客数
    total_of_delays = 0.0; // 初始化延迟总时间
    area_num_in_q = 0.0;
    area_server_status = 0.0;

    total_num_in_system = 0.0; // 初始化系统中的人数
    total_time_in_system = 0.0; // 初始化系统中的总时间
	num_custs_delayed_over_1_min = 0; // 初始化延迟超过给定时间的顾客数

    /* Initialize event list.  Since no customers are present, the departure
       (service completion) event is eliminated from consideration. */

    time_next_event[1] = sim_time + expon(mean_interarrival); // 初始化到达事件
    time_next_event[2] = 1.0e+30; // 初始化离开事件
}

int mm1::timing(void) /* Timing function. */
{
    int i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 1; i <= num_events; ++i) {
        if (time_next_event[i] < min_time_next_event)
        {
            min_time_next_event = time_next_event[i];
            next_event_type = i;
        }
    }

    /* Check to see whether the event list is empty. */

    if (next_event_type == 0)
    {
        /* The event list is empty, so stop the simulation. */

		printf("\nEvent list empty at time %f", sim_time);
		results.push_back(sim_time);
        return 1;
    }

    /* The event list is not empty, so advance the simulation clock. */

    sim_time = min_time_next_event;

	 // 更新最大系统时间
    if (next_event_type != 0 && (sim_time - time_last_event) > max_time_in_system) {
        max_time_in_system = sim_time - time_last_event;
    }
    return 0;
}

int mm1::arrive(void) /* Arrival event function. */
{
    float delay;

    /* Schedule next arrival. */

    time_next_event[1] = sim_time + expon(mean_interarrival);

    /* Check to see whether server is busy. */

    if (server_status == BUSY)
    {
        /* Server is busy, so increment number of customers in queue. */

        ++num_in_q;

        /* Update max_queue_length if needed. */

        if (num_in_q > max_queue_length) {
            max_queue_length = num_in_q;
        }

        /* Check to see whether an overflow condition exists. */

        if (num_in_q > Q_LIMIT)
        {
            /* The queue has overflowed, so stop the simulation. */

			printf("\nOverflow of the array time_arrival at");
			printf(" time %f", sim_time);
			results.push_back(sim_time);
            return 2;
        }

        /* There is still room in the queue, so store the time of arrival of the
           arriving customer at the (new) end of time_arrival. */

        time_arrival[num_in_q] = sim_time;
    } else {
        /* Server is idle, so arriving customer has a delay of zero.  (The
           following two statements are for program clarity and do not affect
           the results of the simulation.) */

        delay = 0.0;
        total_of_delays += delay;

        /* Increment the number of customers delayed, and make server busy. */

        ++num_custs_delayed;
        server_status = BUSY;

        /* Schedule a departure (service completion). */

        time_next_event[2] = sim_time + expon(mean_service);
    }
	return 0;
}

void mm1::depart(void) /* Departure event function. */
{
    int i;
    float delay;

    /* Check to see whether the queue is empty. */

    if (num_in_q == 0)
    {
        /* The queue is empty so make the server idle and eliminate the
           departure (service completion) event from consideration. */

        server_status = IDLE;
        time_next_event[2] = 1.0e+30;
    }
    else
    {
        /* The queue is nonempty, so decrement the number of customers in
           queue. */

        --num_in_q;

        /* Compute the delay of the customer who is beginning service and update
           the total delay accumulator. */

        delay = sim_time - time_arrival[1];
        total_of_delays += delay;

		// 在顾客离开时计算延迟并更新最大延迟变量
        if (delay > max_delay) {
            max_delay = delay;
        }

        // 如果顾客延迟时间超过一分钟
		if (delay > delay_excess) {
			++num_custs_delayed_over_1_min;
		}

        /* Increment the number of customers delayed, and schedule departure. */

        ++num_custs_delayed;
        time_next_event[2] = sim_time + expon(mean_service);

        /* Move each customer in queue (if any) up one place. */

        for (i = 1; i <= num_in_q; ++i) {
            time_arrival[i] = time_arrival[i + 1];
        }
    }
}

void mm1::report(void) /* Report generator function. */
{
    /* Compute and write estimates of desired measures of performance. */

	results.push_back(total_of_delays / num_custs_delayed); // 计算 time average delay of customers in queue
	results.push_back(area_num_in_q / sim_time); // 计算 average number of customers in queue
	results.push_back(area_server_status / sim_time); // 计算 average server utilization
	results.push_back(sim_time); // 记录模拟时间

    results.push_back(total_num_in_system / sim_time); // 计算系统中的平均人数
    results.push_back(total_time_in_system / num_custs_delayed); // 计算顾客在系统中的平均总时间
    results.push_back(max_queue_length); // 记录最大队列长度
    results.push_back(max_delay);    // 将最大延迟写入结果向量
    results.push_back(max_time_in_system); // 写入最大系统时间

    // 计算延迟超过给定时间的顾客比例
	float proportion_delayed_over_1_min = (float)num_custs_delayed_over_1_min / num_custs_delayed;
	results.push_back(proportion_delayed_over_1_min);
}

void mm1::update_time_avg_stats(void) /* Update area accumulators for time-average
                                    statistics. */
{
    float time_since_last_event;

    /* Compute time since last event, and update last-event-time marker. */

    time_since_last_event = sim_time - time_last_event;
    time_last_event = sim_time;

    /* Update area under number-in-queue function. */

    area_num_in_q += num_in_q * time_since_last_event;

    /* Update area under server-busy indicator function. */

    area_server_status += server_status * time_since_last_event;
    
	// 更新系统中的人数
    total_num_in_system += (num_in_q + server_status) * time_since_last_event;

	// 更新系统中的总时间
    total_time_in_system += (num_in_q + server_status) * time_since_last_event;
}

float mm1::expon(float mean) /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

    return -mean * log(lcgrand(1));
}
