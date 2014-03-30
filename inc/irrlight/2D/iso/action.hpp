/*
 * action.hpp
 *
 *  Created on: Oct 05, 2013
 *	  Author: philip
 */
/*
#include "irrlight/2D/iso/action.hpp"
*/

#ifndef _IRRLIGHT_2D_ACTION_HPP_
#define _IRRLIGHT_2D_ACTION_HPP_

#include "kul/hash.hpp"
#include "kul/threads.hpp"

#include "irrlight/2D/iso/path.hpp"
#include "irrlight/2D/iso/actor.hpp"

namespace irrlight{
/*
class CouldNotDeduceAutonomousActionException : public kul::Exception{
	public:
		CouldNotDeduceAutonomousActionException(const char*f, const int& l, std::string s) : kul::Exception(f, l, s){}
};
*/
enum IsometricActionType{
	NOOP = 0,
	WAIT, // implies thread
	MOVE
};

class AIsometricAction {
	public:
		virtual ~AIsometricAction(){}
		virtual void tick(IsoActor * actor) = 0;
		virtual const IsometricActionType getActionType() const = 0;
		AIsometricAction& operator=(int &);
};

class IsometricNOOPAction : public AIsometricAction{
	public:
		IsometricNOOPAction() {}
		~IsometricNOOPAction(){}

		void tick(IsoActor * actor){}
		IsometricNOOPAction& operator=(int &);

		const IsometricActionType getActionType() const { return NOOP; }
};

class IsoMoveAction : public AIsometricAction{
	protected:
		bool ** grid;
		IsoPlanePath path;
		AIsoActorBase* base;
		kul::Ref<IsoMoveAction> ref;
		kul::Thread thread;
		IsoMoveAction() : ref(*this), thread(ref){ thread.run(); }
	public:
		IsoMoveAction(bool ** grid, IsoPlanePathLocation * start, IsoPlanePathLocation * end, AIsoActorBase* base) 
				: grid(grid), path(start, end), base(base), ref(*this), thread(ref){ thread.run();}
		~IsoMoveAction(){}
		void tick(IsoActor * actor);
		bool threadFinished(){
			return thread.finished();
		}
		const IsometricActionType getActionType() const { return MOVE; }
		void operator()(){
			KLOG(INFO);
			IsoPlanePathFinder::find(&path, base, grid);
		}
};

class BasicScriptedIsoMoveAction : public IsoMoveAction{
	protected:
		BasicScriptedIsoMoveAction(){}
		static void setup(IsoPlanePath* path, AIsoActorBase* base);
	public:
		BasicScriptedIsoMoveAction(AIsoActorBase* base) : IsoMoveAction(){}
		~BasicScriptedIsoMoveAction(){}
		const IsometricActionType getActionType() const { return MOVE; }
		bool threadFinished(){
			return thread.finished();
		}
};

class IsometricActionService{
	private:
		//google::sparse_hash_map<IsoActor*, AIsometricAction*, std::tr1::hash<IsoActor*>, IsoActorComparator> actors;
		kul::hash::Map<IsoActor*, AIsometricAction*, std::tr1::hash<IsoActor*>, IsoActorComparator> actors;
		IsometricActionService() {}
		static IsometricActionService * instance;
	public:
		static IsometricActionService * getInstance();
		void putActorWithNOOP(IsoActor * actor) { getOrCreateActionFor(actor, new IsometricNOOPAction());}
		bool hasActor(IsoActor * actor){
			return actors[actor];
		}
		AIsometricAction * getActionForActor(IsoActor * actor){
			return actors[actor];
		}
		AIsometricAction * getOrCreateActionFor(IsoActor * actor, AIsometricAction * action){
			if(actors[actor] == action) return actors[actor];
			delete actors[actor];
			actors[actor] = action;
			if(actors[actor]) return actors[actor];
			throw std::runtime_error("No subclass of AIsometricAction defined for "/* + actor*/);
		}
};

class AutonomousActionDeductionService{
	public:
		static AIsometricAction* deduceActionOrThrow(AutoIsoActor& actor) {
			return new BasicScriptedIsoMoveAction(actor.getBase());
			//throw CouldNotDeduceAutonomousActionException(__FILE__, __LINE__, "Cannot not deduce action for actor");
		}
};

};
#endif /* _IRRLIGHT_2D_ACTION_HPP_ */
