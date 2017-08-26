/*
 * body.h
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#ifndef BODY_H_
#define BODY_H_
#include "Arduino.h"
#include "RoboTools.h"
#include "constants.h"
#include "leg.h"
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  143 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  471 // this is the 'maximum' pulse length count (out of 4096)
#define STEPSIZE 2
#define STEPDELAY 5
#define Bluetooth Serial1
#define ZERO_OFFSET 0

#define DEFAULT_PWM_FREQUENCY 50

using namespace rt;



class Body;

class Body {

public:

	// ======================================= Members =====================================
	Adafruit_PWMServoDriver pwm;

	//Leg * leg1;
	Leg * legs[4];

	// ======================================= Constructor =====================================
	Body(uint8T3_4 ids, floatT3_4 lengths) : pwm() {

		legs[0] = new Leg(0, ids._1, lengths._1, 1, &pwm);
		legs[1] = new Leg(1, ids._2, lengths._2, 2, &pwm);
		legs[2] = new Leg(2,ids._3, lengths._3, 3, &pwm);
		legs[3] = new Leg(3, ids._4, lengths._4, 4, &pwm);

	}

	// ======================================= Methods =====================================

	/*******************************************************************
	* Initialize PWM and all Legs
	********************************************************************/
	void begin() {
		pwm.begin();
		pwm.setPWMFreq(DEFAULT_PWM_FREQUENCY);

		for (int i = 0; i < 4; i++) {
		legs[i]->begin();
		}
	}

	/*******************************************************************
	* Set current targets as the home angles
	********************************************************************/
	void setTargetsAsHome() {
		for(int i=0; i < 4; i++) {
			legs[i]->setTargetsAsHome();
		}
	}


	/*******************************************************************
	* Set targets for all legs and joints
	********************************************************************/
	void setTargets(int8T3_4 thetas, uint8_t speed) {

		for(int i = 0; i < 4; i++) {
			legs[i]->setTargets(thetas.get(i), speed);
		}

	}

	/*******************************************************************
	* Set Targets for all legs and joints with different speeds
	********************************************************************/
	void setTargetsInterpolation(int8T3_4 thetas, uint8T3_4 speed) {

		for(int i = 0; i < 4; i++) {
					legs[i]->setTargetsInterpolation(thetas.get(i), speed.get(i));
				}
	}

	/*******************************************************************
	* Disengage pwm and rest the motors for all legs and joints
	********************************************************************/
	void rest() {
		for (int i = 0; i < 4; i++) {
			legs[i]->rest();
		}
	}

	/*******************************************************************
	* Re-Initialize PWM and all Legs
	********************************************************************/
	void wakeup() {
		for (int i = 0; i < 4; i++) {
			legs[i]->wakeup();
		}
	}

	/*******************************************************************
	* Let angle offsets for all joints for consistent shape
	********************************************************************/
	void setOffsets(int8T3_4 offsets) {

		for(int i = 0; i < 4; i++) {
			legs[i]->setOffsets(offsets.get(i));
		}
	}

	/*******************************************************************
	* Seek target angles for all legs (move one step toward target)
	********************************************************************/
	void seekTargets() {
		seekTargets(true);
	}

	/*******************************************************************
	* Seek target angles for all legs (move one step toward target) with delay after step
	********************************************************************/
	void seekTargets(bool withDelay) {
		for (int i = 0; i < 4; i++) {
				legs[i]->seekTargets(false);
			}

		if(withDelay)
			delayMicroseconds(Joint::SEEKDELAYMICROS);
	}

	/*******************************************************************
	* Returns true if all targets have been reached
	********************************************************************/
	bool targetsReached() {
		for (int i = 0; i < 4; i++) {
					if(!legs[i]->targetsReached()) {
						return false;
					}
				}
		return true;
	}

	/*******************************************************************
	* All joints go immediately to pre-set targets
	********************************************************************/
	void gotoTargets() {
		for(int i=0; i < 4; i++) {
			legs[i]->gotoTargets();
		}
	}

	/*******************************************************************
	* All joints go immediately to the specified angles
	********************************************************************/
	void gotoAngles(int8T3_4 thetas) {
		for(int i=0; i < 4; i++) {
			legs[i]->gotoAngles(thetas.get(i));
		}
	}

	/*******************************************************************
	* Set quadrants for all legs 1,2,3,4 starting with front right, going clockwise
	********************************************************************/
	void resetQuadrants() {
		for(int i=0; i < 4; i++) {
			legs[i]->resetQuadrant();
		}
	}

	/*******************************************************************
	* Set quadrants for all legs defined by quads parameter ie., (2,3,4,1)
	* Useful for orienting robot for a move in a different direction ie., East, West, South
	********************************************************************/
	void setQuadrants(tuple4<uint8_t> quads) {
		for(int i=0; i < 4; i++) {
			legs[i]->quadrant = quads.get(i);
		}
	}

	/*******************************************************************
	* Finds the leg that is opposite of the leg provided
	********************************************************************/
	Leg * opposite(Leg * leg) {
		return legs[leg->index ^ 2];
	}

	/*******************************************************************
	* Finds the leg that next to current leg (clockwise)
	********************************************************************/
	Leg * cw(Leg * leg) {
		return (leg->index + 1 > 3) ? legs[1] : legs[leg->index+1];
	}

	/*******************************************************************
	* Finds the leg that next to current leg (counter clockwise)
	********************************************************************/
	Leg * cc(Leg * leg) {
		return (leg->index - 1 < 0) ? legs[3] : legs[leg->index-1];
	}

	/*******************************************************************
	* Sets target angles for all joints to their home angle
	********************************************************************/
	void goHome(uint8_t speed) {
		 for(int i=0; i < 4; i++) {
			 legs[i]->goHome(speed);
		 }
	 }

	/*******************************************************************
	* Moves joints until all targets reached (blocking)
	********************************************************************/
	void moveTillTargetsReached() {
		int count = 0;
		while (!targetsReached() && count < 100) {
			seekTargets();
			count++;
		}
	}



};

#endif /* BODY_H_ */
