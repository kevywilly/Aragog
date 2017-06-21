/*
 * knee.h
 *
 *  Created on: Jun 19, 2017
 *      Author: kevywilly
 */

#ifndef KNEE_H_
#define KNEE_H_

#include "joint.h"
#include "RoboTools.h"

class Knee {
public:
	Joint * coxa;
	Joint * femur;
	Joint * tibia;

	float baseRadius;

	Knee(Joint *pcoxa, Joint *pfemur, Joint *ptibia) {
		coxa = pcoxa;
		femur = pfemur;
		tibia = ptibia;
		baseRadius = getRadius();
	}

	inline float getRadius() const{
		return coxa->radius+femur->y;
	}

	float getHorizontal() const {
		Pos2D p = Pos2D(coxa->radius+femur->y, coxa->rads);
		return p.x;
	}
	float getVertical() const {
		return femur->x;
	}

	/**
	 * Set vertical position of knee
	 */
	void setVertical(float v) {

		// adjust x pos of femur (vertical)
		femur->setX(v);
		// keep tibia vertical
		tibia->setTheta(femur->theta);

	}

	inline void moveVertical(float dist) {
		setVertical(getVertical()+dist);
	}

	void setTheta(float theta) {
		Pos2D p = Pos2D(getRadius(), coxa->rads);
		p.setTheta(theta);
		coxa->setTheta(p.theta);
	}
	void setHorizontal(float h) {
		Pos2D p = Pos2D(getRadius(), coxa->rads);
		p.setX(h);
		coxa->setTheta(p.theta);
	}
	void moveHorizontal(float dist) {

		Pos2D p = Pos2D(getRadius(), coxa->rads);
		setHorizontal(p.x + dist);

	}

	void setXYZ(float x, float y, float z) {
		setHorizontal(x);
		setVertical(z);
		tibia->moveX(-y);
	}

	inline void setX(float x) {
		setHorizontal(x);
	}
	inline void setZ(float z) {
		setVertical(z);
	}

	void setY(float y) {
		tibia->setTheta(femur->theta);
		tibia->moveX(-y);
	}


};


#endif /* KNEE_H_ */
