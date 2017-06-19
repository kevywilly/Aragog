/*
 * pwm.h
 *
 *  Created on: Jun 14, 2017
 *      Author: kevywilly
 */

#ifndef PWM_H_
#define PWM_H_

#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include "pwmdriver.h"

class Pwm : public PWMDriver {
public:
	Pwm() {}

	~Pwm() {}

	void begin() override {
		pwm.begin();
	}

	void setFreq(float freq)  override {
		pwm.setPWMFreq(freq);

	}
	void setPwm(uint8_t num, uint16_t on, uint16_t off) override {
		pwm.setPWM(num,on, off);
	}

private:
	Adafruit_PWMServoDriver pwm;
};

#endif /* PWM_H_ */
