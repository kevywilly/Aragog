/*
 * Aragog.h
 *
 *  Created on: Jun 21, 2017
 *      Author: kevywilly
 */

#ifndef ARAGOG_H_
#define ARAGOG_H_

#include "Arduino.h"
//The setup function is called once at startup of the sketch

#include "constants.h"
#include "body.h"
#include "leg.h"
#include "sonar.h"

#define SONAR_TRIGGER_PIN 9
#define SONAR_ECHO_PIN 8
#define SONAR_MAX_DISTANCE 500
#define SONAR_TOO_CLOSE 30

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

#define diag(x) (x ^ 2)
#define lft(x) (x ^ 1 ^ 2)
#define rgt(x) (x ^ 1)

const int8T3_4 P_STAND = int8T3( { 0, 0, 0 }).toT3_4();
const int8T3_4 P_WALK = int8T3( { 0, 30, 30 }).toT3_4();
const int8T3_4 P_OFFSETS = { { 0, -5, 10 }, { 0, 0, 0 }, { 0, 0, 0 },
		{ 0, 0, 0 } };
const floatT3_4 JOINT_LENGTHS = floatT3( { 60, 52, 89 }).toT3_4();
const uint8T3_4 JOINT_IDS = { { 0, 4, 8 }, { 1, 5, 9 }, { 2, 6, 10 },
		{ 3, 7, 11 } };
const int8T3_4 ORIENTATIONS = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 },
		{ 1, 1, 1 } };

const int8T3 J0 = { 0, 0, 0 };

Sonar sonar(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE,
		SONAR_TOO_CLOSE);
Body body(JOINT_IDS, JOINT_LENGTHS);

Leg leg1 = body.leg1;
Leg leg2 = body.leg2;
Leg leg3 = body.leg3;
Leg leg4 = body.leg4;

/**
 * Function Declarations
 */
void seekTargets(long del);
void turn(float degrees, uint8_t speed);
void turnIfTooClose();
void execWalk(Leg RF, Leg RR, Leg LF, Leg LR, uint8_t speed);
bool walk(uint8_t dir, uint8_t speed);
inline Leg opposite(Leg l);
inline Leg leftOf(Leg l);
inline Leg rightOf(Leg l);

/**
 * Function Implementations
 */
inline Leg opposite(Leg l) {
	return *body.legs[l.index ^ 2];
}

inline Leg rightOf(Leg l) {
	return *body.legs[l.index ^ 1];
}

inline Leg leftOf(Leg l) {
	return *body.legs[l.index ^ 1 ^ 2];
}
void seekTargets(long del) {
	int count = 0;
	while (!body.targetsReached() && count < 100) {
		body.seekTargets();
		count++;
	}
	delay(del);
}

void turnIfTooClose() {
	int i = 0;

	while (sonar.isTooClose() && i < 10) {
		body.setTargets(P_STAND, 5);
		seekTargets(0);
		turn(30, 20);
		turn(30, 20);
		i++;
		delay(100);
	}
}

bool walk2(uint8_t dir, uint8_t speed) {

	uint8_t leg_no;
	switch (dir) {
	case 1:
		leg_no = 3;
		break;
	case 2:
		leg_no = 4;
		break;
	case 3:
		leg_no = 1;
		break;
	default:
		leg_no = 2;
	}

	int f = leg_no - 1;
	Leg LR = *body.legs[f];
	Leg LF = *body.legs[f ^ 1];
	Leg RF = *body.legs[f ^ 2];
	Leg RR = *body.legs[(f ^ 1) ^ 2];

	turnIfTooClose();
	execWalk(RF, RR, LF, LR, speed);
	turnIfTooClose();
	execWalk(LF, LR, RF, RR, speed);

	return true;
}
// walk2(3,4)
bool walk(uint8_t dir, uint8_t speed) {

	uint8_t s = speed;
	int dlay = 10;

	int8_t vLean = 25;
	int8_t hLean = 20;
	int8_t a1 = 25;
	int8_t a2 = 20;

	int leg_index;

	switch (dir) {
	case 1:
		leg_index = 2;
		break;
	case 2:
		leg_index = 3;
		break;
	case 3:
		leg_index = 0;
		break;
	default:
		leg_index = 1;
	}

	Leg leg = *body.legs[leg_index];

	turnIfTooClose();


	// 1: SHIFT WEIGHT FOR FIRST STEP

	// lean away from stepping leg
	leg.down(vLean, s * 2);
	opposite(leg).up(vLean, s * 2);

	// move adjacent legs toward stepping leg
	leftOf(leg).backward(hLean, s * 2);
	rightOf(leg).backward(hLean, s * 2);

	//leftOf(leg).up(20, s * 2);
	seekTargets(dlay);

	// 2: EXECUTE STEP OF FIRST LEG

	// raise the leg
	leg.up(40, s); seekTargets(dlay);

	// move the leg forward
	leg.forward(80, s); seekTargets(dlay);

	// bring the leg down
	leg.down(vLean, s); seekTargets(dlay);


	// 5: Raise adjacent right (clockwise) leg

	// raise adjacent leg
	rightOf(leg).up(vLean, s);
	// reach out with adjacent leg
	rightOf(leg).out(90, s); seekTargets(dlay);

	// 6: complete the step

	// shift slightly toward adjacent left leg
	leftOf(leg).up(vLean / 2.0, s);
	rightOf(leg).forward(30, s); //seekTargets(dlay);
	rightOf(leg).out(90, s);
	leftOf(leg).down(20, s);

	leg.down(20, s);
	opposite(leg).down(20, s);
	leg.backward(0, s);
	seekTargets(dlay);

	// 7:
	rightOf(leg).up(20, s); //seekTargets(dlay);
	leftOf(leg).down(20, s);
	leg.down(0, s);
	opposite(leg).down(30, s);

	seekTargets(dlay);

	return true;
}

void turn(float degrees, uint8_t speed) {
	float theta;
	float stepHeight = 30;
	int ids[4] = { 0, 2, 1, 3 };
	for (int j = 0; j < 4; j++) {
		int i = ids[j];
		theta = (i < 2 ? -degrees : degrees) * TO_RAD;
		body.legs[i]->down(stepHeight, speed);
		body.legs[diag(i)]->up(stepHeight, speed);
		seekTargets(0);

		body.legs[i]->up(stepHeight, speed);
		seekTargets(0);

		body.legs[i]->setYaw(theta, speed);
		seekTargets(0);

		body.legs[i]->down(0, speed);
		body.legs[diag(i)]->down(0, speed);
		seekTargets(0);

	}
	for (int i = 0; i < 4; i++) {
		body.legs[i]->setYaw(0, speed);
	}
	seekTargets(0);
}

void twist() {
	body.setTargets(int8T3( { 60, 0, 0 }).toT3_4(), 10);
	seekTargets(1000);
	body.setTargets(int8T3( { 0, 0, 0 }).toT3_4(), 10);
	seekTargets(1000);
}

#endif /* ARAGOG_H_ */
