/*
 * ghost.cpp
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */
#include <iostream>
using namespace std ;
#include "character.h"
#include "ghost.h"
using namespace std ;

Ghost::Ghost(int xx,int yy):Character(0,0)
{
x=xx;
y=yy;


}


Ghost::~Ghost()
{

}

Point Ghost::getPosition()
{
	int x=this->getX();
	x=x/60;
	int y=this->getY();
	y=12-(y/60);

	Point p(y,x);
	return p;

}
