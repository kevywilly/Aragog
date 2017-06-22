/*
 * leg.h
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#ifndef LEG_H_
#define LEG_H_

#include "Arduino.h"
#include "RoboTools.h"
#include "joint.h"
#include <Adafruit_PWMServoDriver.h>
#include "knee.h"
#include <math.h>

using namespace rt;

class Leg {
public:

	int index;
	Joint coxa;
	Joint femur;
	Joint tibia;
	Knee knee;

	int8_t quadrant;
	int8_t original_quadrant;

	Joint * joints[3];

	Leg(int idx, uint8T3 ids, floatT3 lengths, uint8_t quad, Adafruit_PWMServoDriver * ppwm) :
			coxa(ids._1, lengths._1, ppwm),
			femur(ids._2, lengths._2, ppwm),
			tibia(ids._3, lengths._3, ppwm),
			knee(&coxa,  &femur, &tibia){

		//body = pbody;
		index = idx;
		joints[0] = &coxa;
		joints[1] = &femur;
		joints[2] = &tibia;
		quadrant = quad;
		original_quadrant = quad;

	}

	void begin(){
		for (int i = 0; i < 3; i++) {
				joints[i]->begin();
			}
	}

	/**
	 * Set base angle = target
	 */
	void setBase() {

		for (int i = 0; i < 3; i++) {
				joints[i]->setBase();
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

	void setTargets(int8T3 thetas) {
		setTargets(thetas, Joint::DEFAULTSPEED);
	}

	void setDeltas(int8T3 thetas) {
		setDeltas(thetas, Joint::DEFAULTSPEED);
	}

	void setTargets(int8T3 thetas, uint8_t speed) {
		for(int i = 0; i < 3; i++) {
				joints[i]->setTarget(thetas.get(i), speed);
		}
	}

	void setDeltas(int8T3 thetas, uint8_t speed) {
		for(int i = 0; i < 3; i++) {
					joints[i]->setDelta(thetas.get(i), speed);
			}
	}

	int8T3 getDeltas() {
		return {coxa.getDelta(), femur.getDelta(), coxa.getDelta()};
	}
	void setTargetsInterpolation(int8T3 thetas, uint8T3 speeds) {

		for(int i = 0; i < 3; i++) {
					joints[i]->setTarget(thetas.get(i), speeds.get(i));
			}

	}

	void setDeltasInterpolation(int8T3 thetas, uint8T3 speeds) {
		for(int i = 0; i < 3; i++) {
						joints[i]->setDelta(thetas.get(i), speeds.get(i));
				}
	}

	void seekTargets() {
			seekTargets(true);
		}

	void seekTargets(bool withDelay) {
		for (int i = 0; i < 3; i++) {
			joints[i]->seekTarget(false);
		}
		if(withDelay)
			delayMicroseconds(Joint::SEEKDELAYMICROS);
	}

	void gotoTargets() {
		for (int i = 0; i < 3; i++) {
			joints[i]->gotoTarget();
		}
	}

	void gotoAngles(int8T3 thetas) {

		for(int i = 0; i < 3; i++) {
						joints[i]->gotoAngle(thetas.get(i));
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

	void setOffsets(int8T3 offsets) {

		for(int i = 0; i < 3; i++) {
			joints[i]->setOffset(offsets.get(i));
		}
	}

	void revertTargets() {
		for (int i = 0; i < 3; i++) {
			joints[i]->revertTarget();
		}
	}

	/**
	 * Revert targets
	 */
	void revertTargets(uint8_t speed) {
		for (int i = 0; i < 3; i++) {
			joints[i]->revertTarget(speed);
		}

	}


	/**
	 * Set length in centimeters for all joints in leg
	 */
	void setCM(floatT3 cms) {
		for(int i=0; i < 3; i++) {
			joints[i]->setCM(cms.get(i));
		}
	}


	inline void forward(float dist, uint8_t speed) {
		setX(dist, speed);
	}

	inline void backward(float dist, uint8_t speed) {
		setX(-dist, speed);
	}

	inline void in(float dist, uint8_t speed) {
		setY(dist, speed);
	}

	inline void out(float dist, uint8_t speed) {
		setY(-dist, speed);
	}

	inline void up(float cm, uint8_t speed) {
		setZ(cm, speed);
	}

	inline void down(float cm, uint8_t speed) {
		setZ(-cm, speed);
	}

	void setX(float cm, int8_t speed) {
		knee.setX(cm);
		setTargets(knee.getTargets(this->quadrant),speed);
	}

	void setY(float cm, uint8_t speed) {
			knee.setY(cm);
			setTargets(knee.getTargets(this->quadrant),speed);
		}

	void setZ(float cm, int8_t speed) {
		knee.setZ(cm);
		setTargets(knee.getTargets(this->quadrant),speed);
	}
	/**
	 * Set XYZ position of leg (knee and foot)
	 */
	void setXYZ(float cm_x, float cm_y, float cm_z, uint8_t speed) {
		knee.setXYZ(cm_x,cm_y,cm_z);
		setTargets(knee.getTargets(this->quadrant),speed);
	}

	/**
	 * Set yaw in radians
	 */
	void setYaw(float radians, uint8_t speed) {
		knee.setYaw(radians);
		setTargets(knee.getTargets(this->quadrant),speed);
	}


	void resetQuadrant() {
		quadrant = original_quadrant;
	}



};

#endif /* LEG_H_ */
