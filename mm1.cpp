/* External definitions for single-server queueing system. */
#include "pch.h"
#include "mm1.h"
#include "lcgrand.h" /* Header file for random-number generator. */

int next_event_type, num_custs_delayed, num_delays_required, num_events,
    num_in_q, server_status;
float area_num_in_q, area_server_status, mean_interarrival, mean_service,
    sim_time, time_arrival[Q_LIMIT + 1], time_last_event, time_next_event[3],
    total_of_delays;
vector<float> results;

vector<float> mm1(float m_l, float m_s, int n_d_r) /* Main function. */
{
    /* Specify the number of events for the timing function. */

    num_events = 2;

    /* Read input parameters. */

	mean_interarrival = m_l;
	mean_service = m_s;
	num_delays_required = n_d_r;

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

void initialize(void) /* Initialization function. */
{
	/* Initialize the results vector. */

    results.clear();

    /* Initialize the simulation clock. */

    sim_time = 0.0;

    /* Initialize the state variables. */

    server_status = IDLE;
    num_in_q = 0;
    time_last_event = 0.0;

    /* Initialize the statistical counters. */

    num_custs_delayed = 0;
    total_of_delays = 0.0;
    area_num_in_q = 0.0;
    area_server_status = 0.0;

    /* Initialize event list.  Since no customers are present, the departure
       (service completion) event is eliminated from consideration. */

    time_next_event[1] = sim_time + expon(mean_interarrival);
    time_next_event[2] = 1.0e+30;
}

int timing(void) /* Timing function. */
{
    int i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event)
        {
            min_time_next_event = time_next_event[i];
            next_event_type = i;
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
    return 0;
}

int arrive(void) /* Arrival event function. */
{
    float delay;

    /* Schedule next arrival. */

    time_next_event[1] = sim_time + expon(mean_interarrival);

    /* Check to see whether server is busy. */

    if (server_status == BUSY)
    {
        /* Server is busy, so increment number of customers in queue. */

        ++num_in_q;

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
        return 0;
    }

    else
    {
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
}

void depart(void) /* Departure event function. */
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

        /* Increment the number of customers delayed, and schedule departure. */

        ++num_custs_delayed;
        time_next_event[2] = sim_time + expon(mean_service);

        /* Move each customer in queue (if any) up one place. */

        for (i = 1; i <= num_in_q; ++i) {
            time_arrival[i] = time_arrival[i + 1];
        }
    }
}

void report(void) /* Report generator function. */
{
    /* Compute and write estimates of desired measures of performance. */
	results.push_back(mean_interarrival);
	results.push_back(mean_service);
	results.push_back(num_delays_required);

	results.push_back(total_of_delays / num_custs_delayed);
	results.push_back(area_num_in_q / sim_time);
	results.push_back(area_server_status / sim_time);
	results.push_back(sim_time);
}

void update_time_avg_stats(void) /* Update area accumulators for time-average
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
}

float expon(float mean) /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

    return -mean * log(lcgrand(1));
}