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

	// Create knee with coxa, femur and tibia
	Knee(Joint *pcoxa, Joint *pfemur, Joint *ptibia) {
		coxa = pcoxa;
		femur = pfemur;
		tibia = ptibia;
		baseRadius = getRadius();
	}

		float getRadius() const{
			return coxa->pos.radius+femur->pos.y;
		}

		float getX() const {
			Pos2D p = Pos2D(getRadius(), 0);
			p.setTheta(coxa->pos.theta);
			return p.x;
		}

		float getY() const {
			return getRadius() + tibia->pos.x;
		}

		float getZ() const {
			return femur->pos.x;
		}

		void setX(float cm) {
			Pos2D p = Pos2D(getRadius(), 0);
			p.setX(cm);
			coxa->pos.setTheta(p.theta);
		}

		void setY(float cm) {
			tibia->pos.setX(cm);
		}

		void setZ(float cm) {
			femur->pos.setX(cm);
			tibia->pos.setTheta(femur->pos.theta);
		}

		void moveX(float dist) {
			Pos2D p = Pos2D(getRadius(), 0);
			setX(p.x + dist);
		}

		void setXZ(float x, float z) {
			setZ(z);
			setX(x);
		}
		void setXYZ(float x, float y, float z) {
			setZ(z);
			setX(x);
			tibia->pos.moveX(-y);
		}

		void setYaw(float radians) {
			coxa->pos.setTheta(radians);
		}

		int8_x3_t getTargets(int quadrant) {
			int8_x3_t targets = {0,0,0};
			targets._1 = round((quadrant < 3) ? - (coxa->pos.relativeTheta() * TO_DEG) : (coxa->pos.relativeTheta() * TO_DEG));
			targets._2 = round(femur->pos.relativeTheta() * TO_DEG);
			targets._3 = round(tibia->pos.relativeTheta() * TO_DEG);
			return targets;
		}
};


#endif /* KNEE_H_ */
