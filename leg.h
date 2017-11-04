/*
 * leg.h
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#ifndef LEG_H_
#define LEG_H_

#include "Arduino.h"
#include "joint.h"
#include <Adafruit_PWMServoDriver.h>
#include <math.h>
#include "ik.h"
#include "rttypes.h"

class Leg {
public:

	enum Side {Left, Right};

	int index;
	Joint * coxa;
	Joint * femur;
	Joint * tibia;

	int8_t quadrant;
	int8_t original_quadrant;

	Joint * joints[3];

	IK * ik;



	Leg(int idx, uint8T3 ids, floatT3 lengths, uint8_t quad,
			Adafruit_PWMServoDriver * ppwm) {

		coxa = new Joint(ids._1, lengths._1, ppwm);
		femur = new Joint(ids._2, lengths._2, ppwm);
		tibia = new Joint(ids._3, lengths._3, ppwm);

		ik = new IK(lengths._1, lengths._2, lengths._3);

		//body = pbody;
		index = idx;
		joints[0] = coxa;
		joints[1] = femur;
		joints[2] = tibia;
		quadrant = quad;
		original_quadrant = quad;
		speed_ = 4;

	}

	void setIK(float x, float y, float z) {
		ik->solveForXyz(x,y,z);
		setTargets({ik->a, ik->b, ik->c});
	}


	void setIKOffset(float x, float y, float z) {
		ik->solveForXyzOffset(x,y,z);
		setTargets({ik->a, ik->b, ik->c});
	}

	/**
	 * Begin by initializing all joints
	 */
	void begin() {
		for (int i = 0; i < 3; i++) {
			joints[i]->begin();
		}
	}

	/**
	 * Side of the body the leg is on currently;
	 */
	Side side() {
		if(quadrant > 2)
			return Side::Left;
		else
			return Side::Right;
	}

	/**
	 * Set base angle = target
	 */
	void setTargetsAsHome() {

		for (int i = 0; i < 3; i++) {
			joints[i]->setTargetsAsHome();
		}
	}

	/**
	 * Is target reached for all joints?
	 */
	bool targetsReached() {
		for (int i = 0; i < 3; i++) {
			if (!joints[i]->targetReached()) {
				return false;
			}
		}
		return true;
	}

	void setTargets(intT3 thetas) {
		setTargets(thetas, Joint::DEFAULTSPEED);
	}



	void setTargets(intT3 thetas, uint8_t speed) {
		for (int i = 0; i < 3; i++) {
			joints[i]->setTarget(thetas.at(i), speed);
		}
	}


	int8T3 getDeltas() {
		return {coxa->getDelta(), femur->getDelta(), coxa->getDelta()};
	}

	void setTargetsInterpolation(int8T3 thetas, uint8T3 speeds) {

		for (int i = 0; i < 3; i++) {
			joints[i]->setTarget(thetas.at(i), speeds.at(i));
		}

	}

	void seekTargets() {
		seekTargets(true);
	}

	void seekTargets(bool withDelay) {
		for (int i = 0; i < 3; i++) {
			joints[i]->seekTarget(false);
		}
		if (withDelay)
			delayMicroseconds(Joint::SEEKDELAYMICROS);
	}

	void gotoTargets() {
		for (int i = 0; i < 3; i++) {
			joints[i]->gotoTarget();
		}
	}

	void gotoAngles(int8T3 thetas) {

		for (int i = 0; i < 3; i++) {
			joints[i]->gotoAngle(thetas.at(i));
		}
	}

	void rest() {
		for (int i = 0; i < 3; i++) {
			joints[i]->rest();
		}
	}

	void wakeup() {
		for (int i = 0; i < 3; i++) {
			joints[i]->wakeup();
		}
	}

	void setOffsets(intT3 offsets) {

		for (int i = 0; i < 3; i++) {
			joints[i]->setOffset(offsets.at(i));
		}
	}

	void goHome(uint8_t speed) {
			 for(int i=0; i < 3; i++) {
				 joints[i]->goHome(speed);
			 }
		 }

	void setCM(floatT3 cms) {
		for (int i = 0; i < 3; i++) {
			joints[i]->setCM(cms.at(i));
		}
	}

	void resetQuadrant() {
		quadrant = original_quadrant;
	}

	void setSpeed(uint8_t speed) {
		speed_ = speed;
	}

private:
	uint8_t speed_;

};

#endif /* LEG_H_ */
