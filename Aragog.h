/*
 * Aragog.h
 *
 *  Created on: Jun 21, 2017
 *      Author: kevywilly
 */

#ifndef ARAGOG_H_
#define ARAGOG_H_

#include "Arduino.h"
#include "constants.h"
#include "body.h"
#include "leg.h"
#include "sonar.h"
#include "stdio.h"
#include "rttypes.h"

#define SONAR_TRIGGER_PIN 9
#define SONAR_ECHO_PIN 8
#define SONAR_MAX_DISTANCE 500
#define SONAR_TOO_CLOSE 36

// Find index of diagonal leg
#define diag(x) ( (x) ^ 2)
// Find index of leg to the left (ccw)
#define lft(x) ( (x-1) < 0 ? 0 : (x-1) )
// Find index of leg to the right (cw)
#define rgt(x) ((x+1) > 3 ? 3 : (x+1))

// Directional heading
enum Heading {
	North, South, East, West
};

/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Define Joint Lengths and PWM Ids ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

const floatT3_4 kJointLengths = floatT3( { 60, 52, 89 }).toT3_4();
const uint8T3_4 kJointPwmPins = { { 0, 4, 8 }, { 1, 5, 9 }, { 2, 6, 10 },{ 3, 7, 11 } };

Sonar SonarSensor(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE, SONAR_TOO_CLOSE);


//////////////////////////////////
// Define Body
//////////////////////////////////

Body body(kJointPwmPins, kJointLengths);

//////////////////////////////////
// Define pointers to each leg
//////////////////////////////////

Leg * leg1 = body.legs[0];
Leg * leg2 = body.legs[1];
Leg * leg3 = body.legs[2];
Leg * leg4 = body.legs[3];

//////////////////////////////////////////////////
// ***** Forward Declarations
//////////////////////////////////////////////////

inline void go();
inline void go(long dlay);


//////////////////////////////////////////////////////
// ***** Function Implementations
//////////////////////////////////////////////////////

// ----------------------------------------------
// Go Methods (move till targets reached)
// ----------------------------------------------
inline void go() {
	go(0);
}

// Go with Delay
inline void go(long dlay) {
	body.moveTillTargetsReached();
	delay(dlay);
}

// ----------------------------------------------
// Find opposite leg (diagonal)
// ----------------------------------------------

inline Leg opposite_to(Leg l) {
	return *body.legs[diag(l.index)];
}






#endif /* ARAGOG_H_ */
