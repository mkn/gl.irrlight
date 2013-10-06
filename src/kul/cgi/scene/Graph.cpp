/*
 * Graph.cpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */

#include "kul/cgi/scene/graph.hpp"

kul::cgi::SceneGraph* kul::cgi::SceneGraph::instance(0);

kul::cgi::SceneGraph::~SceneGraph(){}

kul::cgi::SceneGraph* kul::cgi::SceneGraph::getInstance(){
    if(instance == 0){
        instance = new SceneGraph();
    }
    return instance;
}

void kul::cgi::SceneGraph::draw(/*IDrawableContext idc*/) const{
    if(scene.get()){
        scene->draw(this->getDevice());
    }
}

void kul::cgi::SceneGraph::setDevice(IrrlichtDevice* device){
	this->device.reset(device);
}

void kul::cgi::SceneGraph::setScene(AScene * aScene){
    if(this->scene.get()){
        this->scene->tearDown(this->getDevice());
    }
    this->scene.reset(aScene);
    aScene->setUp(this->getDevice());
}

bool kul::cgi::SceneGraph::OnEvent(const SEvent& event){
    if(scene.get()){
        if		(event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown) 	scene->keyDown	(getDevice(), event.KeyInput.Key);
        else if	(event.EventType == EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown) scene->keyUp	(getDevice(), event.KeyInput.Key);
        return scene->OnEvent(getDevice(), event);
    }
    return false;
}



