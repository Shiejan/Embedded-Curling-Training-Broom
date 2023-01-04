/*
 * SweepingForce.cpp
 *
 *  Created on: Mar. 5, 2022
 *      Author: zbolt
 */

#include "SweepingForce.h"

#include "SavLayFilter.h"

PinName forceSensorInputPin = AIN0;
AnalogIn forceSensor(forceSensorInputPin);

double forceSensorValue = 0;
SavLayFilter savLayFilter_force(&forceSensorValue, 0, 7);		//Cubic smoothing with window size of 7
float filteredForce = 0;

void initSweepingForce(void)
{

}

void processSweepingForce(void)
{
	while (1)
	{
		forceSensorValue = (forceSensor.read() * 3.3);
		filteredForce = savLayFilter_force.Compute();
		filteredForce = mapForce(filteredForce);
		Serial.println(filteredForce);

		ThisThread::sleep_for(1);
	}
}

float mapForce(float x)
{
  return V_TO_N(x);
}
