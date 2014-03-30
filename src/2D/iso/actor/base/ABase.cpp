/*
 * AIsoActorBase.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */

#include "irrlight/2D/iso/actor/base.hpp"

irrlight::AIsoActorBase* irrlight::AIsoActorBase::create(
	const unsigned int& x, const unsigned int& y, const unsigned int& r, 
	const unsigned int& g, const unsigned int& b, 
	IsoActorBaseType base, int size){	
	switch(base){
		case CIRCLE:
			return new CircleIsoActorBase(x, y, r, g, b, size);
		case SQUARE:
			return new SquareIsoActorBase(x, y, r, g, b, size);
		case POLYGON:
			throw;
		default:
			throw;
	}
}
