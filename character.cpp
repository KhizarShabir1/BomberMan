/*
 * character.cpp
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */
#include <iostream>
#include "character.h"
using namespace std ;

void Character::getCoordinates(int &xx,int &yy)
{
	xx=x;
	yy=y;
}
Character::Character(int xx,int yy)
{
	x=xx;
	y=yy;
}
Character::~Character()
{

}
void Character::setX(int xx)
{
	x=xx;
}
void Character::setY(int yy)
{
	y=yy;
}
int Character::getX()
{
	return x;
}
int Character::getY()
{
	return y;
}
