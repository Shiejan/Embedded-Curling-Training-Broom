/*
 * SweepingFrequency.h
 *
 *  Created on: Jan. 13, 2022
 *      Author: zbolt
 */

#ifndef SWEEPINGFREQUENCY_H_
#define SWEEPINGFREQUENCY_H_

#include "Capstone.h"

#define TICK_PERIOD_SWEEPING_FREQUENCY 10	// 10 ms

#define FREQ_RUNNING_AVERAGE_LENGTH 3
#define MIN_ACCEL_CUTOFF 0.02
#define MICRO_SECONDS 1000000.0f

typedef enum
{
	FIRST_PEAK,
	SECOND_PEAK,
	TOTAL_PEAKS
} CurrentPeak_t;

void initSweepingFrequency(void);
void processSweepingFrequency(void);

float getFilteredAccelData(void);
void calculateFrequency(float accelX);
float getSweepingFrequency(void);

#endif /* SWEEPINGFREQUENCY_H_ */
