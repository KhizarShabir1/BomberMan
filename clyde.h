/*
 * clyde.h
 *
 *  Created on: May 20, 2017
 *      Author: khizar
 */

#ifndef CLYDE_H_
#define CLYDE_H_
#include <iostream>
#include "character.h"
#include "ghost.h"
using namespace std ;

class Clyde:public Ghost
{

public:
	Clyde(int x=485,int y=665);
		~Clyde();
};


#endif /* CLYDE_H_ */
