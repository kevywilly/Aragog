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
			return coxa->radius+femur->y;
		}

		float getX() const {
			Pos2D p = Pos2D(getRadius(), 0);
			p.setTheta(coxa->theta);
			return p.x;
		}

		float getY() const {
			return getRadius() + tibia->x;
		}

		float getZ() const {
			return femur->x;
		}

		void setX(float cm) {
			Pos2D p = Pos2D(getRadius(), 0);
			p.setX(cm);
			coxa->setTheta(p.theta);
		}

		void setY(float cm) {
			tibia->setX(cm);
		}

		void setZ(float cm) {
			femur->setX(cm);
			tibia->setTheta(femur->theta);
		}



		void moveX(float dist) {

			Pos2D p = Pos2D(getRadius(), 0);
			setX(p.x + dist);

		}

		void setXYZ(float x, float y, float z) {
			setX(x);
			setZ(z);
			tibia->moveX(-y);
		}

		void setYaw(float radians) {
			coxa->setTheta(radians);
		}

		int8T3 getTargets(int quadrant) {
			int8T3 targets = {};
			targets._1 = round((quadrant < 3) ? - (coxa->theta * TO_DEG) : (coxa->theta * TO_DEG));
			targets._2 = round(femur->theta * TO_DEG);
			targets._3 = round(tibia->theta * TO_DEG);
			return targets;
		}
};


#endif /* KNEE_H_ */
