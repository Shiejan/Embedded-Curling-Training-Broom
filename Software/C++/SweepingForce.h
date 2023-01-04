/*
 * SweepingForce.h
 *
 *  Created on: Mar. 5, 2022
 *      Author: zbolt
 */

#ifndef SWEEPINGFORCE_H_
#define SWEEPINGFORCE_H_

#include "Capstone.h"

#define V_REF 0.3
#define V_MAX 3.3
#define FORCE_MIN 0
#define FORCE_MAX 445

#define V_TO_N(x) ((175.33 * x) - 133.59)

void initSweepingForce(void);
void processSweepingForce(void);

float mapForce(float x);

#endif /* SWEEPINGFORCE_H_ */
