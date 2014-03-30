/*
 * scene.hpp
 *
 *  Created on: Oct 04, 2013
 *	  Author: philip
 */
/*
#include "irrlight/scene.hpp"
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
