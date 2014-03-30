/*
 * actor.hpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
/*
#include "irrlight/2D/iso/actor.hpp"
*/

#ifndef _IRRLIGHT_2D_ACTOR_HPP_
#define _IRRLIGHT_2D_ACTOR_HPP_

#include "irrlight/2D/plane.hpp"
#include "irrlight/2D/iso/actor/base.hpp"

namespace irrlight{ 

class IsoActor{
	protected:
		std::auto_ptr<AIsoActorBase> base;
		bool selected;
		long UUID;
	public:
		IsoActor(AIsoActorBase* base);
		IsoActor(irr::core::vector2d<int> point, const unsigned int& r, const unsigned int& g, const unsigned int& b);
        int getX() { return base->getX();}
        int getY() { return base->getY();}
        AIsoActorBase* getBase() { return base.get(); }
        long getUUID() 		const	{ return UUID; }
        bool isSelected() 	const 	{ return selected; }
        void select() 				{ this->selected = true; }
        void unSelect() 			{ this->selected = false; }
        void draw(irr::IrrlichtDevice* device, Plane* plane);
        IsoActor& operator=(IsoActor &);
        ~IsoActor(){}
};

struct IsoActorComparator{
	public:
		bool operator()(IsoActor * ia1, IsoActor * ia2) const{
			return (ia1->getUUID() == ia2->getUUID()) || (ia1 && ia2);
		}
};

class AutoIsoActor : public IsoActor {
	public:
		AutoIsoActor(irr::core::vector2d<int> point, const unsigned int& r, const unsigned int& g, const unsigned int& b);
};

};
#endif /* _IRRLIGHT_2D_ACTOR_HPP_ */
