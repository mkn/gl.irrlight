/*
 * AIsoPlaneCollider.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */

#include "irrlight/2D/isometric.hpp"

irrlight::AIsoPlaneCollider*  irrlight::AIsoPlaneCollider::create(const irrlight::IsoPlaneCollisionType& type) {
	
	switch(type){
		case POINT:
			return new IsoPlanePointCollider();
		default:
			throw;
	}
}
