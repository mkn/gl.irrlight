/**

./inc/irrlight/scene/graph.hpp

Created on: 16 May 2013

Copyright (c) 2013, Philip Deegan

This file is part of irrlight.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _IRRLIGHT_SCENE_GRAPH_HPP_
#define _IRRLIGHT_SCENE_GRAPH_HPP_

#include <memory>

#include "irrlicht.h"

#include "irrlight/scene.hpp"

using namespace irr;

namespace irrlight{ 
class SceneGraph : public IEventReceiver{
    private:
		std::shared_ptr<AScene> scene;
		std::shared_ptr<IrrlichtDevice> device;
        SceneGraph() : scene(0), device(irr::createDevice(video::EDT_NULL, core::dimension2d<u32>(512, 384))){}
        static SceneGraph *instance;
    public:
        ~SceneGraph();

        IrrlichtDevice  * getDevice()   const { return this->device.get();}
        AScene          * getScene()    const { return this->scene.get();}

        void setDevice  (IrrlichtDevice* device);
        void setScene   (AScene  * aScene);

        void draw       (/*IDrawableContext idc*/)  const;

        bool OnEvent    (const SEvent& event);

        static SceneGraph * getInstance();
};

};
#endif /* _IRRLIGHT_SCENE_GRAPH_HPP_ */
