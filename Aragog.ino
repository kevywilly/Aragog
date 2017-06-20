#include "Arduino.h"
//The setup function is called once at startup of the sketch

#include "constants.h"
#include "body.h"
#include "leg.h"


#define SONAR_TRIGGER_PIN 9
#define SONAR_ECHO_PIN 8
#define SONAR_MAX_DISTANCE 500

const int8T3_4 P_STAND = int8T3({0,0 ,0}).toT3_4();
const int8T3_4 P_WALK = int8T3({0,30 ,30}).toT3_4();
const int8T3_4 P_OFFSETS = {{0,0,5},{0,-5,0},{0,0,8},{0,0,0}};
const floatT3_4 JOINT_LENGTHS = floatT3({60, 52 ,89}).toT3_4();
const uint8T3_4 JOINT_IDS = {{ 0, 4, 8 }, { 1, 5, 9 }, { 2, 6, 10 }, { 3, 7, 11 }};
const int8T3_4 ORIENTATIONS = {{1,1,1},{1,1,1},{1,1,1},{1,1,1}};

const int8T3 J0 = {0,0,0};

int8T3 knee_up = {0,45,45};
int8T3 knee_dn = {0,-45,-45};
int8T3 leg_step = {-30,-45,-45};



Body body(JOINT_IDS, JOINT_LENGTHS);

Leg leg1 = body.leg1;
Leg leg2 = body.leg2;
Leg leg3 = body.leg3;
Leg leg4 = body.leg4;


void walk2() {
	uint8_t s = 2; //speed
	int dlay = 50;
	int8_t a1 = 40;

	// 1-1: Shift NorthWest
	body.shift(Heading::nw, a1, s); seekTargets(dlay);

	// 1-2: Step RR
	body.leg2.up(a1,s); seekTargets(dlay);
	body.leg2.forward(60,s); seekTargets(dlay);

	// 1-3
	body.leg2.down(0,s); seekTargets(dlay);

	// 1-4

	body.leg1.up(a1,s);
	body.leg1.out(60,s);
	body.leg3.up(a1,s);
	body.leg3.forward(0,s); seekTargets(dlay);

	// 1-5
	body.leg1.up(a1,s); seekTargets(dlay);
	body.leg1.forward(0,s); seekTargets(dlay);
	body.leg1.out(80,s); seekTargets(dlay);


	// 2-1 Start with Left Rear
	body.shift(Heading::ne, 30, s);
	seekTargets(dlay);

	// 2-2
	body.leg3.up(a1,s);seekTargets(dlay);
	body.leg3.forward(60,s); seekTargets(dlay);

	// 2-3
	body.leg3.down(0,s); seekTargets(dlay);

	// 2-4
	body.leg2.up(a1,s);
	body.leg4.down(a1,s);
	body.leg2.forward(0,s); seekTargets(dlay);

	// 2-5
	body.leg4.up(a1,s); seekTargets(dlay);
	body.leg4.forward(0,s); seekTargets(dlay);
	body.leg4.out(80,s); seekTargets(dlay);

}

void walk(Leg RF, Leg RR, Leg LF, Leg LR) {


  uint8_t s = 2;
  int dlay = 5;
  int8_t a1 = 30;


  // =============== Step Left Rear

  // 1:
  // Lean toward right front
  RF.up(a1,s);
  LR.down(a1,s);
  RR.up(20,s);
  RR.backward(a1, s);
  LF.backward(a1, s); seekTargets(dlay);

  // 2:
  // LR Up
  LR.up(40,s); seekTargets(dlay);

  // 3:
  // LR forward
  LR.forward(60, s); seekTargets(dlay);

  // 4:
  LR.down(40,s); seekTargets(dlay);

  // 5:
  // ================== Step Front Left
  LF.up(a1,s);
  LF.forward(0,s); //TODO
  LF.out(60,s); seekTargets(dlay);

  // 6:
  // LF fowrward to 0
  LF.forward(0,s); seekTargets(dlay);

  // 7:
  LF.down(20,s); seekTargets(dlay);

  // 8:
  // LF Down, LR lean and back to 0
  LF.up(a1,s);
  RR.down(a1,s);
  LR.up(0,s);
  RF.down(0,s);
  LR.backward(0,s);
  seekTargets(dlay);


  //RF.down(0,s);
  //LR.down(0,s);
  //seekTargets(dlay);

}
void setup() {

	body.setOffsets(P_OFFSETS);
	body.setTargets(P_STAND);
	body.setBase();
	body.begin();

 
	delay(2000);
	//walk2();

	//walk(leg1, leg2, leg4, leg3);
	//walk(leg4, leg3, leg1, leg2);


}

// The loop function is called in an endless loop
void loop() {

	walk(leg1, leg2, leg4, leg3);
	walk(leg4, leg3, leg1, leg2);

}

void twist() {
	body.setTargets(int8T3({60,0,0}).toT3_4(), 10);
	seekTargets(1000);
  body.setTargets(int8T3({0,0,0}).toT3_4(), 10);
  seekTargets(1000);
}

void seekTargets(long del) {
	int count = 0;
	while(!body.targetsReached() && count < 100) {
		body.seekTargets();
		count++;
	}
 delay(del);
}
