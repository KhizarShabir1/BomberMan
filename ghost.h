/*
 * ghost.h
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */

#ifndef GHOST_H_
#define GHOST_H_
#include <iostream>
#include "character.h"
#include "point.h"
using namespace std ;

class Ghost:public Character
{

public:

	Ghost(int xx=0,int yy=0);
	Point getPosition();


	virtual ~Ghost();
};



#endif /* GHOST_H_ */
