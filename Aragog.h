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


#define diag(x) (x ^ 2)
#define lft(x) ((x-1 < 0 ? 3 : x-1))
#define rgt(x) ((x+1 > 3 ? 0 : x+1))

enum Heading { North, South, East, West };

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
bool walk(uint8_t dir, uint8_t speed);
inline Leg opposite(Leg l);
//inline Leg leftOf(Leg l);
//inline Leg rightOf(Leg l);

/**
 * Function Implementations
 */

inline Leg opposite(Leg l) {
	return *body.legs[diag(l.index)];
}

/*
inline Leg rightOf(Leg l) {
	return *body.legs[rgt(l.index)];
}*/

inline Leg leftOf(Leg l) {
	return *body.legs[lft(l.index)];
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


// walk2(3,4)
bool walk(Heading heading, uint8_t speed, int dlay) {


	float vLean = 25;
	float vLeanToHLean = 0.8;

	float vStep = 40;
	float vStepToBigHStep = 2.0;
	float vStepToSmallHStep = 0.625;
	float reach = 90.0;

	int leg_index[4];
	/*
	Leg steppingLeg1 = *body.legs[2];
	Leg steppingLeg2 = *body.legs[3];
	*/

	Leg steppingLeg1 = *body.legs[1];
	Leg steppingLeg2 = *body.legs[0];

	/*
	switch (int(heading)) {
	default: // North
		leg_index[0] = 2;
		leg_index[1] = 3;
		leg_index[2] = 1;
		leg_index[3] = 0;
		break;
	*/
	/*
	case Heading::East: // East
		leg_index[0] = 3;
		leg_index[1] = 2;
		break;
	case Heading::South: // South
		leg_index[0] = 0;
		leg_index[1] = 3;
		break;
	default: // West
		leg_index[0] = 1;
		leg_index[1] = 0;

	}
*/


	//turnIfTooClose();

	// ============== Step with the first leg


	// 1: SHIFT WEIGHT FOR FIRST STEP

	// lean away from stepping leg
	steppingLeg1.down(vLean, speed * 2);
	opposite(steppingLeg1).up(vLean, speed * 2);

	// move adjacent legs toward stepping leg
	opposite(steppingLeg2).backward(vLean*vLeanToHLean, speed * 2);
	steppingLeg2.backward(vLean*vLeanToHLean, speed * 2);

	//leftOf(leg).up(20, s * 2);
	seekTargets(dlay);

	// 2: EXECUTE STEP OF FIRST LEG

	// raise the leg
	steppingLeg1.up(vStep, speed); seekTargets(dlay);

	// move the leg forward
	steppingLeg1.forward(vStep*vStepToBigHStep, speed); seekTargets(dlay);

	// bring the leg down
	steppingLeg1.down(vLean, speed); seekTargets(dlay);

	// ============= step with the second leg ============

	//steppingLeg = *body.legs[leg_index[j+1]];

	// 3: Change stepping leg to the Right Adjacent leg

	// raise adjacent leg
	steppingLeg2.up(vLean, speed);
	// reach out with adjacent leg
	steppingLeg2.out(reach, speed); seekTargets(dlay);

	// 4: complete the step

	// turn stepping leg forward and reach out big
	steppingLeg2.forward(vStep*vStepToSmallHStep, speed);
	steppingLeg2.out(reach, speed);

	// lean toward stepping leg by leaning away from opposite leg
	//opposite(steppingLeg).up(vLean / 2.0, s);
	opposite(steppingLeg2).down(vLean*0.8, speed);

	// raise up the body by pushing left of and right of legs down
	steppingLeg1.down(vLean*0.8, speed);
	opposite(steppingLeg1).down(vLean*0.8, speed);

	// move the original (left of stepping leg back) to zero horizontal
	steppingLeg1.backward(0, speed);

	seekTargets(dlay);

	// 7:
	// fall into stepping leg on the ground by bending the knee, and pushing the opposite leg down
	steppingLeg2.up(20, speed);
	opposite(steppingLeg2).down(vLean, speed);

	// bring left of stepping leg back down to 0
	steppingLeg1.down(0, speed);
	opposite(steppingLeg1).down(vLean*1.2, speed);  // TODO maybe zero?
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
