/**

./src/Terminal.cpp

Created on: 16 May 2013

Copyright (c) 2013, Philip Deegan

This file is part of irrlight.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "irrlight.hpp"

void irrlight::Terminal::draw	 (irr::IrrlichtDevice * device){
	irr::core::rect<int> rect(0, 0, 400, 300);
	irr::video::IVideoDriver * driver	= device->getVideoDriver();
	driver->draw2DRectangle(irr::video::SColor(200,0,0,0), rect);
}
void irrlight::Terminal::rollDown (irr::IrrlichtDevice * device){	
	irr::gui::IGUIEnvironment * env	 = device->getGUIEnvironment();	
	irr::core::rect<int> commandRect(0, 280, 400, 300);	
	irr::core::rect<int> commandHistoryRect(0, 0, 400, 300);
	env->addStaticText(L"HISTORY", commandHistoryRect, false, true, env->getRootGUIElement(), irrlight::TerminalUI::COMMAND__HISTORY_BOX, false);	
	env->addEditBox(L"text", commandRect, true, env->getRootGUIElement(), COMMAND_BOX)->setText(L"");	
}

void irrlight::Terminal::rollUp   (irr::IrrlichtDevice * device){
	device->getGUIEnvironment()->getRootGUIElement()->removeChild(
			device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(COMMAND_BOX, true));
	device->getGUIEnvironment()->getRootGUIElement()->removeChild(
				device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(COMMAND__HISTORY_BOX, true));
}

const wchar_t * irrlight::Terminal::getText(irr::IrrlichtDevice *device) const{
	return ((irr::gui::IGUIEditBox *)device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(COMMAND_BOX, true))->getText();
}

void irrlight::Terminal::setText(irr::IrrlichtDevice *device, const wchar_t* text){
	((irr::gui::IGUIEditBox *)device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(COMMAND_BOX, true))->setText(text);
}
