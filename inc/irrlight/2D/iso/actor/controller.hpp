/*
 * controller.hpp
 *
 *  Created on: Oct 05, 2013
 *	  Author: philip
 */
/*
#include "irrlight/2D/iso/actor/controller.hpp"
*/

#ifndef _IRRLIGHT_2D_ISO_ACTOR_CONTROLLER_HPP_
#define _IRRLIGHT_2D_ISO_ACTOR_CONTROLLER_HPP_

#include "vector"

#include "kul/threads.hpp"

#include "irrlight/2D/iso/actor.hpp"
#include "irrlight/2D/iso/action.hpp"
#include "irrlight/2D/iso/actor/controller.hpp"

namespace irrlight{ 

class IsoActorController {
	protected:
		bool isPaused;
		kul::Ref<IsoActorController> ref;
		std::shared_ptr<kul::Thread> thread;
		kul::Mutex mutex;
		std::vector<IsoActor*> actors;

		static void act(IsoActor * actor, AIsometricAction * action){
			KLOG(INFO);
			action->tick(actor);
			KLOG(INFO);
		}
	public:
		IsoActorController() 
				: isPaused(0), ref(*this), thread(new kul::Thread(ref)){ thread->run();}
		virtual ~IsoActorController(){}
		IsoActorController& operator=(IsoActorController &);
		std::vector<IsoActor*>& getActors() {return actors;}
		IsoActorController* addActor(IsoActor* actor){
			IsometricActionService::getInstance()->putActorWithNOOP(actor);
			actors.push_back(actor);
			return this;
		}
		virtual void pause(){
			kul::ScopeLock lock(mutex);
			if(isPaused){ 
				thread.reset(new kul::Thread(ref));
			}else{ 
				thread->interrupt(); /*thread.yield();*/ 
				thread->~Thread();
				thread.reset();
			}
			isPaused = !isPaused;			
		}
		virtual void operator()(){			
			while(true){
				{
					kul::ScopeLock lock(mutex);
						for(uint i = 0; i < actors.size(); i++)
							IsoActorController::act( actors[i], IsometricActionService::getInstance()->getActionForActor(actors[i]));	
				}				
				thread->sleep(1000);
			}			
		}
};

class AutoIsoActorController : public IsoActorController {
	private:
		AutoIsoActor* 			castActor(IsoActor* actor) 	{ return static_cast<AutoIsoActor*>(actor); }
		AutoIsoActorController* cast(IsoActorController* c) 	{ return static_cast<AutoIsoActorController*>(c); }
	public:
		//AutoIsoActorController(const AutoIsoActorController& c) : IsoActorController(c){}
		AutoIsoActorController() : IsoActorController(){ actors.reserve(10); }
		virtual ~AutoIsoActorController(){}
		AutoIsoActorController& operator=(AutoIsoActorController &);
		std::vector<AutoIsoActor*>& getActors() {return (std::vector<AutoIsoActor*>&) actors;}
		AutoIsoActorController* addActor(AutoIsoActor* actor){
			return cast(IsoActorController::addActor(actor));
		}
		virtual void pause(){
			IsoActorController::pause();
		}
		virtual void operator()(){			
			IsometricActionService& actionService = *IsometricActionService::getInstance();
			while(true){
				{
					kul::ScopeLock lock(mutex);
					for(uint i = 0; i < actors.size(); i++){
						//try{
						if(actionService.getActionForActor(actors[i])->getActionType() == IsometricActionType::NOOP)
							actionService.getOrCreateActionFor(actors[i], AutonomousActionDeductionService::deduceActionOrThrow(*castActor(actors[i])));
						IsoActorController::act( actors[i], IsometricActionService::getInstance()->getActionForActor(actors[i]));
						//}catch(CouldNotDeduceAutonomousActionException& e){ std::cout << e.what() << std::endl; }
					}
				}
				thread->sleep(1000);
			}
			
		}
};

namespace controller{ 
enum Type{
	NON_PLAYER_CHARACTERS = 0,
	PLAYER_CHARACTERS,
	ENEMIES,
};
inline Type& operator++(Type& obj)	{
	switch(obj){
		case NON_PLAYER_CHARACTERS	: obj = PLAYER_CHARACTERS;  	break;
		case PLAYER_CHARACTERS  	: obj = ENEMIES; 				break;
		case ENEMIES 				: obj = NON_PLAYER_CHARACTERS; 	break;
		default: assert(false); break;
	}
	return obj;
}
struct Comparator{
	public:
		bool operator()(Type ia1, Type ia2) const{
			return (ia1 == ia2) || (ia1 && ia2);
		}		
};
};


};

namespace std {
	namespace tr1 {
		template<> inline size_t hash<irrlight::controller::Type>::operator()(irrlight::controller::Type t) const {
			return t;
		}
	}
}

#endif /* _IRRLIGHT_2D_ISO_ACTOR_CONTROLLER_HPP_ */
