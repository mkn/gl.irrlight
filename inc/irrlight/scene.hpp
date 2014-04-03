/**

./inc/irrlight/scene.hpp

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
#ifndef _IRRLIGHT_SCENE_HPP_
#define _IRRLIGHT_SCENE_HPP_

#include <irrlicht.h>

using namespace irr;
using namespace irr::gui;
namespace irrlight{

class AScene{
	public:
		virtual ~AScene(){};
		virtual void setUp 		(IrrlichtDevice *device){}
		virtual void tearDown	(IrrlichtDevice *device){}
		virtual void draw		(IrrlichtDevice *device){}
		virtual bool OnEvent	(IrrlichtDevice *device, const SEvent& event)   { return false; }
		virtual bool keyDown	(IrrlichtDevice *device, irr::EKEY_CODE keyCode){ return false; }
		virtual bool keyUp		(IrrlichtDevice *device, irr::EKEY_CODE keyCode){ return false; }
};

class AGUIScene : public AScene {
	public:
		virtual ~AGUIScene(){};
		virtual void setUpUI(IGUIEnvironment* env) = 0;
};

};
#endif /* _IRRLIGHT_SCENE_HPP_ */
