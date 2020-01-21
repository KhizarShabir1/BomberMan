/*
 * inky.h
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */

#ifndef INKY_H_
#define INKY_H_
#include <iostream>
#include "character.h"
#include "ghost.h"
using namespace std ;
class Inky:public Ghost
{
public:
	Inky(int x=425+60*4,int y=545);
	~Inky();


};





#endif /* INKY_H_ */
