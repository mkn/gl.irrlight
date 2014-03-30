/*
 * IsoActor.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
 
#include "irrlight/2D/iso/actor.hpp"

#include "time.h"

irrlight::IsoActor::IsoActor(AIsoActorBase* base) : base(base), selected(false){

	srand(time(0));
	UUID = rand() ^ rand();
}

irrlight::IsoActor::IsoActor(irr::core::vector2d<int> point, const unsigned int& r, const unsigned int& g, const unsigned int& b) :
		  base(AIsoActorBase::create(point.X, point.Y, r, g, b, SQUARE, 15)), selected(false){

	srand(time(0));
	UUID = rand() ^ rand();
}

void irrlight::IsoActor::draw(irr::IrrlichtDevice* device, Plane* plane){
	base->draw(device, isSelected());
}
