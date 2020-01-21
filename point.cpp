/*
 * point.cpp
 *
 *  Created on: May 10, 2017
 *      Author: khizar
 */
#include <iostream>
#include <cmath>
#include "point.h"
using namespace std ;

Point::Point(int xx,int yy)
{
	x=xx;
	y=yy;
}
void Point::reset()
{
	x=0;
	y=0;
}
Point& Point::operator=(const Point & p)
{
	//if(this==&p)
		//return *this;
	x=p.getX();
	y=p.getY();
	return *this;
}
bool Point::operator==(const Point &p)
		{
	if(x==p.getX()&& y==p.getY())
		return true ;
	else
		return false;
		}

Point::Point(const Point & p)
{
	*this=p;
}

Point& Point::getPoint()
{
	return *this;
}
int Point::getX() const {
	return  x;
}

void Point::setX(int x) {
	this->x = x;
}

int Point::getY() const {
	return y;
}

void Point::setY(int y) {
	this->y = y;
}
ostream& operator<<(ostream & cout,const Point& p)
{
	cout<<"x = "<<p.getX()<<" y= "<<p.getY()<<'\n';
	return cout;
}

//double Distance(Point &p1,Point &p2)
//{
//	double dis=0;
//	int xx=0,yy=0;
//	xx=p1.getX()-p2.getX();
//	xx=pow(xx,2);
//	yy=p1.getY()-p2.getY();
//	xx=pow(xx,2);
//	dis=xx+yy;
//	dis=sqrt(dis);
//	return dis;
//}
