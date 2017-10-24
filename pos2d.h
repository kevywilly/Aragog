/*
 * pos2d.h
 *
 *  Created on: Jun 22, 2017
 *      Author: kevywilly
 */

#ifndef POS2D_H_
#define POS2D_H_

#include "Arduino.h"

class Pos2D {
public:

	float x0;
	float y0;
	float theta0;
	float x;
	float y;
	float theta; //radians
	float radius;


	/**
	 * Initialize pos2d with radius and base angle
	 */
	Pos2D(float radius, float theta) {

		this->radius = radius;
		this->theta0 = this->theta = theta;

		x0 = x = calcX(this->radius,this->theta0);
		y0 = y = calcY(this->radius,this->theta0);

	}

	/**
	 * Calculate x based on hypotenuse and angle
	 */
	static float calcX(float hypotenuse, float theta) {
		return sin(theta)*hypotenuse;
	}

	/**
	 * Calculate y based on hypotenuse and angle
	 */
	static float calcY(float hypotenuse, float theta) {
			return cos(theta)*hypotenuse;;
	}

	/**
	 * reset to angle 0
	 */
	void reset() {
		x = x0;
		y = y0;
		theta = theta0;
	}

	int servoDegrees() const{
		return -round(degrees(theta));
	}

	void setX(float px) {
		x = (px < 0 ? -px : px) > radius ? radius : px;
		theta = asin(x/radius);
		y = calcY(radius,theta);
	}

	/*
	 * MoveX
	 */
	void moveX(float dist) {
		setX(x+dist);

	}

	void setTheta(float rads) {
		theta = rads;
		x = calcX(radius, theta);
		y = calcY(radius, theta);
	}

	/**
	 * Set Y
	 */
	void setY(float py) {
		y = (py < 0 ? -py : py) > radius ? radius : py;
		theta = acos(y/radius);
		x = calcX(radius,theta);
	}
	/**
	 * MoveY
	 */
	void moveY(float dist) {
		setY(y+dist);
	}

	/**
	 * MoveTheta
	 */
	void moveTheta(float rads) {
		setTheta(theta+rads);
	}

	/**
	 * Relative Angle
	 */
	float relativeTheta() {
		return theta;
	}

};



#endif /* POS2D_H_ */
