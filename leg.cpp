/*
 * leg.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#include "leg.h"
#include "constants.h"

Leg::Leg(uint8T3 ids, floatT3 lengths, Adafruit_PWMServoDriver * ppwm) :
		coxa(ids._1, lengths._1, ppwm),
		femur(ids._2, lengths._2, ppwm),
		tibia(ids._3, lengths._3, ppwm) {

	joints[0] = &coxa;
	joints[1] = &femur;
	joints[2] = &tibia;

}

void Leg::begin() {
	for (int i = 0; i < 3; i++) {
		joints[i]->begin();
	}
}

void Leg::setBase() {

	for (int i = 0; i < 3; i++) {
			joints[i]->setBase();
		}
}
bool Leg::targetsReached() {
	for (int i = 0; i < 3; i++) {
		if (!joints[i]->targetReached()) {
			return false;
		}
	}
	return true;
}

void Leg::setTargets(int8T3 thetas) {
	setTargets(thetas, Joint::DEFAULTSPEED);
}

void Leg::setDeltas(int8T3 thetas) {
	setDeltas(thetas, Joint::DEFAULTSPEED);
}

void Leg::setTargets(int8T3 thetas, uint8_t speed) {
	for(int i = 0; i < 3; i++) {
			joints[i]->setTarget(thetas.get(i), speed);
	}
}

void Leg::setDeltas(int8T3 thetas, uint8_t speed) {
	for(int i = 0; i < 3; i++) {
				joints[i]->setDelta(thetas.get(i), speed);
		}
}

int8T3 Leg::getDeltas() {
	return {coxa.getDelta(), femur.getDelta(), coxa.getDelta()};
}
void Leg::setTargetsInterpolation(int8T3 thetas, uint8T3 speeds) {

	for(int i = 0; i < 3; i++) {
				joints[i]->setTarget(thetas.get(i), speeds.get(i));
		}

}

void Leg::setDeltasInterpolation(int8T3 thetas, uint8T3 speeds) {
	for(int i = 0; i < 3; i++) {
					joints[i]->setDelta(thetas.get(i), speeds.get(i));
			}
}

void Leg::seekTargets(bool withDelay) {
	for (int i = 0; i < 3; i++) {
		joints[i]->seekTarget(false);
	}
	if(withDelay)
		delayMicroseconds(Joint::SEEKDELAYMICROS);
}

void Leg::gotoTargets() {
	for (int i = 0; i < 3; i++) {
		joints[i]->gotoTarget();
	}
}

void Leg::gotoAngles(int8T3 thetas) {

	for(int i = 0; i < 3; i++) {
					joints[i]->gotoAngle(thetas.get(i));
			}
}

void Leg::rest() {
	for (int i = 0; i < 3; i++) {
		joints[i]->rest();
	}
}

void Leg::wakeup() {
	for (int i = 0; i < 3; i++) {
		joints[i]->wakeup();
	}
}

void Leg::setOffsets(int8T3 offsets) {

	for(int i = 0; i < 3; i++) {
		joints[i]->setOffset(offsets.get(i));
	}
}

void Leg::revertTargets() {
	for (int i = 0; i < 3; i++) {
		joints[i]->revertTarget();
	}
}

void Leg::revertTargets(uint8_t speed) {
	for (int i = 0; i < 3; i++) {
		joints[i]->revertTarget(speed);
	}

}
