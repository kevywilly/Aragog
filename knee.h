/*
 * knee.h
 *
 *  Created on: Jun 19, 2017
 *      Author: kevywilly
 */

#ifndef KNEE_H_
#define KNEE_H_

#include "Arduino.h"
#include "joint.h"
#include "rttypes.h"

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
		}

		void setZadj(float cm) {
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

		void setXZadj(float x, float z) {
			setZadj(z);
			setX(x);
		}


		void setYaw(float radians) {
			coxa->pos.setTheta(radians);
		}

		int8T3 getTargets(int quadrant) {
			int8T3 targets = {0,0,0};
			targets._1 = round((quadrant < 3) ? - degrees(coxa->pos.relativeTheta()) : degrees(coxa->pos.relativeTheta()));
			targets._2 = round(degrees(femur->pos.relativeTheta()));
			targets._3 = round(degrees(tibia->pos.relativeTheta()));
			return targets;
		}
};


#endif /* KNEE_H_ */
