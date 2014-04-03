/**

./src/scene/Graph.cpp

Created on: 16 May 2013

Copyright (c) 2013, Philip Deegan

This file is part of irrlight.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "irrlight/scene/graph.hpp"

irrlight::SceneGraph* irrlight::SceneGraph::instance(0);

irrlight::SceneGraph::~SceneGraph(){}

irrlight::SceneGraph* irrlight::SceneGraph::getInstance(){
    if(instance == 0){
        instance = new SceneGraph();
    }
    return instance;
}

void irrlight::SceneGraph::draw(/*IDrawableContext idc*/) const{
    if(scene.get()){
        scene->draw(this->getDevice());
    }
}

void irrlight::SceneGraph::setDevice(IrrlichtDevice* device){
	this->device.reset(device);
}

void irrlight::SceneGraph::setScene(AScene * aScene){
    if(this->scene.get()){
        this->scene->tearDown(this->getDevice());
    }
    this->scene.reset(aScene);
    aScene->setUp(this->getDevice());
}

bool irrlight::SceneGraph::OnEvent(const SEvent& event){
    if(scene.get()){
        if		(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown) 	scene->keyDown	(getDevice(), event.KeyInput.Key);
        else if	(event.EventType == EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown) scene->keyUp	(getDevice(), event.KeyInput.Key);
        return scene->OnEvent(getDevice(), event);
    }
    return false;
}



