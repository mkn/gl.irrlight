/*
 * SquareIsoActorBase.cpp
 *
 *  Created on: 14 Mar 2012
 *      Author: dekken
 */

#include "irrlight/2D/iso/actor/base.hpp"

irrlight::SquareIsoActorBase::~SquareIsoActorBase(){}

void irrlight::SquareIsoActorBase::draw(irr::IrrlichtDevice* device, bool selected){
	irr::video::IVideoDriver * driver    = device->getVideoDriver();

	irr::core::rect<int> rect(x, y, x + size, y + size);

	driver->draw2DRectangle(irr::video::SColor(255, r, g, b), rect);
	if(selected){

		driver->draw2DRectangleOutline(rect, irr::video::SColor(255,0,0,0));
	}
}

bool irrlight::SquareIsoActorBase::isSelected(int x, int y){

	return x >= getX() && x <= getX() + getSize() && y >= getY() && y <= getY() + getSize();
}
