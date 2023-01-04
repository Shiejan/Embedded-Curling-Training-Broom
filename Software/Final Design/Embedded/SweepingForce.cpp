/*
 * SweepingForce.cpp
 *
 *  Created on: Mar. 5, 2022
 *      Author: zbolt
 */

#include "SweepingForce.h"

#include "Bluetooth.h"

#include "SavLayFilter.h"

PinName forceSensorInputPin1 = AIN0;
PinName forceSensorInputPin2 = AIN1;

AnalogIn forceSensor1(forceSensorInputPin1);
AnalogIn forceSensor2(forceSensorInputPin2);

double forceSensorValues[NUM_FSR] = { 0 };
double forceSensorValueAvg = 0;
SavLayFilter savLayFilter_force(&forceSensorValueAvg, 0, 7);		//Cubic smoothing with window size of 7
float filteredForce = 0;

void initSweepingForce(void)
{

}

void processSweepingForce(void)
{
	while (1)
	{
		if ((getDeviceRecordStatus() == DEVICE_RECORDING) && (getCurrSweepingMetric() == SWEEPING_FORCE))
		{
			forceSensorValues[0] = (forceSensor1.read() * V_REF);
			forceSensorValues[1] = (forceSensor2.read() * V_REF);
			forceSensorValueAvg = (forceSensorValues[0] + forceSensorValues[1]) / 2.0;

			filteredForce = savLayFilter_force.Compute();
			filteredForce = mapForce(filteredForce);
			sendSweepingForce(filteredForce);
		}

		ThisThread::sleep_for(TICK_PERIOD_SWEEPING_FORCE);
	}
}

float mapForce(float x)
{
  return V_TO_KG(x);
}

float getSweepingForce(void)
{
	return filteredForce;
}
