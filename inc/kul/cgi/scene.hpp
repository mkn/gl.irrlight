/*
 * scene.hpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */
/*
#include "kul/cgi/scene.hpp"
*/
 
#ifndef _KUL_CGI_SCENE_HPP_
#define _KUL_CGI_SCENE_HPP_

#include "glog/logging.h"

using namespace irr;
using namespace irr::gui;
namespace kul { namespace cgi{ 

class AScene{
    public:
        virtual ~AScene(){};
        virtual bool OnEvent    (IrrlichtDevice *device, const SEvent& event) = 0;
        virtual void setUp      (IrrlichtDevice *device) = 0;
        virtual void tearDown   (IrrlichtDevice *device) = 0;
        virtual void draw       (IrrlichtDevice *device) = 0;
        virtual bool keyDown    (IrrlichtDevice *device, irr::EKEY_CODE keyCode) = 0;
        virtual bool keyUp      (IrrlichtDevice *device, irr::EKEY_CODE keyCode) = 0;
};

class AGUIScene : public AScene {
public:
	virtual ~AGUIScene(){};
	virtual void setUpUI(IGUIEnvironment* env) = 0;
};

};};
#endif /* _KUL_CGI_SCENE_HPP_ */
