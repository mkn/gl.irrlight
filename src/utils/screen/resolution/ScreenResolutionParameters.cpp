
// Author 	: Philip Deegan
// Date		:
// License	:



#include "utils/screen/resolution/ScreenResolutionParameters.hpp"
#include "utils/screen/resolution/handler/ScreenResolutionHandler.hpp"


ScreenResolutionParameters::ScreenResolutionParameters(){
    ScreenResolutionHandler::construct(this);

}

ScreenResolutionParameters::~ScreenResolutionParameters(){}

