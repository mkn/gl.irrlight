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
#include "irrlight.hpp"

irrlight::SceneGraph* irrlight::SceneGraph::instance(0);

irrlight::SceneGraph::~SceneGraph(){}

irrlight::SceneGraph* irrlight::SceneGraph::INSTANCE(){
    if(instance == 0){
        instance = new SceneGraph();
    }
    return instance;
}

void irrlight::SceneGraph::draw(/*IDrawableContext idc*/) const{
    if(s.get()){
        s->draw();
    }
}

void irrlight::SceneGraph::device(irr::IrrlichtDevice* device){
	this->d.reset(device);
}

void irrlight::SceneGraph::scene(AScene * aScene){
    if(this->s.get()){
        this->s->tearDown();
    }
    this->s.reset(aScene);
    aScene->setUp();
}

bool irrlight::SceneGraph::OnEvent(const irr::SEvent& event){
    if(s.get()){
        if		(event.EventType == irr::EET_KEY_INPUT_EVENT &&  event.KeyInput.PressedDown)    s->keyDown	(event.KeyInput.Key);
        else if	(event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)	s->keyUp	(event.KeyInput.Key);
        return s->OnEvent(event);
    }
    return false;
}



