/*
 * pacman.h
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */

#ifndef PACMAN_H_
#define PACMAN_H_
#include <iostream>
#include "character.h"
#include "point.h"
using namespace std ;
class Pacman :public Character
{
	int life;
public:
	Pacman(int x=90,int y=90,int life=4);
	virtual ~Pacman();
	Point getIndex();

	int getLife() const ;

	void setLife(int life);
};




#endif /* PACMAN_H_ */
