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
#include "knee.h"
#include <math.h>
#include "pos2d.h"


class Leg {
public:

	enum Side {Left, Right};

	int index;
	Joint * coxa;
	Joint * femur;
	Joint * tibia;
	Knee * knee;

	int8_t quadrant;
	int8_t original_quadrant;

	Joint * joints[3];



	Leg(int idx, uint8T3 ids, floatT3 lengths, uint8_t quad,
			Adafruit_PWMServoDriver * ppwm) {

		coxa = new Joint(ids._1, lengths._1, ppwm);
		femur = new Joint(ids._2, lengths._2, ppwm);
		tibia = new Joint(ids._3, lengths._3, ppwm);
		knee = new Knee(coxa, femur, tibia);
		//body = pbody;
		index = idx;
		joints[0] = coxa;
		joints[1] = femur;
		joints[2] = tibia;
		quadrant = quad;
		original_quadrant = quad;
		speed_ = 4;

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

	void setTargets(int8T3 thetas) {
		setTargets(thetas, Joint::DEFAULTSPEED);
	}



	void setTargets(int8T3 thetas, uint8_t speed) {
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

	void setOffsets(int8T3 offsets) {

		for (int i = 0; i < 3; i++) {
			joints[i]->setOffset(offsets.at(i));
		}
	}



	 void goHome(uint8_t speed) {
			 for(int i=0; i < 3; i++) {
				 joints[i]->goHome(speed);
			 }
		 }
	/**
	 * Set length in centimeters for all joints in leg
	 */
	void setCM(floatT3 cms) {
		for (int i = 0; i < 3; i++) {
			joints[i]->setCM(cms.at(i));
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
		setZAdj(cm, speed);
	}

	inline void down(float cm, uint8_t speed) {
		setZAdj(-cm, speed);
	}

	void setX(float cm, uint8_t speed) {
		knee->setX(cm);
		setTargets(knee->getTargets(this->quadrant), speed);
	}

	void moveX(float cm, uint8_t speed) {
		setX(cm + knee->getX(), speed);
	}

	void setY(float cm, uint8_t speed) {
		knee->setY(cm);
		setTargets(knee->getTargets(this->quadrant), speed);
	}

	void moveZ(float cm, uint8_t speed) {
		knee->setZadj(cm + knee->getZ());
		setTargets(knee->getTargets(this->quadrant), speed);
	}

	void setZ(float cm, int8_t speed) {
			knee->setZ(cm);
			setTargets(knee->getTargets(this->quadrant), speed);
		}

	void setZAdj(float cm, int8_t speed) {
		knee->setZadj(cm);
		setTargets(knee->getTargets(this->quadrant), speed);
	}

	// set x and z coordinates
	void setXZAdj(float x, float z, int8_t speed) {
		knee->setXZadj(x,z);
		setTargets(knee->getTargets(this->quadrant), speed);
	}
	/**
	 * Set XYZ position of leg (knee and foot)
	 */
	void setXYZadj(float cm_x, float cm_y, float cm_z, uint8_t speed) {
		knee->setX(cm_x);
		knee->setZadj(cm_z);
		knee->setY(-cm_y);
		setTargets(knee->getTargets(this->quadrant), speed);
	}

	void setXYZ(float cm_x, float cm_y, float cm_z, uint8_t speed) {
			knee->setX(cm_x);
			knee->setZ(cm_z);
			knee->setY(-cm_y);
			setTargets(knee->getTargets(this->quadrant), speed);
		}

	void pos(tuple3<float> p) {
		setXYZadj(p._1, p._2, p._3, speed_);
	}

	void cw(float radians, uint8_t speed) {
		if(quadrant < 3) {
			setYaw(radians, speed);
		} else {
			setYaw(-radians, speed);
		}
	}

	void ccw(float radians, uint8_t speed) {
			if(quadrant < 3) {
				setYaw(-radians, speed);
			} else {
				setYaw(radians, speed);
			}
		}
	/**
	 * Set yaw in radians
	 */
	void setYaw(float radians, uint8_t speed) {
		knee->setYaw(radians);
		setTargets(knee->getTargets(this->quadrant), speed);
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
