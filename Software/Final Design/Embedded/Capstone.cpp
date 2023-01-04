#include "Capstone.h"

#include "Bluetooth.h"
#include "SweepingAngle.h"
#include "SweepingFrequency.h"
#include "BroomAngle.h"
#include "SweepingForce.h"

Thread bluetoothThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "bluetooth");
Thread sweepingForceThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "sweeping_force");
Thread sweepingFrequencyThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "sweeping_frequency");
Thread sweepingAngleThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "sweeping_angle");
Thread broomAngleThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "broom_angle");

void setup()
{
	Serial.begin(115200);

	initBluetooth();
	initSweepingForce();
	initSweepingFrequency();
	initSweepingAngle();
	initBroomAngle();

	bluetoothThread.start(processBluetooth);
	sweepingForceThread.start(processSweepingForce);
	sweepingFrequencyThread.start(processSweepingFrequency);
	sweepingAngleThread.start(processSweepingAngle);
	broomAngleThread.start(processBroomAngle);
}

void loop()
{

}
