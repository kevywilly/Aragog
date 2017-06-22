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
#include "RoboTools.h"
#include "constants.h"
#include "body.h"
#include "leg.h"
#include "sonar.h"

#define SONAR_TRIGGER_PIN 9
#define SONAR_ECHO_PIN 8
#define SONAR_MAX_DISTANCE 500
#define SONAR_TOO_CLOSE 36

#define diag(x) ( (x) ^ 2)
#define lft(x) (((x)-1 < 0 ? 3 : (x)-1))
#define rgt(x) (((x)+1 > 3 ? 0 : (x)+1))

enum Heading {
	North, South, East, West
};

const int8T3_4 P_STAND = int8T3( { 0, 0, 0 }).toT3_4();
const int8T3_4 P_WALK = int8T3( { 0, 30, 30 }).toT3_4();
const int8T3_4 P_OFFSETS = { { 0, -5, 10 }, { 0, 0, 0 }, { 0, 0, 0 },
		{ 0, 0, 0 } };
const floatT3_4 JOINT_LENGTHS = floatT3( { 6.0, 5.2, 8.9 }).toT3_4();
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
void stroll(Leg l1, Leg l2);
inline Leg opposite(Leg l);
//inline Leg leftOf(Leg l);
//inline Leg rightOf(Leg l);

/**
 * Function Implementations
 */

inline Leg opposite(Leg l) {
	return *body.legs[diag(l.index)];
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


void stroll(Leg l1, Leg l2) {
	Leg l1B = opposite(l1);
	Leg l2B = opposite(l2);

	l1.quadrant = 3;
	l2.quadrant = 4;
	l1B.quadrant = 1;
	l2B.quadrant = 2;

	// Lean
	l1.down(3, 10);
	l1B.up(3, 10);
	l2.backward(2, 10);
	l2B.backward(2,10);
	seekTargets(10);

	//step leg 1
	l1.up(3,10); seekTargets(0);
	l1.forward(8,10); seekTargets(0);
	l1.down(3,10); seekTargets(0);

	// step leg 2 - up and forward
	l2B.up(2,10); l2B.backward(0,10); seekTargets(0);
	l2.up(3,10); seekTargets(0);

	l2B.down(3,10);
	l2.forward(0,10);
	l2.out(6,10);
	l1.forward(4,10);
	l1.down(0,10);
	l1B.down(0,10);
	seekTargets(0);

	l2B.down(0,10);
	l2.down(0,10);

	seekTargets(0);


	body.resetQuadrants();
}
// walk2(3,4)
bool walk(Heading heading, uint8_t speed, int dlay) {

	float vLean = 2.5;
	float vLeanToHLean = 0.8;
	float vStep = 4.0;
	float vStepToBigHStep = 2.0;
	float vStepToSmallHStep = 0.625;
	float reach = 5.0;

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

	// Array holding pairs of legs
	tuple2< tuple2<int> > leg_ids = {{1, 0}, {2, 3 }};

	for (int j = 0; j < 2; j++) {

		turnIfTooClose();

		Leg L1_A = *body.legs[leg_ids.get(j)._1];
		Leg L1_B = opposite(L1_A);

		Leg L2_A = *body.legs[leg_ids.get(j)._2];
		Leg L2_B = opposite(L2_A);



		// ============== Step with the first leg

		/********************************************
		 * 1: SHIFT WEIGHT FOR FIRST STEP
		 *******************************************/

		// lean away from stepping leg
		L1_A.down(vLean, speed * 2);
		L1_B.up(vLean, speed * 2);

		// move adjacent legs toward stepping leg
		L2_A.backward(vLean * vLeanToHLean, speed * 2);
		L2_B.backward(vLean * vLeanToHLean, speed * 2);

		//leftOf(leg).up(20, s * 2);
		seekTargets(dlay);

		/**************************************
		 * 2: EXECUTE STEP OF FIRST LEG
		 *************************************/

		// Raise Leg 1
		L1_A.up(vStep, speed); seekTargets(dlay);

		// Move Leg 1 forward
		L1_A.forward(vStep * vStepToBigHStep, speed); seekTargets(dlay);

		// Bring Leg 1 Down
		L1_A.down(vLean, speed); seekTargets(dlay);

		// ============= step with the second leg ============

		//steppingLeg = *body.legs[leg_index[j+1]];

		/**************************************
		 * 3: EXECUTE STEP OF LEG 2
		 *************************************/

		// Raise Leg 2
		L2_A.up(vLean, speed);

		// Reach out with Leg 2
		L2_A.out(reach, speed);

		seekTargets(dlay);

		// Move leg 2 forward
		L2_A.forward(vStep * vStepToSmallHStep, speed);
		L2_A.out(reach, speed);

		// Lean toward leg 2 by leaning away from diagonal leg
		//opposite(steppingLeg).up(vLean / 2.0, s);
		L2_B.down(vLean * 0.8, speed);

		// Rais up the body by pushing adjacent legs down a bit.
		L1_A.down(vLean * 0.5, speed);
		L1_B.down(vLean * 0.5, speed);

		// Move Leg 1 back to original position
		L1_A.backward(0, speed);

		seekTargets(dlay);

		// 7:
		// fall into Leg 2 on the ground by bending the knee, and pushing the opposite leg down
		L2_A.up(vLean*0.8, speed);
		L2_B.down(vLean, speed);

		// bring left of stepping leg back down to 0
		L1_A.down(0, speed);
		L1_B.down(0, speed);
		//L1_B.down(vLean * 1.2, speed);  // TODO maybe zero?
		seekTargets(dlay);

	}
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
