/*
 * leg.h
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#ifndef LEG_H_
#define LEG_H_

#include <RoboTools.h>
#include "Arduino.h"
#include "joint.h"
#include <Adafruit_PWMServoDriver.h>

using namespace rt;
class Leg {
public:

	Leg(uint8T3 ids, floatT3 lengths, Adafruit_PWMServoDriver * ppwm);

	void begin();

	Joint coxa;
	Joint femur;
	Joint tibia;

	Joint * joints[3];

	/**
	 * Set base angle = target
	 */

	void setBase();
	/**
	 * Set length in centimeters for all joints in leg
	 */
	void setCM(floatT3 cms) {
		for(int i=0; i < 3; i++) {
			joints[i]->setCM(cms.get(i));
		}
	}

	/**
	 * Is target reached for all joints?
	 */
	bool targetsReached();

	/**
	 * Set targets for all joints with default speed
	 */
	void setTargets(int8T3 thetas);
	void setDeltas(int8T3 thetas);
	int8T3 getDeltas();

	/**
	 * Set targets for all joints with speed
	 */
	void setTargets(int8T3 thetas, uint8_t speed);
	void setDeltas(int8T3 thetas, uint8_t speed);

	/**
	 * Set targets interpolation
	 */
	void setTargetsInterpolation(int8T3 thetas, uint8T3 speeds);
	void setDeltasInterpolation(int8T3 thetas, uint8T3 speeds);

	/**
	 * Seek targets for all joints in the leg
	 */
	void seekTargets() {
		seekTargets(true);
	}
	void seekTargets(bool withDelay);

	/**
	 * Go targets for all joints in the leg
	 */
	void gotoTargets();

	/**
	 * Go to angles for all joints in the leg
	 */
	void gotoAngles(int8T3 thetas);

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
	 * Set offsets for all joints on leg
	 *
	 * Example:
	 * 	setOffsets({0,0,0})
	 */
	void setOffsets(int8T3 offsets);

	/**
	 * void revertTarget()
	 * reverts to previous target
	 */
	void revertTargets();

	/**
	 * void revertTargets(float speed)
	 * reverts to previous target
	 */
	void revertTargets(uint8_t speed);

	void setOrientations(int8T3 orients) {
		for(int i=0; i < 3; i++) {
			joints[i]->setOrientation(orients.get(i));
		}
	}


};

#endif /* LEG_H_ */
