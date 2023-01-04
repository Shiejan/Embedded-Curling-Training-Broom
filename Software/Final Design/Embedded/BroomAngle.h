/*
 * BroomAngle.h
 *
 *  Created on: Jan. 20, 2022
 *      Author: zbolt
 */

#ifndef BROOMANGLE_H_
#define BROOMANGLE_H_

#include "Capstone.h"

#define TICK_PERIOD_BROOM_ANGLE 50	// 50 ms

#define INTERRUPT_PIN 6

void initBroomAngle(void);
void processBroomAngle(void);

void dmpDataReady(void);
void zeroBroomAngle(void);

float getBroomAngle(void);

void print_memory_info(char* printEvent, int iSize);

#endif /* BROOMANGLE_H_ */
