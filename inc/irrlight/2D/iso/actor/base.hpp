/*
 * base.hpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
/*
#include "irrlight/2D/iso/actor/base.hpp"
*/
#ifndef _IRRLIGHT_2D_ACTOR_BASE_HPP_
#define _IRRLIGHT_2D_ACTOR_BASE_HPP_

#include "irrlicht.h"

namespace irrlight{ 

enum IsoActorBaseType{
	SQUARE = 0,
	CIRCLE,
	POLYGON
};

class AIsoActorBase {
	protected:
		unsigned int x, y;
		unsigned int r,g,b;
		unsigned int size;
	public:
		static AIsoActorBase* create(
			const unsigned int& x, const unsigned int& y, 
			const unsigned int& r, const unsigned int& g, const unsigned int& b, 
			IsoActorBaseType base, int size);
		AIsoActorBase(const unsigned int& x, const unsigned int& y, int size) : x(x), y(y), r(0), g(0),b(255), size(size){}
		AIsoActorBase(
			const unsigned int& x, const unsigned int& y, const unsigned int& r, 
			const unsigned int& g, const unsigned int& b, int size) : x(x), y(y), r(r), g(g),b(b), size(size){}
        virtual ~AIsoActorBase(){}
        virtual void draw(irr::IrrlichtDevice* device, bool selected) = 0;
        virtual bool isSelected(int x, int y) = 0;
        void setColour(int r, int g, int b) { this->r = r; this->g = g; this->b = b; }
        int getX() { return x;} void setX(int x) { this->x = x;}
        int getY() { return y;} void setY(int y) { this->y = y;}
        int getSize() { return size; }        
        AIsoActorBase& operator=(AIsoActorBase &);
};

class SquareIsoActorBase : public AIsoActorBase{
	private:
		SquareIsoActorBase();
	public:
		SquareIsoActorBase(
			const unsigned int& x, const unsigned int& y, 
			const unsigned int& r, const unsigned int& g, const unsigned int& b, int size) : AIsoActorBase(x, y, r, g, b, size){}
        ~SquareIsoActorBase();
        void draw(irr::IrrlichtDevice* device, bool selected);
        bool isSelected(int x, int y);
};

class CircleIsoActorBase : public AIsoActorBase{
	private:
		CircleIsoActorBase();
	public:
		CircleIsoActorBase(
			const unsigned int& x, const unsigned int& y, 
			const unsigned int& r, const unsigned int& g, const unsigned int& b, int size) : AIsoActorBase(x, y, r, g, b, size){}
        ~CircleIsoActorBase();
        void draw(irr::IrrlichtDevice* device, bool selected);
        bool isSelected(int x, int y);
};

};
#endif /* _IRRLIGHT_2D_ACTOR_BASE_HPP_ */
