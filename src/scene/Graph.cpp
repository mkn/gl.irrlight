/*
 * Graph.cpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
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



