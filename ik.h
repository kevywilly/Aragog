/*
 * ik.h
 *
 *  Created on: Oct 27, 2017
 *      Author: kevywilly
 */

#ifndef IK_H_
#define IK_H_


#include <Arduino.h>
#include <math.h>

#define RADIANS 0.01745329252
#define DEGREES 57.2957795129

class IK {
public:
	float aCoxa;
	float aFemur;
	float aTibia;
	float x;
	float y;
	float z;
	float xBase;
	float yBase;
	float zBase;
	float aCoxaZero;
	float aFemurZero;
	float aTibiaZero;

	int a,b,c;

	IK(float coxa, float femur, float tibia) {
		coxa_ = coxa;
		femur_ = femur;
		tibia_ = tibia;

		aCoxa = 0;
		aTibia = 90;
		aFemur = 90;

		aCoxaZero = aCoxa;
		aFemurZero = aFemur;
		aTibiaZero = aTibia;

		solveForAngles(aCoxa, aTibia, aFemur);

		xBase = x;
		yBase = y;
		zBase = z;

	}

	void solveForAngles() {
		solveForAngles(aCoxa, aFemur, aTibia);
	}

	// solve to get x,y,z for angles
	void solveForAngles(float angleCoxa, float angleFemur, float angleTibia) {

		aCoxa = angleCoxa;
		aFemur = angleFemur;
		aTibia = angleTibia;

		// Distance from body to tip of femur
		float zFemur = sin((aFemur-90)*RADIANS)*femur_;
		float yFemur = cos((aFemur-90)*RADIANS)*femur_;
		float hypToFemurTip = coxa_ + yFemur;

		float a1 = aFemur;
		float a2 = aTibia - a1;

		// Distance from body to tip of tibia
		float zTibia = cos(a2*RADIANS)*tibia_;
		float yTibia = sin(a2*RADIANS)*tibia_;
		float hypToTibiaTip = hypToFemurTip+yTibia;

		// calc x
		y = cos((aCoxa)*RADIANS)*hypToTibiaTip;
		x = sin((aCoxa)*RADIANS)*hypToTibiaTip;
		z = zTibia - zFemur;

		//hypFemur = coxa_ + cos()
	}

	void solveForXyzOffset(float xOff, float yOff, float zOff) {
		solveForXyz(xBase+xOff, yBase+yOff, zBase+zOff);
	}

	void solveForXyz(float xVal, float yVal, float zVal) {


		float L1 = sqrt(pow(xVal,2) + pow(yVal,2));

		float ZOffset = zVal;


		// Calc Gamma
		float gamma = atan(xVal/yVal);

		float L = sqrt((pow(ZOffset,2)+pow((L1-coxa_),2)));


		float alpha = acos(ZOffset/L) + acos((pow(tibia_,2) - pow(femur_,2) - pow(L,2))/((-2)*femur_*L));

		float beta = acos((pow(L,2) - pow(tibia_,2) - pow(femur_,2))/((-2)*tibia_*femur_));

		aCoxa = gamma * DEGREES;
		aFemur = alpha * DEGREES;
		aTibia = beta * DEGREES;

		x = xVal;
		y = yVal;
		z = zVal;

		// recalc x,y,z based on angles calculated
		solveForAngles(aCoxa, aFemur, aTibia);

		a = -(aCoxa);
		b = (aFemur-90);
		c = -(aTibia-90);
	}


private:
	float coxa_;
	float femur_;
	float tibia_;

};


#endif /* IK_H_ */
