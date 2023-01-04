/*
 * Bluetooth.cpp
 *
 *  Created on: Mar. 16, 2022
 *      Author: zbolt
 */

#include "Bluetooth.h"

#include "SweepingFrequency.h"
#include "SweepingForce.h"
#include "SweepingAngle.h"
#include "BroomAngle.h"

#include <ArduinoBLE.h>

BLEService broomRecordStatusService("71c20cf2-51bb-4025-9d65-2d76bde6ee83");
BLEUnsignedCharCharacteristic broomRecordStatusBool("71c20cf2-51bb-4025-9d65-2d76bde6ee83", BLERead);

BLEService appRecordStatusService("220bb2c3-7666-4543-9369-9300d056b016");
BLEBoolCharacteristic appRecordStatusBool("1f41e770-b85d-4b30-a90e-1cb4c5ac7f88", BLEWrite);

BLEService sendAccelDataService("e3c6fa62-225f-4ed4-8b7b-e836100526df");
BLEUnsignedShortCharacteristic sendAccelDataUShort("6460a455-3b32-4454-8532-2add24c1a655", BLERead | BLENotify);

BLEService sendSweepingForceService("8fdf3889-5c69-4e00-a8a2-2f8cbb20c94c");
BLEUnsignedShortCharacteristic sendSweepingForceUShort("43c8c357-2278-48e5-ba17-f9709328924e", BLERead | BLENotify);

BLEService sendSweepingAngleService("374b7b46-591a-4a40-9b26-0f19d76ad45b");
BLEUnsignedShortCharacteristic sendSweepingAngleUShort("dc6fa209-5d45-438c-9776-5e4c78babd91", BLERead | BLENotify);

BLEService sendBroomAngleService("3abdbea2-e47c-431b-8113-082b63a45bea");
BLEUnsignedShortCharacteristic sendBroomAngleUShort("9f6b0b4f-79e4-4fa9-85de-e78fc2f0eded", BLERead | BLENotify);

BLEService currentMetricService("f6ffe3a6-407c-40d8-902c-042b931ed5a5");
BLEUnsignedCharCharacteristic currentMetricUChar("e0abaa08-e4c0-4fe7-93bc-a2edede13873", BLEWrite);

BLEService currentGoalService("a1b44e19-e75d-4033-8230-b7043e8eb8ff");
BLEUnsignedShortCharacteristic currentGoalUShort("95501fe3-4cc2-4589-807d-4b4cef4403c9", BLEWrite);

BLEService currentToleranceService("710b975a-c67d-4f41-b6a1-a19980ee7cf0");
BLEUnsignedShortCharacteristic currentToleranceUShort("10cdd090-a68c-4ea6-96ad-aadf9c79eb4b", BLEWrite);

BroomRecordStatus_t broomRecordStatus = DEVICE_NOT_RECORDING;
AppRecordStatus_t appRecordStatus = APP_NOT_RECORDING;
SweepingMetric_t currMetric = SWEEPING_FREQUENCY;
uint16_t currGoal = 0;
uint16_t currTolerance = 0;

float (*sweepingMetricFuncs[NUM_METRICS])() = { getSweepingFrequency, getSweepingForce, getSweepingAngle, getBroomAngle };
void (*zeroSweepingMetrics[NUM_ZERO_FUNCS])() = { zeroSweepingAngle, zeroBroomAngle };

DigitalOut redLED(P1_12);
DigitalOut greenLED(P1_15);
DigitalOut blueLED(P1_13);

void initBluetooth(void)
{
	redLED = LED_ON;
	greenLED = LED_OFF;
	blueLED = LED_ON;

	if (!BLE.begin())
	{
		Serial.println("- Starting Bluetooth Low Energy module failed!");
		while (1);
	}
	else
	{
		Serial.println("- Starting Bluetooth Low Energy module successful!");
	}

	BLE.setLocalName("Arduino Nano 33 BLE");

	broomRecordStatusService.addCharacteristic(broomRecordStatusBool);
	BLE.addService(broomRecordStatusService);

	appRecordStatusService.addCharacteristic(appRecordStatusBool);
	BLE.addService(appRecordStatusService);

	sendAccelDataService.addCharacteristic(sendAccelDataUShort);
	BLE.addService(sendAccelDataService);

	sendSweepingForceService.addCharacteristic(sendSweepingForceUShort);
	BLE.addService(sendSweepingForceService);

	sendSweepingAngleService.addCharacteristic(sendSweepingAngleUShort);
	BLE.addService(sendSweepingAngleService);

	sendBroomAngleService.addCharacteristic(sendBroomAngleUShort);
	BLE.addService(sendBroomAngleService);

	currentMetricService.addCharacteristic(currentMetricUChar);
	BLE.addService(currentMetricService);

	currentGoalService.addCharacteristic(currentGoalUShort);
	BLE.addService(currentGoalService);

	currentToleranceService.addCharacteristic(currentToleranceUShort);
	BLE.addService(currentToleranceService);

	BLE.advertise();
}

void processBluetooth(void)
{
	while (1)
	{
		BLEDevice central = BLE.central();

		if (central)
		{
			while (central.connected())
			{
				if (broomRecordStatus == APP_NOT_RECORDING)
				{
					redLED = LED_OFF;
					greenLED = LED_OFF;
					blueLED = LED_ON;

					if (currentMetricUChar.written())
					{
						currMetric = (SweepingMetric_t)currentMetricUChar.value();

						if (currMetric == SWEEPING_ANGLE)
						{
							zeroPerformanceMetric(0);
						}
						else if (currMetric == BROOM_ANGLE)
						{
							zeroPerformanceMetric(1);
						}
					}
					if (currentGoalUShort.written())
					{
						currGoal = currentGoalUShort.value();
					}
					if (currentToleranceUShort.written())
					{
						currTolerance = currentToleranceUShort.value();
					}
				}
				else if (broomRecordStatus == APP_RECORDING)
				{
					processPerformanceMetric(currMetric, currGoal, currTolerance);
				}

				processRecording();
			}
		}

		if (!central || !central.connected())
		{
			redLED = LED_ON;
			greenLED = LED_OFF;
			blueLED = LED_ON;
		}

		ThisThread::sleep_for(BLE_TICK_PERIOD);
	}
}

void processRecording(void)
{
	if (appRecordStatusBool.written())
	{
		appRecordStatus = (AppRecordStatus_t)appRecordStatusBool.value();

		if (appRecordStatus == APP_RECORDING)
		{
			if (broomRecordStatus == DEVICE_NOT_RECORDING)
			{
				setBroomRecordStatus(DEVICE_RECORDING);
			}
		}
		else if (appRecordStatus == APP_NOT_RECORDING)
		{
			if (broomRecordStatus == DEVICE_RECORDING)
			{
				setBroomRecordStatus(DEVICE_NOT_RECORDING);
			}
		}
	}
}

void processPerformanceMetric(uint8_t metric, uint16_t goal, uint16_t tolerance)
{
	float reading = (*sweepingMetricFuncs[metric])();
	float goalNormalized = (float)goal / 100.0;
	float toleranceNormalized = (float)tolerance / 100.0;

	if (reading > (goalNormalized + toleranceNormalized))
	{
		redLED = LED_ON;
		greenLED = LED_OFF;
		blueLED = LED_OFF;
	}
	else if (reading < (goalNormalized - toleranceNormalized))
	{
		redLED = LED_ON;
		greenLED = LED_ON;
		blueLED = LED_OFF;
	}
	else if ((reading <= (goalNormalized + toleranceNormalized)) && (reading >= (goalNormalized - toleranceNormalized)))
	{
		redLED = LED_OFF;
		greenLED = LED_ON;
		blueLED = LED_OFF;
	}
}

void zeroPerformanceMetric(uint8_t metric)
{
	(*zeroSweepingMetrics[metric])();
}

void setBroomRecordStatus(BroomRecordStatus_t status)
{
	broomRecordStatus = status;
	broomRecordStatusBool.writeValue(broomRecordStatus);
}

void sendAccelData(float accelData)
{
	sendAccelDataUShort.writeValue(accelData * 100);
}

void sendSweepingForce(float force)
{
	sendSweepingForceUShort.writeValue(force * 100);
}

void sendSweepingAngle(float sweepingAngle)
{
	sendSweepingAngleUShort.writeValue(sweepingAngle * 100);
}

void sendBroomAngle(float broomAngle)
{
	Serial.println(broomAngle);
	sendBroomAngleUShort.writeValue(broomAngle * 100);
}

BroomRecordStatus_t getDeviceRecordStatus(void)
{
	return broomRecordStatus;
}

SweepingMetric_t getCurrSweepingMetric(void)
{
	return currMetric;
}
