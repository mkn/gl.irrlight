/*
 * path.hpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
/*
#include "irrlight/2D/iso/path.hpp"
*/

#ifndef _IRRLIGHT_2D_PATH_HPP_
#define _IRRLIGHT_2D_PATH_HPP_

#include "queue"

#include "irrlight/2D/iso/actor.hpp"
#include "irrlight/2D/iso/actor/base.hpp"

namespace irrlight{ 

class IsoPlanePathLocation{
	private:
		int x, y;
	public:
		IsoPlanePathLocation(int x, int y) : x(x), y(y){}
		const int& getX(){return x;}
		const int& getY(){ return y;}
};

class IsoPlanePath{
	private:
		std::shared_ptr<IsoPlanePathLocation> start;
		std::shared_ptr<IsoPlanePathLocation> end;
		std::queue<IsoPlanePathLocation> pathLocations;
	public:
		IsoPlanePath(IsoPlanePathLocation * start, IsoPlanePathLocation * end) : start(start), end(end){}
		IsoPlanePath(){}
		~IsoPlanePath(){}
		IsoPlanePathLocation* getStart(){ return start.get();}
		IsoPlanePathLocation* getEnd() 	{ return end.get();}
		bool reached(IsoActor* actor)		{ return pathLocations.empty();}
		IsoPlanePathLocation& pop()		{
			IsoPlanePathLocation& loc = pathLocations.front();
			pathLocations.pop();
			return loc;
		}
		void pushPath(IsoPlanePathLocation path){pathLocations.push(path);}
};

namespace path{
enum Direction{
	UPLEFT,		UP, 	UPRIGHT,
	LEFT,				RIGHT,
	DOWNLEFT,	DOWN,	DOWNRIGHT
};
};

class IsoPlanePathFinder{

	private:
		static bool isAcceptable(IsoPlanePath* path, AIsoActorBase* base, int x1, int x2, int y1, int y2);
		static bool pushAndCheck(const path::Direction& d, IsoPlanePath* path, AIsoActorBase* base, int& x1, int& x2, int& y1, int& y2);
	public:
		static IsoPlanePath* find(IsoPlanePath* path, AIsoActorBase* base, bool** grid);
};

};
#endif /* _IRRLIGHT_2D_PATH_HPP_ */
