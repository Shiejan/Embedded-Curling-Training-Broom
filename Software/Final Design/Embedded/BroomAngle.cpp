/*
 * BroomAngle.cpp
 *
 *  Created on: Jan. 20, 2022
 *      Author: zbolt
 */

#include "BroomAngle.h"

#include "Bluetooth.h"

#include <Wire.h>
#include "SavLayFilter.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float broomAngle = 0;
float zeroAngle = 0;

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void dmpDataReady(void)
{
    mpuInterrupt = true;
}

char printMemUsage[] = { 0 };

void initBroomAngle(void)
{
	Wire.begin();
	Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

	mpu.initialize();
	pinMode(INTERRUPT_PIN, INPUT);

	// Load and configure the DMP
	devStatus = mpu.dmpInitialize();

	// Supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXAccelOffset(-2164);
	mpu.setYAccelOffset(153);
	mpu.setZAccelOffset(1212);
	mpu.setXGyroOffset(122);
	mpu.setYGyroOffset(-65);
	mpu.setZGyroOffset(-17);

	// Make sure it worked (returns 0 if so)
	if (devStatus == 0)
	{
		// Calibration Time: generate offsets and calibrate our MPU6050
		mpu.CalibrateAccel(6);
		mpu.CalibrateGyro(6);
		mpu.PrintActiveOffsets();

		// Turn on the DMP, now that it's ready
		mpu.setDMPEnabled(true);

		// Enable Arduino interrupt detection
		attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();

		// Set our DMP Ready flag so the main loop() function knows it's okay to use it
		dmpReady = true;

		// Get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else
	{
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}
}

void processBroomAngle(void)
{
	while (1)
	{
		// If programming failed, don't try to do anything
		if (!dmpReady) return;
		// Read a packet from FIFO
		if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) // Get the latest packet
		{
			mpu.dmpGetQuaternion(&q, fifoBuffer);
			mpu.dmpGetGravity(&gravity, &q);
			mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

			if ((getDeviceRecordStatus() == DEVICE_RECORDING) && (getCurrSweepingMetric() == BROOM_ANGLE))
			{
				broomAngle = (ypr[0] * 180.0 / M_PI);
				broomAngle = ((broomAngle - zeroAngle) > 0) ? (broomAngle - zeroAngle) : (abs(broomAngle - zeroAngle));

				sendBroomAngle(broomAngle);
			}
		}

//		print_memory_info(printMemUsage, 100);

		ThisThread::sleep_for(TICK_PERIOD_BROOM_ANGLE);
	}
}

void zeroBroomAngle(void)
{
	if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
	{
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

		zeroAngle = ypr[0] * 180.0 / M_PI;
	}
}

float getBroomAngle(void)
{
	return broomAngle;
}

void print_memory_info(char* printEvent, int iSize)
{
    // Allocate enough room for every thread's stack statistics
    int count = osThreadGetCount();
    String threadName = "";
    mbed_stats_thread_t *stats = (mbed_stats_thread_t*) malloc(count * sizeof(mbed_stats_thread_t));

    count = mbed_stats_thread_get_each(stats, count);
    for (int i = 0; i < count; i++)
    {
    	threadName = String(stats[i].name);
    	snprintf_P(printEvent, iSize,
    			"Thread ID: 0x%lX, Thread name: %s, Stack size: %lu / %lu\r\n",
				stats[i].id, stats[i].name, stats[i].stack_size - stats[i].stack_space, stats[i].stack_size);
    	Serial.println(printEvent);
    }
    free(stats);

    // Grab the heap statistics
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    snprintf_P(printEvent, iSize, "Heap size: %lu / %lu bytes\r\n", heap_stats.current_size, heap_stats.reserved_size);
    Serial.println(printEvent);
}
