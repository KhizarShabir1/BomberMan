/*
 * point.h
 *
 *  Created on: May 10, 2017
 *      Author: khizar
 */

#ifndef POINT_H_
#define POINT_H_
#include <iostream>
using namespace std ;
class Point
{
private:
	int x;
	int y;
public:
	Point(int xx=0,int yy=0);
	Point(const Point & p);
	int getX() const ;
	void setX(int x);
	Point& getPoint();
	int getY() const;
	void setY(int y) ;
	void reset();
	//double Distance(Point &p1,Point &p2);
	Point& operator=(const Point & p);
	bool operator==(const Point &p);
};
ostream& operator<<(ostream & cout,const Point& p);




#endif /* POINT_H_ */
