/*
 * plane.hpp
 *
 *  Created on: Oct 05, 2013
 *      Author: philip
 */
/*
#include "irrlight/2D/plane.hpp"
*/

#ifndef _IRRLIGHT_2D_PLANE_HPP_
#define _IRRLIGHT_2D_PLANE_HPP_

#include "memory"

namespace irrlight{

class Plane{

	public:
		virtual ~Plane(){}
		virtual const unsigned int width()  const = 0;
		virtual const unsigned int height() const = 0;

		//This is used for collision detection -|- ALSO DESTROY IT YOURSELF -|- 1 = occupied pixel / 0 = unoccupied -|- easy eh?
		virtual bool ** getGrid() const = 0;
};

};
#endif /* _IRRLIGHT_2D_PLANE_HPP_ */
