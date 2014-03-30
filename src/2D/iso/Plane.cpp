/*
 * IsoPlane.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */

#include "irrlight/2D/isometric.hpp"

#include "time.h"
#include "fstream"

//#include "boost/shared_array.hpp"
//#include "boost/lexical_cast.hpp"

irrlight::IsoPlane::IsoPlane(IsoPlaneCollisionType& type, const unsigned int& w, const unsigned int& h, std::string fileLocation)
: w(w) , h(h) ,
  grid(buildGrid(fileLocation)),
  detector(AIsoPlaneCollider::create(type)){
	actorControllerMap[controller::PLAYER_CHARACTERS]		= new IsoActorController();
	actorControllerMap[controller::NON_PLAYER_CHARACTERS] 	= new AutoIsoActorController();
	actorControllerMap[controller::ENEMIES] 				= new AutoIsoActorController();
}

irrlight::IsoPlane::IsoPlane(const unsigned int& w, const unsigned int& h)
: w(w) , h(h) ,
  grid(buildGrid()),
  detector(AIsoPlaneCollider::create(POINT)){
	actorControllerMap[controller::PLAYER_CHARACTERS] 		= new IsoActorController();
	actorControllerMap[controller::NON_PLAYER_CHARACTERS] 	= new AutoIsoActorController();
	actorControllerMap[controller::ENEMIES] 				= new AutoIsoActorController();
}

void irrlight::IsoPlane::pauseUnPauseControllers(){
	for(int i = controller::NON_PLAYER_CHARACTERS ; i <= controller::ENEMIES; i++)
		actorControllerMap[static_cast<controller::Type>(i)]->pause();
}

//Is seriously fooked FIXME
bool** irrlight::IsoPlane::buildGrid(std::string fileLocation){

	bool** array = new bool*[w];
	for(int i = 0; i < w; i++)
		array[i] = new bool[h];

	std::ifstream inFile(fileLocation.c_str());
	std::string line;

	int j = 0;
	while(inFile.good()){
		int k = 0;
		getline(inFile, line);

		KLOG(INFO) << line;
		std::string bit = "";
		for(unsigned int i = 0; i < line.size(); i++){
			if(line[i] == 'Y' || line[i] == 'N'){
				//TODO WTF IS THIS
				/*
				try{
					int count = boost::lexical_cast< int >( bit );
					for(; k < count + 1; k++){
						array[j][k] = (line[i] == 'Y') ? true : false;
					}
				}
				catch(const boost::bad_lexical_cast & ) {  std::cout << "ERROR building mapOverlay, invalid File" << std::endl; exit(1);}
				*/
				bit = "";
			}else{
				bit += line[i];
			}
		}
		j++;
	}
	inFile.close();

	return array;
}
bool** irrlight::IsoPlane::buildGrid(){
	bool** array = new bool*[w];
	for(int i = 0; i < w; i++)
		array[i] = new bool[h];

	srand(time(0));

	for(int i = 0; i < w; i ++){
		for(int j = 0; j < h; j ++){
			array[i][j] = (rand()) % 121 == 0 ? 1 : 0;
		}
	}
	return array;
}

void irrlight::IsoPlane::tick(/* GameContext */ irr::IrrlichtDevice *device){

	irr::video::IVideoDriver * driver    = device->getVideoDriver();

	if(console().isShown()){
		// PAUSED = true;
	}

	for(int i = 0; i < w; i ++){
		for(int j = 0; j < h; j ++){
			int colour = grid[i][j] ? 0 : 255;
			driver->drawPixel(i, j, irr::video::SColor(255, colour, colour, colour));
		}
	}
	
	for(int i = controller::NON_PLAYER_CHARACTERS ; i <= controller::ENEMIES; i++){
		std::vector<IsoActor *>& actors = actorControllerMap[static_cast<controller::Type>(i)]->getActors();
		for(uint i = 0; i < actors.size(); i++){
			actors[i]->draw(device, this);
		}		
	}

	if(console().isShown()){
		console().draw(device);
	}	
}

bool irrlight::IsoPlane::triggerEvent(irr::IrrlichtDevice *device, const irr::SEvent& event, const IsometricActionType& action){
	irr::core::position2d<int> mousePos		= device->getCursorControl()->getPosition();
	std::vector<IsoActor *>& actors 	= getPlayerController().getActors();
	for(uint i = 0; i < actors.size(); i++){
		if(!actors[i]->isSelected()) continue;
		switch(action){

			case NOOP: break;
			case MOVE:
				IsometricActionService::getInstance()->getOrCreateActionFor(actors[i],
						new IsoMoveAction(this->getGrid(),
								new IsoPlanePathLocation(actors[i]->getX(), actors[i]->getY()),
								new IsoPlanePathLocation(mousePos.X, mousePos.Y), actors[i]->getBase())); break;

			case WAIT: break;
		}
	}
	return false;
}

// TODO if shift down - don't unselect
bool irrlight::IsoPlane::characterSelected(irr::IrrlichtDevice *device /* bool shiftDown*/){
	irr::core::position2d<int> mousePos		= device->getCursorControl()->getPosition();
	std::vector<IsoActor *>& actors 	= getPlayerController().getActors();
	uint selected = actors.size();

	for(uint i = 0; i < actors.size(); i++){
		if(actors[i]->getBase()->isSelected(mousePos.X, mousePos.Y)){
			actors[i]->select();
			selected = i; break;
		}
	}

	//if(shiftDown && selected > 0) return true;

	if(selected < actors.size()){
		for(uint i = 0; i < actors.size(); i++){
			if(i == selected) continue;
			actors[i]->unSelect();
		}

		return true;
	}

	return false;
}

irrlight::controller::Type& operator++(irrlight::controller::Type& obj)	{
	using namespace irrlight::controller;
	switch(obj){
		case NON_PLAYER_CHARACTERS 	: obj = PLAYER_CHARACTERS;  	break;
		case PLAYER_CHARACTERS 		: obj = ENEMIES; 				break;
		case ENEMIES 				: obj = NON_PLAYER_CHARACTERS; 	break;
		default: assert(false); break;
	}
	return obj;
}
