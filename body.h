/*
 * body.h
 *
 *  Created on: Jun 13, 2017
 *      Author: kevywilly
 */

#ifndef BODY_H_
#define BODY_H_
#include <RoboTools.h>
#include "Arduino.h"
#include "constants.h"
#include "leg.h"
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  143 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  471 // this is the 'maximum' pulse length count (out of 4096)
#define STEPSIZE 2
#define STEPDELAY 5
#define Bluetooth Serial1
#define ZERO_OFFSET 0

#define DEFAULT_PWM_FREQUENCY 50

using namespace rt;

enum Heading { n, s, e, w, nw, ne, se};

class Body {

public:



	Adafruit_PWMServoDriver pwm;

	Leg leg1;
	Leg leg2;
	Leg leg3;
	Leg leg4;

	Leg * legs[4];


	void setCM(floatT3_4 cms) {
		for(int i=0; i < 4; i++) {
			legs[i]->setCM(cms.get(i));
		}
	}


	/**
	 * seek targets on all legs
	 * withDelay defaults to true
	 */
	void seekTargets() {
		seekTargets(true);
	}



	Body(uint8T3_4 ids, floatT3_4 lengths) :
			pwm(), leg1(ids._1, lengths._1, &pwm), leg2(ids._2, lengths._2, &pwm), leg3(
					ids._3, lengths._3, &pwm), leg4(ids._4, lengths._4, &pwm) {
		legs[0] = &leg1;
		legs[1] = &leg2;
		legs[2] = &leg3;
		legs[3] = &leg4;

		for(int i=0; i < 4; i++) {
			legs[i]->quadrant = i+1;
		}
	}

	void begin() {
		pwm.begin();

	  pwm.setPWMFreq(DEFAULT_PWM_FREQUENCY);

	  for (int i = 0; i < 4; i++) {
	    legs[i]->begin();
	  }
	}

	void setBase() {
		for(int i=0; i < 4; i++) {
			legs[i]->setBase();
		}
	}

	void setTargets(int8T3_4 thetas) {
		setTargets(thetas, Joint::DEFAULTSPEED);
	}


	void setTargets(int8T3_4 thetas, uint8_t speed) {

		for(int i = 0; i < 4; i++) {
			legs[i]->setTargets(thetas.get(i), speed);
		}

	}

	void setTargetsInterpolation(int8T3_4 thetas, uint8T3_4 speed) {

		for(int i = 0; i < 4; i++) {
					legs[i]->setTargetsInterpolation(thetas.get(i), speed.get(i));
				}
	}

	void setDeltas(int8T3_4 pos) {
		setDeltas(pos, Joint::DEFAULTSPEED);
	}

	void setDeltas(int8T3_4 pos, uint8_t speed) {
		for(int i = 0; i < 4; i++) {
				legs[i]->setDeltas(pos.get(i), speed);
			}
	}

	void setDeltasInterpolation(int8T3_4 thetas, uint8T3_4 speed) {

		for(int i = 0; i < 4; i++) {
						legs[i]->setDeltasInterpolation(thetas.get(i), speed.get(i));
					}
	}

	void rest() {
		for (int i = 0; i < 4; i++) {
			legs[i]->rest();
		}
	}

	void wakeup() {
		for (int i = 0; i < 4; i++) {
			legs[i]->wakeup();
		}
	}

	void setOffsets(int8T3_4 offsets) {

		for(int i = 0; i < 4; i++) {
			legs[i]->setOffsets(offsets.get(i));
		}

	}

	void revertTargets() {
		for (int i = 0; i < 4; i++) {
			legs[i]->revertTargets();
		}
	}

	void revertTargets(float speed) {
		for (int i = 0; i < 4; i++) {
			legs[i]->revertTargets(speed);
		}
	}

	void seekTargets(bool withDelay) {
		for (int i = 0; i < 4; i++) {
				legs[i]->seekTargets(false);
			}

		if(withDelay)
			delayMicroseconds(Joint::SEEKDELAYMICROS);
	}

	bool targetsReached() {
		for (int i = 0; i < 4; i++) {
					if(!legs[i]->targetsReached()) {
						return false;
					}
				}
		return true;
	}

	void gotoTargets() {
		for(int i=0; i < 4; i++) {
			legs[i]->gotoTargets();
		}
	}

	void gotoAngles(int8T3_4 thetas) {
		for(int i=0; i < 4; i++) {
			legs[i]->gotoAngles(thetas.get(i));
		}
	}

	void shift(Heading heading, float dist, uint8_t speed) {
		int h = heading;
		switch(h) {
		case Heading::nw:
			leg4.up(dist,speed);
			leg2.down(dist, speed);
			leg1.backward(dist,speed);
			leg3.backward(dist,speed);
			break;
		case Heading::ne:
			leg1.up(dist,speed);
			leg3.down(dist, speed);
			leg2.backward(dist,speed);
			leg4.backward(dist,speed);
			break;
		default :
			break;
		}
	}

};

#endif /* BODY_H_ */
