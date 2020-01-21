/*
 * bomb.h
 *
 *  Created on: May 9, 2017
 *      Author: khizar
 */

#ifndef BOMB_H_
#define BOMB_H_
#include <iostream>
using namespace std;
class Bomb
{
private:
	int bmbx;
	int bmby;
	int bmbTime;
public:
	Bomb();
	int getBmbTime() const {
		return bmbTime;
	}

	void setBmbTime(int bmbTime) {
		this->bmbTime = bmbTime;
	}

	int getBmbx() const {
		return bmbx;
	}

	void setBmbx(int bmbx) {
		this->bmbx = bmbx;
	}

	int getBmby() const {
		return bmby;
	}

	void setBmby(int bmby) {
		this->bmby = bmby;
	}
};




#endif /* BOMB_H_ */
