/*
 * blinky.cpp
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */
#include <iostream>
#include "character.h"
#include "ghost.h"
#include "blinky.h"
using namespace std ;

Blinky::Blinky(int xx,int yy)
{
	x=xx;
	y=yy;
	colr=84;
}


Blinky::~Blinky()
{

}


float Blinky::getColr() const {
		return colr;
	}

	void Blinky::setColr(float colr) {
		this->colr = colr;
	}
