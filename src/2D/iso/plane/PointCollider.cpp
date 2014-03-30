/*
 * IsoPlanePointCollider.cpp
 *
 *  Created on: 14 Mar 2012
 *      Author: dekken
 */

#include "irrlight/2D/isometric.hpp"

irrlight::IsoPlanePointCollider::~IsoPlanePointCollider(){}

irr::core::vector2d<int> irrlight::IsoPlanePointCollider::getClosestCharacterPointOnGridOrThrow(int x, int y, Plane* plane){

	return irr::core::vector2d<int>(x, y);
	//throw CannotFindValidCharacterPositionException("Cannot find reasonably near position to place character");
}


