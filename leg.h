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
#include "knee.h"
#include <math.h>

using namespace rt;
class Leg {
public:

	Joint coxa;
	Joint femur;
	Joint tibia;
	Knee knee;

	int8_t quadrant;

	Joint * joints[3];

	Leg(uint8T3 ids, floatT3 lengths, Adafruit_PWMServoDriver * ppwm) :
			coxa(ids._1, lengths._1, ppwm),
			femur(ids._2, lengths._2, ppwm),
			tibia(ids._3, lengths._3, ppwm),
			knee(&coxa,  &femur, &tibia){

		joints[0] = &coxa;
		joints[1] = &femur;
		joints[2] = &tibia;
		quadrant = 1;

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


	/**
	 * Set XYZ position of leg (knee and foot)
	 */
	void setXYZ(float x, float y, float z, uint8_t speed) {

		knee.setXYZ(x,y,z);

		setKneeTargets(speed);
	}

	inline void forward(float dist, uint8_t speed) {
		setX(dist, speed);
	}

	inline void backward(float dist, uint8_t speed) {
		setX(-dist, speed);
	}

	void setX(float x, uint8_t speed) {
		knee.setX(x);
		setKneeTargets(speed);
	}

	inline void up(float dist, uint8_t speed) {
		setZ(dist, speed);
	}

	inline void down(float dist, uint8_t speed) {
			setZ(-dist, speed);
	}

	void setZ(float z, uint8_t speed) {
		knee.setZ(z);
		setKneeTargets(speed);
	}

	inline void in(float dist, uint8_t speed) {
		setY(-dist, speed);
	}

	inline void out(float dist, uint8_t speed) {
		setY(dist, speed);
	}
	void setY(float y, uint8_t speed) {
		knee.setY(y);
		setKneeTargets(speed);
	}

	void setKneeTargets(uint8_t speed) {
		int8_t acoxa = round(knee.coxa->theta*TO_DEG);
		int8_t afemur = round(knee.femur->theta*TO_DEG);
		int8_t atibia = round(knee.tibia->theta*TO_DEG);

		if(quadrant < 3) {
			acoxa *= -1;
		}
		setTargets({acoxa,afemur,atibia}, speed);
	}


};

#endif /* LEG_H_ */
