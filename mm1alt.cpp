/* External definitions for single-server queueing system. */
#include "pch.h"
#include "mm1alt.h"
#include "lcgrand.h" /* Header file for random-number generator. */

vector<float> mm1alt::mm1Alt(float m_l, float m_s, float open_time, float close_time) /* Main function. */
{
    /* Specify the number of events for the timing function. */
    
    num_events = 3;

    /* Read input parameters. */

	mean_interarrival = m_l;
	mean_service = m_s;
    time_end = (close_time - open_time) * 60;

    /* Initialize the simulation. */

    initialize();

    /* Run the simulation until it terminates after an end-simulation event
       (type 3) occurs. */

    do
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
        case 3:
            report();
            break;
		}
        /* If the event just executed was not the end-simulation event (type 3),
           continue simulating.  Otherwise, end the simulation. */
	} while (next_event_type != 3);

    return results;
}

void mm1alt::initialize() /* Initialization function. */
{
	/* Initialize the results vector. */

    results.clear();

    /* Initialize the simulation clock. */

    sim_time = 0.0;

    /* Initialize the state variables. */

    server_status = IDLE;
    num_in_q = 0;
    time_last_event = 0.0;

	// 初始化系统中的人数和总时间

    max_queue_length = 0; // 初始化最大队列长度
    max_delay = 0.0;    // 初始化最大延迟变量
    max_time_in_system = 0.0; // 初始化最大系统时间

    /* Initialize the statistical counters. */

    num_custs_delayed = 0;
    total_of_delays = 0.0;
    area_num_in_q = 0.0;
    area_server_status = 0.0;

	// 初始化系统中的人数和总时间

    total_num_in_system = 0.0; // 初始化系统中的人数
    total_time_in_system = 0.0; // 初始化系统中的总时间
	num_custs_delayed_over_1_min = 0; // 初始化计数器

    /* Initialize event list.  Since no customers are present, the departure
       (service completion) event is eliminated from consideration.  The end-
       simulation event (type 3) is scheduled for time time_end. */

    time_next_event[1] = sim_time + expon(mean_interarrival);
    time_next_event[2] = 1.0e+30;
	time_next_event[3] = time_end;
}

int mm1alt::timing(void) /* Timing function. */
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

int mm1alt::arrive(void) /* Arrival event function. */
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

		// 在顾客开始服务时计算延迟并更新最大延迟变量
        delay = sim_time - time_arrival[1];
        if (delay > max_delay) {
            max_delay = delay;
        }

        // 如果顾客延迟时间超过一分钟
		if (delay > 1.0) {
			++num_custs_delayed_over_1_min;
		}
        delay = 0.0;

        /* Schedule a departure (service completion). */

        time_next_event[2] = sim_time + expon(mean_service);
    }
	return 0;
}

void mm1alt::depart(void) /* Departure event function. */
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

        /* Increment the number of customers delayed, and schedule departure. */

        ++num_custs_delayed;
        time_next_event[2] = sim_time + expon(mean_service);

        /* Move each customer in queue (if any) up one place. */

        for (i = 1; i <= num_in_q; ++i) {
            time_arrival[i] = time_arrival[i + 1];
        }
    }
}

void mm1alt::report(void) /* Report generator function. */
{
    /* Compute and write estimates of desired measures of performance. */
	//results.push_back(mean_interarrival);
	//results.push_back(mean_service);
	//results.push_back(num_delays_required);

	results.push_back(total_of_delays / num_custs_delayed);
	results.push_back(area_num_in_q / sim_time);
	results.push_back(area_server_status / sim_time);
	results.push_back(num_custs_delayed);

    results.push_back(total_num_in_system / sim_time); // 计算系统中的平均人数
    results.push_back(total_time_in_system / num_custs_delayed); // 计算顾客在系统中的平均总时间
    results.push_back(max_queue_length); // 记录最大队列长度
    results.push_back(max_delay);    // 将最大延迟写入结果向量
    results.push_back(max_time_in_system); // 写入最大系统时间

	float proportion_delayed_over_1_min = (float)num_custs_delayed_over_1_min / num_custs_delayed;
	results.push_back(proportion_delayed_over_1_min);
}

void mm1alt::update_time_avg_stats(void) /* Update area accumulators for time-average
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

float mm1alt::expon(float mean) /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

    return -mean * log(lcgrand(1));
}
