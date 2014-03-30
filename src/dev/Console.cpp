/*
 * Console.cpp
 *
 *  Created on: Oct 04, 2013
 *	  Author: philip
 */

#include "irrlight/dev/console.hpp"

void irrlight::dev::Console::draw	 (irr::IrrlichtDevice * device){
	irr::core::rect<int> rect(0, 0, 400, 300);
	irr::video::IVideoDriver * driver	= device->getVideoDriver();
	driver->draw2DRectangle(irr::video::SColor(200,0,0,0), rect);
}
void irrlight::dev::Console::rollDown (irr::IrrlichtDevice * device){	
	irr::gui::IGUIEnvironment * env	 = device->getGUIEnvironment();	
	irr::core::rect<int> commandRect(0, 280, 400, 300);	
	irr::core::rect<int> commandHistoryRect(0, 0, 400, 300);
	env->addStaticText(L"HISTORY", commandHistoryRect, false, true, env->getRootGUIElement(), irrlight::dev::ConsoleUI::COMMAND__HISTORY_BOX, false);	
	env->addEditBox(L"text", commandRect, true, env->getRootGUIElement(), COMMAND_BOX)->setText(L"");	
}

void irrlight::dev::Console::rollUp   (irr::IrrlichtDevice * device){
	device->getGUIEnvironment()->getRootGUIElement()->removeChild(
			device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(COMMAND_BOX, true));
	device->getGUIEnvironment()->getRootGUIElement()->removeChild(
				device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(COMMAND__HISTORY_BOX, true));
}

const wchar_t * irrlight::dev::Console::getText(irr::IrrlichtDevice *device) const{
	return ((irr::gui::IGUIEditBox *)device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(COMMAND_BOX, true))->getText();
}

void irrlight::dev::Console::setText(irr::IrrlichtDevice *device, const wchar_t* text){
	((irr::gui::IGUIEditBox *)device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(COMMAND_BOX, true))->setText(text);
}
