/*
 * IsoMoveAction.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
 
#include "irrlight/2D/iso/action.hpp"

void irrlight::IsoMoveAction::tick(IsoActor * actor){
	if(!threadFinished()) return;

	IsoPlanePathLocation& next(path.pop());

	actor->getBase()->setX(next.getX());
	actor->getBase()->setY(next.getY());

	if(path.reached(actor)){
		IsometricActionService::getInstance()->putActorWithNOOP(actor);
	}
}
