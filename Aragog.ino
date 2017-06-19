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

void setup() {

	body.setOffsets(P_OFFSETS);
	body.setTargets(P_WALK);
	body.setBase();
	body.begin();

  delay(1000);


  // Shift for  1
  body.setDeltas({{+00,	-45, -45},{-40, +00, +00}, {+00, +45, +45}, {-40, +00, +00}}, 25); seekTargets(1000);
  // Step leg 1
  body.leg1.setDeltas({-30,	+45, -45}); seekTargets(1000);
  body.leg1.setDeltas({-30,	-45, -45}); seekTargets(1000);


  // Unshift for leg 1
  body.setDeltas({{-30,0,0},J0,J0,J0}, 25); seekTargets(1000);


  /*
	//body.leg1.setTargets({-20,80,30},100);
	seekTargets();

  delay(1000);
  body.leg1.setDeltas({-30,45,-20},100);
  seekTargets();
  
  delay(1000);
  body.leg1.setDeltas({-30,0,-20},100);
  seekTargets();

  delay(1000);
  */

	
	delay(10000);
  body.setTargets(P_WALK);
  seekTargets(0);
	//body.rest();
	
  /*
  delay(2000);

  twist();

  delay(2000);
  body.rest();
  */

// Add your initialization code here
}

// The loop function is called in an endless loop
void loop() {
//Add your repeated code here
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
