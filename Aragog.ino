#include "Aragog.h"
#include "stdlib.h"

const int8_3x4_t P_STAND = int8_x3_t( { 0, 0, 0 }).toT3_4();
const int8_3x4_t P_WALK = int8_x3_t( { 0, 40, 42 }).toT3_4();
const int8_3x4_t P_SIT = int8_x3_t( { 0, 60, 70 }).toT3_4();
const int8_3x4_t P_OFFSETS = {{ 0, 5, 0 },{ 0, 0, -5 },{ 0, 0, 3 },{ 0, 5, -5 }};
const int8_3x4_t P_LOW_WALK = int8_x3_t( { 0, 70, 70 }).toT3_4();

const uint8_t current_speed = 5;

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

	/*
	for(int i=0; i < 5; i++) {
		walker();
	}

	delay(5000);
	body.rest();
	//walkToward(Heading::South, 3);
	//walkDemo();
	*/
}


// The loop function is called in an endless loop
void loop() {

	//walker();
	walkToward(Heading::North, current_speed);

}

void walker() {


	body.setSpeed(5);

	// First leg step (rear right)
	body.pos({{-3.5,0,0},{0,3.5,0},{-3.5,0,0},{0,-3.5,0}}); go();
	body.pos({{-3.5,0,0},{0,3.5,4},{-3.5,0,0},{0,-3.5,0}}); go();
	body.pos({{-3.5,0,0},{6,3.5,4},{-3.5,0,0},{0,-3.5,0}}); go();
	body.pos({{-1.75,0,0},{6,0,0},{-1.75,0,0},{0,0,0}}); go();

	body.pos({{0,0,0},{6,0,0},{0,0,0},{0,0,0}}); go();

	// Second leg step (front right)
	body.pos({{0,1.75,4},{6,0,0},{0,-1.75,0},{0,0,0}}); go();
	body.pos({{0,1.75,4},{6,0,0},{0,-1.75,0},{0,0,0}}); go();
	body.pos({{0,0,0},{2.5,0,0},{0,0,0},{-2.5,0,0}}); go();

	// Third leg step (rear left)
	body.pos({{0,-3.5,0},{-3.5,0,0},{0,3.5,0},{-3.5,0,0}}); go();
	body.pos({{0,-3.5,0},{-3.5,0,0},{0,3.5,4},{-3.5,0,0}}); go();
	body.pos({{0,-3.5,0},{-3.5,0,0},{5,3.5,4},{-3.5,0,0}}); go();
	body.pos({{0,0,0},{-1.75,0,0},{5,0,0},{-1.75,0,0}}); go();

	body.pos({{0,0,0},{0,0,0},{5,0,0},{0,0,0}}); go();

	// Fourth leg step (front right)
	body.pos({{0,0,0},{0,-1.75,0},{5,0,0},{0,1.75,4}}); go();
	body.pos({{-2.5,0,0},{0,0,0},{2.5,0,0},{0,0,0}}); go();


}


