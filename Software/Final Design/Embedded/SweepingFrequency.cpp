/*
 * SweepingFrequency.cpp
 *
 *  Created on: Jan. 13, 2022
 *      Author: zbolt
 */

#include "SweepingFrequency.h"

#include "Bluetooth.h"
#include "SavLayFilter.h"

Timer peakTimer;

//Savitzky-Golay filter
double accel_x = 0;
SavLayFilter savLayFilterAccel(&accel_x, 0, 7);		//Cubic smoothing with window size of 7

// Peak tracking
static CurrentPeak_t currentPeak = FIRST_PEAK;
static float peakTimes[TOTAL_PEAKS] = { 0 };
static bool zeroPassed = false;

// Acceleration
static float filteredAccelX = 0;
static float frequency = 0;

void initSweepingFrequency(void)
{
	if (!IMU.begin())
	{
		Serial.println("Failed to initialize IMU!");
		while (1);
	}
	else
	{
		Serial.println("Successfully initialized IMU!");
	}

	IMU.setAccelFS(3);
	IMU.setAccelODR(5);
	IMU.setAccelOffset(-0.011394, -0.022219, 0.019609);
	IMU.setAccelSlope (0.995941, 1.000615, 1.014014);
}

void processSweepingFrequency(void)
{
	peakTimer.start();

	while (1)
	{
		float accelDataX = 0;

		if (IMU.accelerationAvailable() && (getDeviceRecordStatus() == DEVICE_RECORDING) && (getCurrSweepingMetric() == SWEEPING_FREQUENCY))
		{
			accelDataX = getFilteredAccelData();
			calculateFrequency(accelDataX);
		}

		ThisThread::sleep_for(TICK_PERIOD_SWEEPING_FREQUENCY);
	}
}

float getFilteredAccelData(void)
{
	float x = 0;
	float y = 0;
	float z = 0;

	IMU.readAcceleration(x, y, z);
	accel_x = x;

	filteredAccelX = savLayFilterAccel.Compute();

	return filteredAccelX;
}

void calculateFrequency(float accelX)
{
	static float peakValue = 0;
	static float period = 0;

	if (accelX > 0)
	{
		zeroPassed = false;

		if (((peakValue == 0) || (accelX > peakValue)) && (currentPeak == FIRST_PEAK))
		{
			peakValue = accelX;
			peakTimes[FIRST_PEAK] = duration_cast<microseconds>(peakTimer.elapsed_time()).count();
		}
		else if (((peakValue == 0) || (accelX > peakValue)) && (currentPeak == SECOND_PEAK))
		{
			peakValue = accelX;
			peakTimes[SECOND_PEAK] = duration_cast<microseconds>(peakTimer.elapsed_time()).count();
		}
	}
	else if ((accelX < 0) && (zeroPassed == false))
	{
		zeroPassed = true;

		if (currentPeak == FIRST_PEAK)
		{
			currentPeak = SECOND_PEAK;
			peakValue = 0;
		}
		else if (currentPeak == SECOND_PEAK)
		{
			period = peakTimes[SECOND_PEAK] - peakTimes[FIRST_PEAK];
			frequency = 1.0 / (period / MICRO_SECONDS);

			sendAccelData(frequency);

			currentPeak = FIRST_PEAK;
			peakTimes[FIRST_PEAK] = 0;
			peakTimes[SECOND_PEAK] = 0;
			peakValue = 0;
		}
	}
}

float getSweepingFrequency(void)
{
	return frequency;
}
