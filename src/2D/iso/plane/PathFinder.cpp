/*
 * IsoPlanePathFinder.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */

#include "irrlight/2D/iso/path.hpp"

irrlight::IsoPlanePath* irrlight::IsoPlanePathFinder::find(IsoPlanePath* path, AIsoActorBase* base, bool** grid){
	using namespace path;
	int startX 	= path->getStart()	->getX(), startY 	= path->getStart()	->getY();
	int endX 	= path->getEnd()	->getX(), endY 		= path->getEnd()	->getY();

	path::Direction d;

	while(true){
		if(startX - endX > -base->getSize() && startX - endX < base->getSize()){
			if		(endY - startY > 0)					{ d = DOWN; }
			else										{ d = UP;	}
		}else if(startX - endX > 0){
			if		(startY - endY < -base->getSize())	{ d = DOWNLEFT;}
			else if	(startY - endY > base->getSize())	{ d = UPLEFT;}
			else										{ d = LEFT;}

		}else{
			if		(startY - endY < -base->getSize())	{ d = DOWNRIGHT;}
			else if	(startY - endY > base->getSize())	{ d = UPRIGHT;}
			else	  									{ d = RIGHT;}
		}
		if(pushAndCheck(d, path, base, startX, endX, startY, endY)) break;
	}

	return path;
}
bool irrlight::IsoPlanePathFinder::pushAndCheck(const path::Direction& d, IsoPlanePath* path, AIsoActorBase* base, int& x1, int& x2, int& y1, int& y2){
	using namespace path;
	switch(d){
		case LEFT: 		{path->pushPath(IsoPlanePathLocation((x1 = x1 - base->getSize()),  y1)); 						 return isAcceptable(path, base, x2, x1,  0,  0);}
		case UPLEFT: 	{path->pushPath(IsoPlanePathLocation((x1 = x1 - base->getSize()), (y1 = y1 - base->getSize()))); return isAcceptable(path, base, x2, x1, y2, y1);}
		case UP: 		{path->pushPath(IsoPlanePathLocation(x1							, (y1 = y1 - base->getSize()))); return isAcceptable(path, base,  0,  0, y2, y1);}
		case UPRIGHT: 	{path->pushPath(IsoPlanePathLocation((x1 = x1 + base->getSize()), (y1 = y1 - base->getSize()))); return isAcceptable(path, base, x1, x2, y2, y1);}
		case RIGHT: 	{path->pushPath(IsoPlanePathLocation((x1 = x1 + base->getSize()),  y1)); 						 return isAcceptable(path, base, x1, x2,  0,  0);}
		case DOWNRIGHT: {path->pushPath(IsoPlanePathLocation((x1 = x1 + base->getSize()), (y1 = y1 + base->getSize()))); return isAcceptable(path, base, x1, x2, y1, y2);}
		case DOWN: 		{path->pushPath(IsoPlanePathLocation(x1							, (y1 = y1 + base->getSize()))); return isAcceptable(path, base,  0,  0, y1, y2);}
		case DOWNLEFT: 	{path->pushPath(IsoPlanePathLocation((x1 = x1 - base->getSize()), (y1 = y1 + base->getSize()))); return isAcceptable(path, base, x2, x1, y1, y2);}
		default: throw;
	}
}

bool irrlight::IsoPlanePathFinder::isAcceptable(IsoPlanePath* path, AIsoActorBase* base, int x1, int x2, int y1, int y2){

	return abs(x2) - abs(x1) - base->getSize() <= 0	&& abs(y2) - abs(y1) - base->getSize() <= 0;
}
