/*
 * AutoIsoActor.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
 
#include "irrlight/2D/iso/actor.hpp"

irrlight::AutoIsoActor::AutoIsoActor(irr::core::vector2d<int> point, const unsigned int& r, const unsigned int& g, const unsigned int& b) : IsoActor(
	AIsoActorBase::create(point.X, point.Y, r, g, b, SQUARE, 15)){
}
