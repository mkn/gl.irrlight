/*
 * screen.hpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */
/*
#include "irrlight/scene.ui.hpp"
*/


#include <irrlicht.h>

#ifndef _IRRLIGHT_UTILS_SCREEN_HPP_
#define _IRRLIGHT_UTILS_SCREEN_HPP_

using namespace irr;

namespace irrlight{

enum AGUISceneUI{
	GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000,
	GUI_ID_POSITION_TEXT,
	GUI_ID_BUTTON_SET_SCALE
};

class ScreenResolutionParameters{
    public:
        ScreenResolutionParameters();
        ~ScreenResolutionParameters();
};

class ScreenResolutionHandler{

    public:
        static void construct   (ScreenResolutionParameters * srp);
        static void destruct    (ScreenResolutionParameters * srp);
};

};
#endif /* _IRRLIGHT_UTILS_SCREEN_HPP_ */