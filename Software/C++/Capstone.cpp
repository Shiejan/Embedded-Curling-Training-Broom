#include "Capstone.h"

#include "SweepingAngle.h"
#include "SweepingFrequency.h"
#include "BroomAngle.h"
#include "SweepingForce.h"

Thread sweepingForceThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "sweeping_force");
Thread sweepingFrequencyThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "sweeping_frequency");
Thread sweepingAngleThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "sweeping_angle");
Thread broomAngleThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "broom_angle");

void setup()
{
	Serial.begin(115200);

	initSweepingForce();
	initSweepingFrequency();
	initSweepingAngle();
	initBroomAngle();

	sweepingForceThread.start(processSweepingForce);
	sweepingFrequencyThread.start(processSweepingFrequency);
	sweepingAngleThread.start(processSweepingAngle);
	broomAngleThread.start(processBroomAngle);
}

void loop()
{

}
