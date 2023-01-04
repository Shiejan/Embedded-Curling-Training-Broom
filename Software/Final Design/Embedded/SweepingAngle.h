/*
 * SweepingAngle.h
 *
 *  Created on: Jan. 13, 2022
 *      Author: zbolt
 */

#ifndef SWEEPINGANGLE_H_
#define SWEEPINGANGLE_H_

#include "Capstone.h"

#define TICK_PERIOD_SWEEPING_ANGLE 50	// 50 ms

#define DECLINATION -8.90

void initSweepingAngle(void);
void processSweepingAngle(void);

void zeroSweepingAngle(void);

float getSweepingAngle(void);

#endif /* SWEEPINGANGLE_H_ */
