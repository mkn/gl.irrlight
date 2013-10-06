/*
 * thread.hpp
 *
 *  Created on: Oct 04, 2013
 *	  Author: philip
 */
/*
#include "kul/cgi/scene/thread.hpp"
*/
//TODO KILL BOOST

#ifndef _KUL_CGI_SCENE_THREAD_HPP_
#define _KUL_CGI_SCENE_THREAD_HPP_

#include "glog/logging.h"

#include "irrlicht.h"

#include "kul/except.hpp"
#include "kul/cgi/scene.hpp"


using namespace irr;
namespace kul { namespace cgi{ 

class SceneThreadException : public kul::Exception{
	public:
		SceneThreadException(const char*f, const int l, std::string s) : kul::Exception(f, l, s){}
};

class SceneThread  {
	public:
		SceneThread(AScene& aScene) throw(SceneThreadException) : aScene(aScene), device(irr::createDevice(video::EDT_NULL, core::dimension2d<u32>(512, 384))){
			this->check();
			//thread  = std::auto_ptr<boost::thread>(new boost::thread(*this));
		}
		~SceneThread() {/*
			if(thread.get()){
				std::cout << " DESTROYED" << std::endl;
			}*/
		}
		IrrlichtDevice * getDevice(){ return this->device.get();}
		AScene   * getScene()		{ return &this->aScene;}
		bool isFinished(){
			return true;//thread->timed_join(boost::posix_time::milliseconds(0));
		}
		void operator()() { go(); }

	protected:
		AScene& aScene;
		std::auto_ptr<IrrlichtDevice> device;
		//std::auto_ptr<boost::thread> thread;
		void go(){
			/*
			boost::xtime xt;
			boost::xtime_get(&xt, boost::TIME_UTC_);
			xt.sec += 1;

			thread->sleep(xt);
			this->aScene.setUp(this->getDevice());
			*/
		}

		SceneThread();

		void check() throw(SceneThreadException){
			if (!&aScene || !&device){
				throw SceneThreadException(__FILE__, __LINE__, "Scene or device cannot be null");
			}
		}

		// prevent copying and assignment; not implemented
		SceneThread& operator=(const AScene &);
		SceneThread& operator=(IrrlichtDevice &);
};

class GUISceneThread  : public SceneThread{
	public:
		GUISceneThread(AGUIScene & aGUIScene): SceneThread (aGUIScene){}
		~GUISceneThread(){}
		void operator()(){ 
			go(); 
		}
	protected:
		void go(){
			((AGUIScene*)this->getScene())->setUpUI(this->getDevice()->getGUIEnvironment());
		}
};

};};
#endif /* _KUL_CGI_SCENE_THREAD_HPP_ */
