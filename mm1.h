#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>

#define Q_LIMIT 100 /* Limit on queue length. */
#define BUSY 1      /* Mnemonics for server's being busy */
#define IDLE 0      /* and idle. */
#define ARRAY_SIZE 10 /* Size of array to store statistics. */

using namespace std;

vector<float> mm1(float mean_interarrival, float mean_service, int num_delays_required);
void initialize();
int timing();
int arrive();
void depart();
void report();
void update_time_avg_stats();
float expon(float mean);
