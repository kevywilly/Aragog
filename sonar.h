/*
 * sonar.h
 *
 *  Created on: Jun 20, 2017
 *      Author: kevywilly
 */

#ifndef SONAR_H_
#define SONAR_H_

#include <NewPing.h>

class Sonar : NewPing{
public:
	unsigned int distance;
	Sonar(uint8_t trigger, uint8_t echo, unsigned int max_distance, unsigned int _too_close) : NewPing(trigger, echo, max_distance) {
		distance = 500;
		too_close = _too_close;
	}

	void read() {
		unsigned int d = this->ping_cm();
		distance = d == 0 ? 500 : d;
	}

	bool isTooClose() {
		read();
		return distance <= too_close;
	}

private:
	//NewPing sonar;

	unsigned int too_close;



};



#endif /* SONAR_H_ */
