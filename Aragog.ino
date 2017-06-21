#include "Aragog.h"

void setup() {

	Serial.begin(115200);

	body.setOffsets(P_OFFSETS);
	body.setTargets(P_STAND);
	body.setBase();
	body.begin();

 
	delay(2000);
	//walk2();

}

// The loop function is called in an endless loop
void loop() {

	walk(NORTH, 8);
	delay(50);

}


