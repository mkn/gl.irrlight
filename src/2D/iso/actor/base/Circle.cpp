/*
 * CircleIsoActorBase.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
 
#include "irrlight/2D/iso/actor/base.hpp"

irrlight::CircleIsoActorBase::~CircleIsoActorBase(){}

void irrlight::CircleIsoActorBase::draw(irr::IrrlichtDevice* device, bool selected){}

bool irrlight::CircleIsoActorBase::isSelected(int x, int y){
	return false;
}
