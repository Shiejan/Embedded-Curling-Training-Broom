/*
 * SweepingForce.h
 *
 *  Created on: Mar. 5, 2022
 *      Author: zbolt
 */

#ifndef SWEEPINGFORCE_H_
#define SWEEPINGFORCE_H_

#include "Capstone.h"

#define TICK_PERIOD_SWEEPING_FORCE 50	// 50 ms

#define NUM_FSR 2
#define V_REF 3.3f

#define V_TO_KG(x) ((87.513 * x) - 74.006)

void initSweepingForce(void);
void processSweepingForce(void);

float mapForce(float x);
float getSweepingForce(void);

#endif /* SWEEPINGFORCE_H_ */
