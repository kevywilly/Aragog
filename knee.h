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
	Pos2D * coxa;
	Pos2D * femur;
	Pos2D * tibia;

	float baseRadius;

	Knee(Pos2D *pcoxa, Pos2D *pfemur, Pos2D *ptibia) {
		coxa = pcoxa;
		femur = pfemur;
		tibia = ptibia;
		baseRadius = getRadius();
	}

	float getRadius() const{
		return coxa->radius+femur->y;
	}

	float getHorizontal() const {
		Pos2D p = Pos2D(coxa->radius+femur->y, coxa->rads);
		return p.x;
	}
	float getVertical() const {
		return femur->x;
	}

	void setVertical(float v, bool footVertical) {
		femur->setX(v);
		float dir = femur->x < 0 ? -1.0 : 1.0;

		if(footVertical) {
			tibia->setX((getRadius()-baseRadius)*dir);
		}
	}

	void moveVertical(float dist, bool footVertical) {
		setVertical(getVertical()+dist, footVertical);
	}

	void setHorizontal(float h) {
		Pos2D p = Pos2D(coxa->radius+femur->y, coxa->rads);
		p.setX(h);
		coxa->setTheta(p.theta);
	}
	void moveHorizontal(float dist) {

		Pos2D p = Pos2D(coxa->radius+femur->y, coxa->rads);
		setHorizontal(p.x + dist);

	}

	void setXYZ(float x, float y, float z) {
		setHorizontal(x);
		setVertical(z,true);
		tibia->moveX(y);
	}
	void setXZ(float x, float z, bool footVertical) {
		setHorizontal(x);
		setVertical(z,footVertical);
	}

};


#endif /* KNEE_H_ */
