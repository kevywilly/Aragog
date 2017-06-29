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
void turn(float degrees, uint8_t speed);
void turnIfTooClose();
void steps(Leg first, Leg second, uint8_t speed);
bool walkToward(Heading heading, uint8_t speed);
inline void go();
inline void go(long dlay);

/**
 * Function Implementations
 */

inline void go() {
	go(0);
}
inline void go(long dlay) {
	body.moveTillTargetsReached();
	delay(dlay);
}

inline Leg opposite(Leg l) {
	return *body.legs[diag(l.index)];
}

void turnIfTooClose() {
	return;
	int i = 0;

	if(sonar.isTooClose()) {
		walkToward(Heading::South, 15);
		turn(45,20);
	}

	while (sonar.isTooClose() && i < 10) {
		body.goHome(5); go();

		turn(30, 20);
		turn(30, 20);
		i++;
		delay(100);
	}
}



// walk2(3,4)

bool walkToward(Heading heading, uint8_t speed) {

	int h = heading;

	switch(h) {
	case Heading::East:
			//body.setQuadrants({2,3,4,1});
			steps(leg4, leg1, speed);
			steps(leg3, leg2, speed);
			break;
	case Heading::South:
		leg1.quadrant = 3;
		leg2.quadrant = 4;
		leg3.quadrant = 1;
		leg4.quadrant = 2;
		steps(leg1, leg2, speed);
		steps(leg4, leg3, speed);
		break;
	case Heading::West:
		body.setQuadrants({4,1,2,3});
		steps(leg2, leg3, speed);
		steps(leg1, leg4, speed);
		break;
	default:
		steps(leg3,leg4, speed);
		steps(leg2, leg1, speed);
		break;
	}

	body.resetQuadrants();

	return true;
}



void turn(float degrees, uint8_t speed) {

	float stepHeight = 3.0;
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

		body.legs[i]->setYaw(degrees*TO_RAD, speed);
		go();

		body.legs[i]->down(0, speed);
		body.legs[diag(i)]->down(0, speed);
		go();

	}
	for (int i = 0; i < 4; i++) {
		body.legs[i]->setYaw(0, speed);
	}
	go();

	body.resetQuadrants();
}


void steps(Leg first, Leg second, uint8_t speed) {

		tuple2<Leg> l1 = {first, body.opposite(first)};
		tuple2<Leg> l2 = {second, body.opposite(second)};

		float vlean = 3.5;
		float vstep = 8.0;
		float vout = 4.0;


		l1._1.down(vlean, speed);
		l1._2.up(vlean, speed);

		// move adjacent legs toward stepping leg
		l2._1.backward(vlean , speed );
		l2._2.backward(vlean, speed );
		go();

		// Step Leg 1
		l1._1.up(vlean, speed); go();
		l1._1.forward(vstep, speed); go();
		l1._1.down(vlean*0.2, speed/2); go();


		// Lean from leg 2
		l2._1.down(vlean*.5, speed);
		l2._2.up(vlean*.5, speed);


		// step leg 2
		l2._1.up(vlean*1.5, speed); go();
		l2._1.forward(vstep/4, speed);
		l2._1.out(vout, speed);
		//l1.forward(vstep*0.8, speed);
		//l1b.backward(vstep*0.2, speed); go();
		go();

		l2._1.down(vlean*.8,speed/2);
		l2._2.up(vlean*.8, speed);

		l1._1.forward(vstep*0.4, speed);
		l1._2.backward(vstep*0.2, speed); go();



		l2._1.up(0, speed);
		l2._2.down(0,speed);
		l2._1.backward(0,speed);
		l1._1.backward(0, speed);
		l1._2.backward(0, speed);
		l1._2.down(0, speed);
		l1._1.down(0, speed);
		go();



}


#endif /* ARAGOG_H_ */
