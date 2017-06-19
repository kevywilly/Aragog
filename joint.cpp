/*
 * joint.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#include "joint.h"

#include "Arduino.h"
#include "Math.h"


Joint::Joint(uint8_t pid, float _cm, Adafruit_PWMServoDriver * ppwm) : Pos2D(_cm,0.0){
	pwm = ppwm;
	id = pid;
	offset = 0;
	target = 0;
	current = 0;
	step = 0;
	orientation = 1;
	cm = DEFAULT_LENGTH_CM;
	base_angle = 0;
}

void Joint::begin() {
	gotoTarget();
}


void Joint::setTarget(int8_t theta) {
	previous_target = target;
	setTarget(theta, Joint::DEFAULTSPEED);
}

void Joint::setTarget(int8_t theta, uint8_t pspeed) {

	// get safe speed
	uint8_t speed = max(min(pspeed, MAXSPEED), MINSPEED);

	// get safe target angle
	target = safeAngle(theta);

	// what is the difference
	int diff = (target - current);

	// store the sign
	float sign = diff < 0 ? -1.0 : 1.0;

	// map 0-1 to 0 - abs(diff)
	int desiredStep = map(speed, 0, 100, 0, abs(diff))*sign;

	if (desiredStep == 0 && target != current) {
		step = 1*sign;
	} else {
		step = desiredStep;
	}

}


void Joint::seekTarget(bool withDelay) {

	// Do nothing if target is already reached
	if (targetReached()) {
		step = 0;
		return;
	}

	// Calc new angle and make sure it is safe
	int new_angle = safeAngle(current + step);

	// Are we stepping too far? If so, go to the angle
	if ((step > 0 && new_angle > target) || (step < 0 && new_angle < target)) {
		new_angle = target;
		step = 0;
	}
	// go to the angle
	gotoAngle(new_angle);

	if(withDelay)
		delayMicroseconds(SEEKDELAYMICROS);
}

void Joint::gotoTarget() {
	gotoAngle(target);
}

void Joint::resetTarget() {
	target = current;
	step = 0;
}

void Joint::gotoAngle(int8_t theta) {
	int a = safeAngle(theta);
	pwm->setPWM(id, 0, angleToPulse(a));
	current = a;
}

bool Joint::targetReached() {
	return current == target;
}

void Joint::rest() {
	pwm->setPWM(id, 0, 0);
}

void Joint::wakeup() {
	resetTarget();
	gotoTarget();
}

void Joint::revertTarget() {
	setTarget(previous_target);
}

void Joint::revertTarget(uint8_t speed) {
	setTarget(previous_target, speed);
}

void Joint::setDelta(int8_t theta) {
	setTarget(theta, DEFAULTSPEED);
}

void Joint::setDelta(int8_t theta, uint8_t speed) {
	setTarget(base_angle + theta, speed);
}

int8_t Joint::getDelta() {
	return target - base_angle;
}
