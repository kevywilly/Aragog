/*
 * joint.h
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#ifndef JOINT_H_
#define JOINT_H_

#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>



class Joint {
public:

	const static uint16_t SERVOMIN = 143; // this is the 'minimum' pulse length count (out of 4096)
	const static uint16_t SERVOMAX = 471; // this is the 'maximum' pulse length count (out of 4096)
	const static int8_t ANGLEMAX = 90;
	const static int8_t ANGLEZERO = 0;
	const static int8_t ANGLEMIN = -90;
	const static uint8_t CLOCKWISE = 1;
	const static uint8_t COUNTERCLOCKWISE = -1;
	const static uint16_t SEEKDELAYMICROS = 12000;
	const static uint8_t DEFAULTSPEED = 50;
	const static uint8_t MAXSPEED = 100;
	const static uint8_t MINSPEED = 0;
	constexpr static float DEFAULT_LENGTH_CM = 50.0;


	Joint(uint8_t pid, float _cm, Adafruit_PWMServoDriver * ppwm) {
		pwm = ppwm;
		id = pid;
		offset = 0;
		target = 0;
		current = 0;
		step = 0;

		cm = DEFAULT_LENGTH_CM;
		home_angle = 0;
	}

	void begin() {
		gotoTarget();
	}

	void setTargetsAsHome() {
		home_angle = target;
		//Pos2D pos(cm, home_angle);
	}

	void setTarget(int theta, uint8_t pspeed) {

		// get safe speed
		uint8_t speed = max(min(pspeed, MAXSPEED), MINSPEED);

		// get safe target angle
		target = safeAngle(theta+home_angle);

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


	void seekTarget(bool withDelay) {

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

	void gotoTarget() {
		gotoAngle(target);
	}

	void resetTarget() {
		target = current;
		step = 0;
	}

	void gotoAngle(int8_t theta) {
		int a = safeAngle(theta);
		pwm->setPWM(id, 0, angleToPulse(a));
		current = a;
		//this->moveTheta(a*TO_RAD);
	}

	bool targetReached() {
		return current == target;
	}

	void rest() {
		pwm->setPWM(id, 0, 0);
	}

	void wakeup() {
		resetTarget();
		gotoTarget();
	}

	int8_t getDelta() {
		return target - home_angle;
	}


	/**
	 * Sets length of joint and connecting limb
	 */
	void setCM(float pcm) {
		cm = pcm;
	}

	/**
	 * Set corrective offset from zero for joint
	 */
	void setOffset(int8_t theta) {
		offset = theta;
	}

	/*
	 * Get the offset for joint
	 */
	int8_t getOffset() {
		return offset;
	}



	/*
	 * Get target for joint
	 */
	int8_t getTarget() {
		return target;
	}

	/*
	 * Get current angle for joint
	 */
	int8_t getCurrent() {
		return current;
	}

	/*
	 * Seek target in steps
	 */
	void seekTarget() {
		seekTarget(true);
	}

	void goHome(uint8_t speed) {
		setTarget(home_angle, speed);
	}

private:

	// baseAngle

	int8_t home_angle;
	// port id
	uint8_t id;

	// length in centimeters
	float cm;

	// adjustment
	int8_t offset = 0;

	// target angle
	int8_t target = 0;

	// current angle
	int8_t current = 0;

	// current step size
	int8_t step = 0;

	// previous target
	int8_t previous_target = 0;

	Adafruit_PWMServoDriver * pwm;

	/**
	 * Make sure angle is within bounds
	 */
	int8_t safeAngle(int8_t theta) {
		return max(min(theta, ANGLEMAX), ANGLEMIN);
	}

	/**
	 * Calculate pulse length for angle
	 */
	uint16_t angleToPulse(int8_t theta) {
		return map(safeAngle(theta + offset), ANGLEMIN, ANGLEMAX, SERVOMIN,
				SERVOMAX);
	}

	/**
	 * Calculate angle from pulse length
	 */
	int8_t pulseToAngle(uint16_t pulse) {
		return map(pulse, SERVOMIN, SERVOMAX, ANGLEMIN, ANGLEMAX) - offset;
	}



};

#endif /* JOINT_H_ */
