#include "Aragog.h"
#include "stdlib.h"

const int8T3_4 P_STAND = int8T3( { 0, 0, 0 }).toT3_4();
const int8T3_4 P_WALK = int8T3( { 0, 40, 42 }).toT3_4();
const int8T3_4 P_SIT = int8T3( { 0, 60, 70 }).toT3_4();
const int8T3_4 P_OFFSETS = {{ 0, 5, 0 },{ 0, 0, -5 },{ 0, 0, 3 },{ 0, 5, -5 }};

const uint8_t current_speed = 4;

//up down
void UpDownDemo() {
	for(int i=0; i<4; i++) {
		body.legs[i]->setZ(-5.0,2);
	}
	go();
	for(int i=0; i<4; i++) {
		body.legs[i]->setZ(5.0,2);
	}
	go();
	for(int i=0; i<4; i++) {
			body.legs[i]->setZ(0.0,2);
		}
	go();
}

void WalkDemo() {
	Heading h[4] = {Heading::North, Heading::East, Heading::South, Heading::West};
	for(int i=0; i < 4; i++) {
		walkToward(h[i], 3);
	}
}

void setup() {

	Serial.begin(115200);

	body.setOffsets(P_OFFSETS);
	body.setTargets(P_WALK, 100);
	body.setTargetsAsHome();
	body.begin();

	delay(2000);

	//walkToward(Heading::South, 3);
	//walkDemo();

}


// The loop function is called in an endless loop
void loop() {

	walkToward(Heading::North, current_speed);

}


