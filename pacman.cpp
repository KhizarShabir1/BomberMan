/*
 * pacman.cpp
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */
#include <iostream>


#include "character.h"
#include "pacman.h"

using namespace std ;

Pacman::Pacman(int xx,int yy,int lif):Character()
{
	x=xx;
	y=yy;
	life=lif;
}

Pacman::~Pacman()
{

}

Point Pacman::getIndex()
{
	int x=this->getX();
	x=x/60;
	int y=this->getY();
	y=12-(y/60);
	cout << "Pacman GetIndex Called ()\n" ;
	cout << "Row Number :" << y << " " << " Col Number :" << x << endl ;
	Point p(y,x);
	return p;

}

int Pacman::getLife() const {
	return life;
}

void Pacman::setLife(int life) {
	this->life = life;
}
