/*
 * blinky.h
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */

#ifndef BLINKY_H_
#define BLINKY_H_
#include <iostream>
#include "character.h"
#include "ghost.h"
using namespace std ;
class Blinky:public Ghost
{
	float colr;
public:
	Blinky(int x=425,int y=125);

	virtual ~Blinky();

	float getColr() const ;

	void setColr(float colr);
};




#endif /* BLINKY_H_ */
