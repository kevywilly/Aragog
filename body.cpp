#include "body.h"


Body::Body(uint8T3_4 ids, floatT3_4 lengths) :
		pwm(), leg1(ids._1, lengths._1, &pwm), leg2(ids._2, lengths._2, &pwm), leg3(
				ids._3, lengths._3, &pwm), leg4(ids._4, lengths._4, &pwm) {
	legs[0] = &leg1;
	legs[1] = &leg2;
	legs[2] = &leg3;
	legs[3] = &leg4;
}

void Body::begin() {
	pwm.begin();

  pwm.setPWMFreq(DEFAULT_PWM_FREQUENCY);

  for (int i = 0; i < 4; i++) {
    legs[i]->begin();
  }
}

void Body::setBase() {
	for(int i=0; i < 4; i++) {
		legs[i]->setBase();
	}
}
void Body::setTargets(int8T3_4 thetas) {
	setTargets(thetas, Joint::DEFAULTSPEED);
}


void Body::setTargets(int8T3_4 thetas, uint8_t speed) {

	for(int i = 0; i < 4; i++) {
		legs[i]->setTargets(thetas.get(i), speed);
	}

}

void Body::setTargetsInterpolation(int8T3_4 thetas, uint8T3_4 speed) {

	for(int i = 0; i < 4; i++) {
				legs[i]->setTargetsInterpolation(thetas.get(i), speed.get(i));
			}
}

void Body::setDeltas(int8T3_4 pos) {
	setDeltas(pos, Joint::DEFAULTSPEED);
}

void Body::setDeltas(int8T3_4 pos, uint8_t speed) {
	for(int i = 0; i < 4; i++) {
			legs[i]->setDeltas(pos.get(i), speed);
		}
}

void Body::setDeltasInterpolation(int8T3_4 thetas, uint8T3_4 speed) {

	for(int i = 0; i < 4; i++) {
					legs[i]->setDeltasInterpolation(thetas.get(i), speed.get(i));
				}
}

void Body::rest() {
	for (int i = 0; i < 4; i++) {
		legs[i]->rest();
	}
}

void Body::wakeup() {
	for (int i = 0; i < 4; i++) {
		legs[i]->wakeup();
	}
}

void Body::setOffsets(int8T3_4 offsets) {

	for(int i = 0; i < 4; i++) {
		legs[i]->setOffsets(offsets.get(i));
	}

}

void Body::revertTargets() {
	for (int i = 0; i < 4; i++) {
		legs[i]->revertTargets();
	}
}

void Body::revertTargets(float speed) {
	for (int i = 0; i < 4; i++) {
		legs[i]->revertTargets(speed);
	}
}

void Body::seekTargets(bool withDelay) {
	for (int i = 0; i < 4; i++) {
			legs[i]->seekTargets(false);
		}

	if(withDelay)
		delayMicroseconds(Joint::SEEKDELAYMICROS);
}

bool Body::targetsReached() {
	for (int i = 0; i < 4; i++) {
				if(!legs[i]->targetsReached()) {
					return false;
				}
			}
	return true;
}

void Body::gotoTargets() {
	for(int i=0; i < 4; i++) {
		legs[i]->gotoTargets();
	}
}

void Body::gotoAngles(int8T3_4 thetas) {
	for(int i=0; i < 4; i++) {
		legs[i]->gotoAngles(thetas.get(i));
	}
}
