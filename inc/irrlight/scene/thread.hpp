/*
 * thread.hpp
 *
 *  Created on: Oct 04, 2013
 *	  Author: philip
 */
/*
#include "irrlight/scene/thread.hpp"
*/
//TODO KILL BOOST

#ifndef _IRRLIGHT_SCENE_THREAD_HPP_
#define _IRRLIGHT_SCENE_THREAD_HPP_

#include "irrlicht.h"

#include "kul/except.hpp"
#include "kul/threads.hpp"
#include "irrlight/scene.hpp"

using namespace irr;
namespace irrlight{

class SceneThreadException : public kul::Exception{
	public:
		SceneThreadException(const char*f, const int l, std::string s) : kul::Exception(f, l, s){}
};

class SceneThread  {
	public:
		SceneThread(AScene& aScene) throw(SceneThreadException) 
				: aScene(aScene), device(irr::createDevice(video::EDT_NULL, core::dimension2d<u32>(512, 384))), ref(*this), thread(ref){
			this->check();
			thread.run();
		}
		~SceneThread() {
			thread.interrupt();
		}
		IrrlichtDevice * getDevice(){ return this->device.get();}
		AScene   * getScene()		{ return &this->aScene;}
		bool isFinished(){
			return thread.finished();
		}
		void operator()() { go(); }

	protected:
		AScene& aScene;
		std::shared_ptr<IrrlichtDevice> device;
		kul::Ref<SceneThread> ref;
		kul::Thread thread;
		void go(){
			KLOG(INFO);
			thread.sleep(1000);
			this->aScene.setUp(this->getDevice());
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

};
#endif /* _IRRLIGHT_SCENE_THREAD_HPP_ */
