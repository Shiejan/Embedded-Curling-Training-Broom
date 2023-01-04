/*
 * SweepingAngle.cpp
 *
 *  Created on: Jan. 13, 2022
 *      Author: zbolt
 */

#include "SweepingAngle.h"

#include "Bluetooth.h"

#include <Wire.h>
#include "SavLayFilter.h"

static double mx = 0;
SavLayFilter savLayFilter_mx(&mx, 0, 7);		//Cubic smoothing with window size of 7
static double my = 0;
SavLayFilter savLayFilter_my(&my, 0, 7);

static double mx_filtered = 0;
static double my_filtered = 0;
static float zeroHeading = 0;
static float heading = 0;

void initSweepingAngle(void)
{
	// Magnetometer settings and offsets
	IMU.setMagnetFS(0);
	IMU.setMagnetODR(8);
	IMU.setMagnetOffset(-14.736328, 21.906738, -68.227539);
	IMU.setMagnetSlope (1.191732, 1.188810, 1.160307);
}

void processSweepingAngle(void)
{
	while (1)
	{
		float mag_x, mag_y, mag_z;

		if (IMU.magnetAvailable() && (getDeviceRecordStatus() == DEVICE_RECORDING) && (getCurrSweepingMetric() == SWEEPING_ANGLE))
		{
			IMU.readMagnet(mag_x, mag_y, mag_z);
			mx = (double)mag_x * -1;
			my = (double)mag_y * -1;

			mx_filtered = savLayFilter_mx.Compute();
			my_filtered = savLayFilter_my.Compute();

			heading = atan2(my_filtered, mx_filtered) * 180 / M_PI;
			heading += DECLINATION;
			heading = ((heading - zeroHeading) > 0) ? (zeroHeading - heading) : (abs(zeroHeading - heading));

			sendSweepingAngle(heading);
		}

		ThisThread::sleep_for(TICK_PERIOD_SWEEPING_ANGLE);
	}
}

void zeroSweepingAngle(void)
{
	float mag_x, mag_y, mag_z;

	IMU.readMagnet(mag_x, mag_y, mag_z);

	zeroHeading = atan2(my_filtered, mx_filtered) * 180 / M_PI;
	zeroHeading += DECLINATION;
}

float getSweepingAngle(void)
{
	return heading;
}
