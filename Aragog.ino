#include "Aragog.h"
#include "stdlib.h"
#include "rttypes.h"
#include "ik.h"

const intT3_4 P_STAND = intT3( { 0, 0, 0 }).toT3_4();
const intT3_4 P_WALK = intT3( { 0, 40, 42 }).toT3_4();
const intT3_4 P_SIT = intT3( { 0, 60, 70 }).toT3_4();
const intT3_4 P_OFFSETS = {{ 0, 5, 0 },{ 0, 0, -5 },{ 0, 0, 3 },{ 0, 5, -5 }};
const intT3_4 P_LOW_WALK = intT3( { 0, 70, 70 }).toT3_4();

const uint8_t current_speed = 5;


void setup() {

	Serial.begin(115200);

	//body.setOffsets(P_OFFSETS);
	body.setTargets(P_STAND, 100);
	body.setTargetsAsHome();
	body.begin();


	delay(2000);

	body.setSpeed(25);


	// Up down
	body.seekIKOffset(0,0,30); delay(1000);
	body.seekIKOffset(0,0,-30); delay(1000);
	body.seekIKOffset(0,0,0); delay(1000);



	body.setSpeed(10);
	body.legs[0]->setIKOffset(0,0,-40);
	body.legs[2]->setIKOffset(0,0,40);
	go(1000);

	body.legs[2]->setIKOffset(0,0,-40);
	go(1000);

	body.legs[2]->setIKOffset(-80,0,-40);
	go(1000);

}


// The loop function is called in an endless loop
void loop() {

	//walker();
	//walkToward(Heading::North, current_speed);

}




