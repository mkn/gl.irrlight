
// Author 	: Philip Deegan
// Date		:
// License	:

#include <iostream>

#include "irrlight/utils/screen.hpp"

void irrlight::ScreenResolutionHandler::construct(ScreenResolutionParameters * srp){
    IrrlichtDevice *device = createDevice(video::EDT_NULL, core::dimension2d<u32>(512, 384));

    video::IVideoModeList* modes = device->getVideoModeList();

    const core::dimension2d< u32 > currentRes = modes->getDesktopResolution();

    std::cout << "Current res width:  " << currentRes.Width  << std::endl;
    std::cout << "Current res height: " << currentRes.Height << std::endl;

    std::cout << "getVideoModeCount :" << modes->getVideoModeCount()  << std::endl;

    const int count = modes->getVideoModeCount();

    for(int i=0; i<count; i++){

        std::cout << "mode1Res res width:  " << modes->getVideoModeResolution(i).Width  << std::endl;
        std::cout << "mode1Res res width:  " << modes->getVideoModeResolution(i).Height  << std::endl;

    }


    const core::dimension2d< u32 > mode0Res = modes->getVideoModeResolution(0);

    std::cout << "mode0Res res width:  " << mode0Res.Width  << std::endl;
    std::cout << "mode0Res res height: " << mode0Res.Height << std::endl;


};

void irrlight::ScreenResolutionHandler::destruct(ScreenResolutionParameters * srp){};

