/*
 * joint.h
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#ifndef JOINT_H_
#define JOINT_H_

#include <Adafruit_PWMServoDriver.h>
#include <RoboTools.h>
#include <Wire.h>

using namespace rt;

class Joint : public Pos2D{
public:

	const static uint16_t SERVOMIN = 143; // this is the 'minimum' pulse length count (out of 4096)
	const static uint16_t SERVOMAX = 471; // this is the 'maximum' pulse length count (out of 4096)
	const static int8_t ANGLEMAX = 90;
	const static int8_t ANGLEZERO = 0;
	const static int8_t ANGLEMIN = -90;
	const static uint8_t CLOCKWISE = 1;
	const static uint8_t COUNTERCLOCKWISE = -1;
	const static uint16_t SEEKDELAYMICROS = 16000;
	const static uint8_t DEFAULTSPEED = 50;
	const static uint8_t MAXSPEED = 100;
	const static uint8_t MINSPEED = 0;
	constexpr static float DEFAULT_LENGTH_CM = 50.0;

	/**
	 * Create joint object with pin and reference to Adafruit PWM Module
	 */
	Joint(uint8_t id, float _cm, Adafruit_PWMServoDriver * ppwm);

	/**
	 * Initialize joint with defaults
	 */
	void begin();

	void setBase() {
		base_angle = target;
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
	 * Set target angle for joint with default speed = 1.0
	 */
	void setTarget(int8_t theta);

	/*
	 * Set target angle for joint with speed 0.0 - 1.0
	 */
	void setTarget(int8_t theta, uint8_t speed);

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

	/*
	 * Seek target in steps
	 */
	void seekTarget(bool withDelay);

	/*
	 * Goes to target immediately
	 */
	void gotoTarget();

	/*
	 * Sets target equal to current angle
	 */
	void resetTarget();

	// Go immediately to angle
	void gotoAngle(int8_t theta);

	/*
	 * Is the target reached?
	 */
	bool targetReached();

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
	 * void revertTarget()
	 * reverts to previous target
	 */
	void revertTarget();

	/**
	 * void revertTarget(float speed)
	 * reverts to previous target
	 */
	void revertTarget(uint8_t speed);

	/**
	 * Set delta target (add or subtract from current target)
	 */
	void setDelta(int8_t theta);

	/**
	 * Set delta target (add or subtract from current target)
	 */
	void setDelta(int8_t theta, uint8_t speed);

	/**
	 * Get current delta
	 */
	int8_t getDelta();

	/**
	 * Set orientation
	 */
	void setOrientation(int8_t orient) {
		orientation = orient;
	}

private:

	// baseAngle

	int8_t base_angle;
	// port id
	uint8_t id;

	// length in centimeters
	float cm;

	// orientation
	int8_t orientation;

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
		return map(safeAngle((theta + offset)*orientation), ANGLEMIN, ANGLEMAX, SERVOMIN,
				SERVOMAX);
	}

	/**
	 * Calculate angle from pulse length
	 */
	int8_t pulseToAngle(uint16_t pulse) {
		return map(pulse, SERVOMIN, SERVOMAX, ANGLEMIN, ANGLEMAX)*orientation - offset;
	}



};

#endif /* JOINT_H_ */
