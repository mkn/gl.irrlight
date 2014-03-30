/*
 * IsometricActionService.cpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
 
#include "irrlight/2D/iso/action.hpp"

irrlight::IsometricActionService* irrlight::IsometricActionService::instance(0);

irrlight::IsometricActionService* irrlight::IsometricActionService::getInstance(){return instance == 0 ? (instance = new IsometricActionService()) : instance;}
