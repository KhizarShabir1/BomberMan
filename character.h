/*
 * character.h
 *
 *  Created on: May 4, 2017
 *      Author: khizar
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_
#include <iostream>
using namespace std ;

class Character{
protected:
	int x;
	int y;
public:
	Character(int xx=0,int yy=0);
	virtual void getCoordinates(int &xx,int &yy);
	virtual void setX(int x);
	virtual void setY(int y);
	virtual int getX();
	virtual int getY();

	virtual ~Character();
};





#endif /* CHARACTER_H_ */
