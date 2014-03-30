/*
 * A3DScene.hpp
 *
 *  Created on: 30 Dec 2012
 *      Author: philix
 */

#ifndef _A3DSCENE_HPP_
#define _A3DSCENE_HPP_

#include "scene/AScene.hpp"

class A3DScene : public AScene{

public:
    virtual ~A3DScene(){};

    virtual bool OnEvent    (IrrlichtDevice *device, const SEvent& event) = 0;

    virtual void setUp      (IrrlichtDevice *device) = 0;
    virtual void tearDown   (IrrlichtDevice *device) = 0;
    virtual void draw       (IrrlichtDevice *device) = 0;

    virtual bool keyDown    (IrrlichtDevice *device, irr::EKEY_CODE keyCode) = 0;
    virtual bool keyUp      (IrrlichtDevice *device, irr::EKEY_CODE keyCode) = 0;
};


#endif /* A3DSCENE_HPP_ */
