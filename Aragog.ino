#include "Aragog.h"

void setup() {

	Serial.begin(115200);

	body.setOffsets(P_OFFSETS);
	body.setTargets(P_STAND);
	body.setBase();
	body.begin();


	delay(2000);

	//stroll(leg1, leg2);

	walk(Heading::North, 4, 10);

	//turn(45, 8);
	//walk(Heading::North, 8, 8);
	//walk2();

}

// The loop function is called in an endless loop
void loop() {

	/*
	turnIfTooClose();
	walk(Heading::North, 8, 10);
	delay(100);
	*/


}


