/*
 * body.h
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#ifndef BODY_H_
#define BODY_H_
#include <RoboTools.h>
#include "Arduino.h"
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

class Body {

public:

	Body(uint8T3_4 ids, floatT3_4 lengths);

	Adafruit_PWMServoDriver pwm;

	Leg leg1;
	Leg leg2;
	Leg leg3;
	Leg leg4;

	Leg * legs[4];

	/**
	 * Begin by initializing PWM and all legs / joints to their target angles
	 */
	void begin();

	/**
		 * Set base angle = target
		 */

		void setBase();

	void setCM(floatT3_4 cms) {
		for(int i=0; i < 4; i++) {
			legs[i]->setCM(cms.get(i));
		}
	}

	/**
	 * Set targets for all legs using tuple
	 */
	void setTargets(int8T3_4 thetas);
	void setTargets(int8T3_4 thetas, uint8_t speed);
	void setTargetsInterpolation(int8T3_4 thetas, uint8T3_4 speed);

	/**
	 * Set deltas for all legs using tuple
	 */
	void setDeltas(int8T3_4 thetas);
	void setDeltas(int8T3_4 thetas, uint8_t speed);
	void setDeltasInterpolation(int8T3_4 pos, uint8T3_4 speed);

	/**
	 * Rest by turning off pwm
	 */
	void rest();

	/**
	 * void wakeup()
	 * Go to previous targets
	 */
	void wakeup();

	/**
	 * Set offsets for all joints on all legs
	 *
	 * Example:
	 * 	setOffsets({0,0,0},{0,0,0},{0,0,0},{0,0,0})
	 */
	void setOffsets(int8T3_4 offsets);

	/**
	 * void revertTargets()
	 * reverts to previous target
	 */
	void revertTargets();

	/**
	 * void revertTargets(float speed)
	 * reverts to previous target
	 */
	void revertTargets(float speed);

	/**
	 * seek targets on all legs
	 * withDelay defaults to true
	 */
	void seekTargets() {
		seekTargets(true);
	}
	void seekTargets(bool withDelay);

	/**
	 * Are targets reached?
	 */
	bool targetsReached();

	/**
	 * Go immediately to targets
	 */
	void gotoTargets();

	/**
	 * Go immediately to angles
	 */
	void gotoAngles(int8T3_4 thetas);

	void setOrientations(int8T3_4 orients) {
			for(int i=0; i < 4; i++) {
				legs[i]->setOrientations(orients.get(i));
			}
		}

};

#endif /* BODY_H_ */
