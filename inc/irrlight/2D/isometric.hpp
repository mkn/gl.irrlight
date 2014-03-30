/*
 * isometric.hpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
/*
#include "irrlight/2D/isometric.hpp"
*/

#ifndef _IRRLIGHT_2D_ISO_HPP_
#define _IRRLIGHT_2D_ISO_HPP_

#include "irrlicht.h"

#include "kul/hash.hpp"
#include "irrlight/dev/console.hpp"
#include "irrlight/2D/iso/actor/controller.hpp"

namespace irrlight{

class AIsoPlaneCollider;

enum IsoPlaneCollisionType{
	POINT,
	TRIG,
	BOX
};

class IsoPlane : public Plane{
	private:
		kul::hash::Map<controller::Type, IsoActorController*, std::tr1::hash<controller::Type>, controller::Comparator> actorControllerMap;
		dev::Console con;
		const int w, h;
		bool ** grid;
		std::shared_ptr<AIsoPlaneCollider> detector;
		bool** buildGrid(std::string fileLocation);
		bool** buildGrid();
		void pauseUnPauseControllers();
		IsoPlane();
	public:
		IsoPlane(IsoPlaneCollisionType& type, const unsigned int& w, const unsigned int& h, std::string fileLocation);
		IsoPlane(const unsigned int& w, const unsigned int& h);

		const unsigned int width () const { return this->w; }
		const unsigned int height() const { return this->h; }

		bool** getGrid() const {return grid;}

		bool characterSelected(irr::IrrlichtDevice *device);
		bool triggerEvent(irr::IrrlichtDevice *device, const irr::SEvent& event, const IsometricActionType& action);

		void tick(irr::IrrlichtDevice *device);
		void showHideConsole(irr::IrrlichtDevice *device){ !console().isShown() ? console().show(device) : console().hide(device); pauseUnPauseControllers();}
		bool consoleShown() { return console().isShown(); }
		dev::Console& console() { return con; }

		AutoIsoActorController&	getNonPlayerController()	{ return (AutoIsoActorController&) *actorControllerMap[controller::NON_PLAYER_CHARACTERS];}
		AutoIsoActorController&	getEnemyController()		{ return (AutoIsoActorController&) *actorControllerMap[controller::ENEMIES];}
		IsoActorController&			getPlayerController()		{ return *actorControllerMap[controller::PLAYER_CHARACTERS];}

		AIsoPlaneCollider& getDetector()				{ return *detector.get(); }

		IsoPlane& operator=(IsoPlane &);

		~IsoPlane() {
			// delete
			for(int j = 0; j < w; j++) delete [] grid[j];
			delete [] grid;
		}
};

class AIsoPlaneCollider {
	public:
		virtual ~AIsoPlaneCollider(){}
		virtual irr::core::vector2d<int> getClosestCharacterPointOnGridOrThrow(int x, int y, Plane* plane ) = 0;
        static AIsoPlaneCollider* create(const IsoPlaneCollisionType& type);
};

class IsoPlanePointCollider : public AIsoPlaneCollider{
	public:
		~IsoPlanePointCollider();
		irr::core::vector2d<int> getClosestCharacterPointOnGridOrThrow(int x, int y, Plane* plane );
		IsoPlanePointCollider& operator=(int &);
};

};
#endif /* _IRRLIGHT_2D_ISO_HPP_ */
