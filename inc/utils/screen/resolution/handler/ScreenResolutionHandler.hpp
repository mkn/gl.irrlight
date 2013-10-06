
// Author 	: Philip Deegan
// Date		:
// License	:

#ifndef _SCREENRESOLUTIONHANDLER_HPP_
#define _SCREENRESOLUTIONHANDLER_HPP_

#include <irrlicht.h>

#include "utils/screen/resolution/ScreenResolutionParameters.hpp"

using namespace irr;

class ScreenResolutionHandler{

    public :
        static void construct   (ScreenResolutionParameters * srp);
        static void destruct    (ScreenResolutionParameters * srp);
};


#endif
