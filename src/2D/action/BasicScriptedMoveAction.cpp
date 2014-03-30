/*
 * BasicScriptedIsoMoveAction.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
 
#include "irrlight/2D/iso/action.hpp"

void irrlight::BasicScriptedIsoMoveAction::setup(IsoPlanePath* path, AIsoActorBase* base) {

	int x = base->getX(), y = base->getY();

	path->pushPath(IsoPlanePathLocation((x							), y));
	path->pushPath(IsoPlanePathLocation((x = x - base->getSize() * 2), y));
	path->pushPath(IsoPlanePathLocation((x							), y));
	base->setColour(0, 255, 0);
	path->pushPath(IsoPlanePathLocation((x = x + base->getSize() * 2), y));
	path->pushPath(IsoPlanePathLocation((x = x + base->getSize() * 2), y));
	path->pushPath(IsoPlanePathLocation((x							), y));
	base->setColour(255, 0, 0);
	path->pushPath(IsoPlanePathLocation((x = x - base->getSize() * 2), y));
	path->pushPath(IsoPlanePathLocation((x							), y));
	base->setColour(0, 0, 255);
	path->pushPath(IsoPlanePathLocation( x, (y = y - base->getSize() * 2)));
	path->pushPath(IsoPlanePathLocation( x, (y							)));
	base->setColour(0, 0, 0);
	path->pushPath(IsoPlanePathLocation( x, (y = y + base->getSize() * 2)));
	path->pushPath(IsoPlanePathLocation( x, (y							)));
	path->pushPath(IsoPlanePathLocation( x, (y = y + base->getSize() * 2)));
	path->pushPath(IsoPlanePathLocation( x, (y							)));
	base->setColour(255, 255, 0);
	path->pushPath(IsoPlanePathLocation( x, (y = y - base->getSize() * 2)));
	path->pushPath(IsoPlanePathLocation( x, (y							)));
}
