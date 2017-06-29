#include "Aragog.h"

const int8T3_4 P_STAND = int8T3( { 0, 0, 0 }).toT3_4();
const int8T3_4 P_WALK = int8T3( { 0, 45, 45 }).toT3_4();
const int8T3_4 P_SIT = int8T3( { 0, 60, 70 }).toT3_4();
const int8T3_4 P_OFFSETS = {{ 0, 5, 0 },{ 0, 0, -5 },{ 0, 0, 3 },{ 0, 5, -5 }};

//up down
void testUpDown() {
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


void walkDemo() {
	Heading h[4] = {Heading::North, Heading::East, Heading::South, Heading::West};
	for(int i=0; i < 4; i++) {
		walkToward(h[i], 5);
	}
}
void setup() {

	Serial.begin(115200);

	body.setOffsets(P_OFFSETS);
	body.setTargets(P_WALK, 100);
	body.setTargetsAsHome();
	body.begin();

	delay(2000);

	//walkToward(Heading::North, 5);
	uint8_t speed = 5;
	leg3.down(3,speed);
	leg1.up(3,speed);
	leg2.backward(4,speed);
	leg4.backward(4,speed);
	go();


}


// The loop function is called in an endless loop
void loop() {



}


