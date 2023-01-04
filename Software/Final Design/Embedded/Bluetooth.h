/*
 * Bluetooth.h
 *
 *  Created on: Mar. 16, 2022
 *      Author: zbolt
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "Capstone.h"

#define BLE_TICK_PERIOD 10

#define LED_OFF 0
#define LED_ON 1

#define NUM_METRICS 4
#define NUM_ZERO_FUNCS 2

typedef enum
{
	DEVICE_NOT_RECORDING = 0,
	DEVICE_RECORDING = 1
} BroomRecordStatus_t;

typedef enum
{
	APP_NOT_RECORDING = 0,
	APP_RECORDING = 1
} AppRecordStatus_t;

typedef enum
{
	SWEEPING_FREQUENCY = 0,
	SWEEPING_FORCE = 1,
	SWEEPING_ANGLE = 2,
	BROOM_ANGLE = 3
} SweepingMetric_t;

void initBluetooth(void);
void processBluetooth(void);

void processRecording(void);
void processPerformanceMetric(uint8_t metric, uint16_t goal, uint16_t tolerance);

void zeroPerformanceMetric(uint8_t metric);
void setBroomRecordStatus(BroomRecordStatus_t status);

void sendAccelData(float accelData);
void sendSweepingForce(float force);
void sendSweepingAngle(float sweepingAngle);
void sendBroomAngle(float broomAngle);

BroomRecordStatus_t getDeviceRecordStatus(void);
SweepingMetric_t getCurrSweepingMetric(void);

#endif /* BLUETOOTH_H_ */
