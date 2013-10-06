/*
 * graph.hpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */
/*
#include "kul/cgi/scene/graph.hpp"
*/

#ifndef _KUL_CGI_SCENE_GRAPH_HPP_
#define _KUL_CGI_SCENE_GRAPH_HPP_

#include "glog/logging.h"

#include <memory>

#include "irrlicht.h"

#include "kul/cgi/scene.hpp"

using namespace irr;

namespace kul { namespace cgi{  
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

};};
#endif /* _KUL_CGI_SCENE_GRAPH_HPP_ */
