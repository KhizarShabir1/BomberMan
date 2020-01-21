/*
 * pinky.h
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */

#ifndef PINKY_H_
#define PINKY_H_

#include <iostream>
#include "character.h"
#include "ghost.h"
using namespace std ;
class Pinky:public Ghost
{
public:
	Pinky(int x=425+60*4,int y=125+60*2);
	~Pinky();
};





#endif /* PINKY_H_ */
