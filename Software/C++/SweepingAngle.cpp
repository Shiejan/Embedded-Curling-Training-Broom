/*
 * SweepingAngle.cpp
 *
 *  Created on: Jan. 13, 2022
 *      Author: zbolt
 */

#include "SweepingAngle.h"

#include <Wire.h>
#include "SavLayFilter.h"

static double mx = 0;
SavLayFilter savLayFilter_mx(&mx, 0, 7);		//Cubic smoothing with window size of 7
static double my = 0;
SavLayFilter savLayFilter_my(&my, 0, 7);

static double mx_filtered = 0;
static double my_filtered = 0;

void initSweepingAngle(void)
{
	// Magnetometer settings and offsets
	IMU.setMagnetFS(0);
	IMU.setMagnetODR(8);
	IMU.setMagnetOffset(-8.986816, 6.855469, -78.754883);
	IMU.setMagnetSlope (1.278576, 1.193771, 1.250956);

	IMU.magnetUnit = MICROTESLA;
}

void processSweepingAngle(void)
{
	while (1)
	{
		float mag_x, mag_y, mag_z;

		if (IMU.magnetAvailable())
		{
			IMU.readMagnet(mag_x, mag_y, mag_z);
			mx = (double)mag_x;
			my = (double)mag_y;

			mx_filtered = savLayFilter_mx.Compute();
			my_filtered = savLayFilter_my.Compute();

			float heading = atan2(my_filtered, mx_filtered) * 180.0 / M_PI + 180.0;

			if (heading < 0)
			{
				heading += 360.0;
			}
			else if (heading > 360.0)
			{
				heading -= 360.0;
			}

			Serial.println(heading);
		}

		ThisThread::sleep_for(1);
	}
}
