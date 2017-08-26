/*
 * Aragog.h
 *
 *  Created on: Jun 21, 2017
 *      Author: kevywilly
 */

#ifndef ARAGOG_H_
#define ARAGOG_H_

#include "Arduino.h"
#include "RoboTools.h"
#include "constants.h"
#include "body.h"
#include "leg.h"
#include "sonar.h"
#include "stdio.h"

#define SONAR_TRIGGER_PIN 9
#define SONAR_ECHO_PIN 8
#define SONAR_MAX_DISTANCE 500
#define SONAR_TOO_CLOSE 36

#define diag(x) ( (x) ^ 2)
#define lft(x) ( (x-1) < 0 ? 0 : (x-1) )
#define rgt(x) ((x+1) > 3 ? 3 : (x+1))

enum Heading {
	North, South, East, West
};


const floatT3_4 JOINT_LENGTHS = floatT3( { 6.0, 5.2, 8.9 }).toT3_4();
const uint8T3_4 JOINT_IDS = { { 0, 4, 8 }, { 1, 5, 9 }, { 2, 6, 10 },{ 3, 7, 11 } };

Sonar SonarSensor(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE,
SONAR_TOO_CLOSE);
Body body(JOINT_IDS, JOINT_LENGTHS);

// ==============================================
// Legs
// ==============================================

Leg * leg1 = body.legs[0];
Leg * leg2 = body.legs[1];
Leg * leg3 = body.legs[2];
Leg * leg4 = body.legs[3];

// ==============================================
// Function Declarations
// ==============================================

void turn(float degrees, uint8_t speed);
bool turnIfTooClose();
bool steps(Leg * first, Leg * second, uint8_t speed);
bool walkToward(Heading heading, uint8_t speed);
inline void go();
inline void go(long dlay);


// ==============================================
// Function Implementations
// ==============================================

// ----------------------------------------------
// Go Methods (move till targets reached)
// ----------------------------------------------
inline void go() {
	go(0);
}
inline void go(long dlay) {
	body.moveTillTargetsReached();
	delay(dlay);
}

// ----------------------------------------------
// Opposite Leg Method
// ----------------------------------------------

inline Leg opposite(Leg l) {
	return *body.legs[diag(l.index)];
}

// ----------------------------------------------
// Turn If Too Close Method
// ----------------------------------------------

bool turnIfTooClose() {

	int i = 0;

	while (SonarSensor.isTooClose() && i < 10) {
				//body.goHome(5); go();

				turn(45, 10);
				delay(100);
				turn(45, 10);
				i++;
				delay(100);

			}
	return false;
}

// ----------------------------------------------
// Walk Toward A Specific Heading
// ----------------------------------------------

bool walkToward(Heading heading, uint8_t speed) {

	int h = heading;

	switch(h) {
	case Heading::East:
		body.setQuadrants({4,1,2,3});
		if(steps(leg4, leg1, speed)) steps(leg3, leg2, speed);

		break;
	case Heading::South:
		body.setQuadrants({3,4,1,2});
		if(steps(leg1, leg2, speed)) steps(leg4, leg3, speed);;
		break;
	case Heading::West:
		body.setQuadrants({2,3,4,1});
		if(steps(leg2, leg3, speed)) steps(leg1, leg4, speed);
		break;
	default:
		if(steps(leg3,leg4, speed)) steps(leg2, leg1, speed);
		break;
	}

	body.resetQuadrants();

	return true;
}




// ----------------------------------------------------------
// Perform Walking Steps (first pair, second pair of legs)
// ----------------------------------------------------------

bool steps(Leg * first, Leg * second, uint8_t speed) {

		// Get pointers to each stepping leg and its opposite leg

		Leg * l1_1 = first;
		Leg * l1_2 = body.opposite(l1_1);
		Leg * l2_1 = second;
		Leg * l2_2 = body.opposite(l2_1);

		// Set walking Parameters

		float vlean = 3.5;
		float vstep = 8.0;
		float vstep2 = 5.0;
		float vlift = 4.0;
		float vout = 3.0;


		// 1: Lean Away from First Stepping Leg

		l1_1->down(vlean, speed*1.5);
		l1_2->up(vlean,  speed*1.5);

		l2_1->backward(vlean, speed*1.5 );
		l2_2->backward(vlean, speed*1.5 );

		go();


		// 2: Lift / Step / Down First Stepping Leg

		l1_1->up(vlift,  speed*1.5);

		go();

		l1_1->forward(vstep,  speed*1.5);

		go();

		l1_1->down(vlean, speed/2);

		go();


		// 3: Lean Away from Second Stepping Leg

		l2_1->down(vlean*.5,  speed*1.5);
		l2_2->up(vlean*.8,  speed*1.5);

		go();


		// 4: Lift / Step / Reach Second Stepping Leg

		l2_1->up(vlift,  speed*1.5); go();
		l2_1->forward(vstep2,  speed*1.5);
		l2_1->out(vout,  speed*1.5);
		l2_2->up(vlean*1.2, speed);

		l1_1->forward(vstep*0.8, speed);

		go();


		// 5: Bring Second Stepping Leg Down, Opposite Leg Up

		l2_1->down(vlean,speed/2);
		l2_2->up(vlean,  speed);

		go();


		// 6: Start Moving Original Stepping Leg and Opposite Backward

		l2_1->down(0,speed);
		l2_2->down(0,speed);

		l1_1->up(vlean*.5, speed);
		l1_1->forward(vstep*0.4,  speed);

		l1_2->down(vlean*.5, speed);
		l1_2->backward(vstep*0.4,  speed);

		go();

		// 7: Second Stepping Leg set to 0 Height,
		//    and Move it Back to Default Position

		l1_1->setXYZ(0,0,0, speed);
		l1_2->setXYZ(0,0,0, speed);

		l2_1->setXYZ(0,0,0, speed);
		l2_2->setXYZ(0,0,0, speed);

		go();

		return true;
}

// ----------------------------------------------
// Turn a certain number of degrees
// ----------------------------------------------

void turn(float degrees, uint8_t speed) {


	float stepHeight = 4.2;
	int ids[4] = { 0, 2, 1, 3 };

	for(int i=0; i < 4; i++) {
		body.legs[i]->quadrant = 3;
	}

	for (int j = 0; j < 4; j++) {
		int i = ids[j];

		body.legs[i]->down(stepHeight, speed);
		body.legs[diag(i)]->up(stepHeight, speed);
		go();

		body.legs[i]->up(stepHeight, speed);
		go();

		body.legs[i]->cw(degrees*TO_RAD, speed);
		go();

		body.legs[i]->down(0, 5);
		body.legs[diag(i)]->down(0, 5);
		go();

	}
	for (int i = 0; i < 4; i++) {
		body.legs[i]->setYaw(0, speed);
	}
	go();

	//body.resetQuadrants();


}

#endif /* ARAGOG_H_ */
