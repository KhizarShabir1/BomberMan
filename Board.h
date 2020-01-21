/*
 * Board.h
 *
 *  Created on: May 2, 2014
 *      Author: Sibt ul Hussain
 */
#ifndef _BOARD_H_
#define _BOARD_H_


#include <GL/glut.h>
#include <iostream>
#include "character.h"
#include "pacman.h"
#include "ghost.h"
#include "bomb.h"
#include "blinky.h"
#include "pinky.h"
#include "clyde.h"

#include "inky.h"

#include "util.h"

#include <queue>
//#define WITH_TEXTURES
using namespace std;
class Board {
private:


	int xcellsize, ycellsize;
	int width, height;
	ColorNames pcolor, bcolor, gcolor;


public:
	int **BfsArray;
	Pacman pacman;

	Bomb bomb;
	Blinky blinky ;
	Pinky pinky;
	Inky inky;
	Clyde clyde;
	int score;
	int nextDirec;
	int **arraypoint;
	queue<Point> bfs;
	int chk180;
	int chkDir;

	Point p2;
	int bmbclr;
	float dt,lastframe;
	int face;
	int point;
	int scatter;
	int bfstime;
	int bluetime;
	bool blueChk;
	int brickChek;
	int bmb;
	bool over;
	unsigned long long ftime=0;
	static const int BOARD_X;
	static const int BOARD_Y;
	int Inkypath();
	int clydeNeighbours();
	int ClydePath();
	int ScatterClyde();
	void moveClyde(bool & blue,bool & chgMod);
	int inkyNeighbours();
	int ScatterIlinky();
	void moveBlinky(bool & blue,bool & chgMod);
	void moveIinky(bool & blue,bool & chgMod);
	void movePinky(bool & blue,bool & chgMod);
	Board(int xsize = 8, int ysize = 8);
	void DisplayBFS();
	double Distance(Point p1,Point p2);

	void dispPoints(int **p);

	~Board(void);
	void InitializeBfs();
	void InitalizeBoard(int, int);
	//draw the board
	void Draw();
	int GhostNeighbours();
	void frontNeighbours(int row,int col);
	int BFsearch();
	int ScatterBlinky();
	int ScatterPinky();
	int Pinkypath();
	int PinkyNeighbours();
	int turn;
	bool changeMode;
	int changeModetime;

	static int GetBoardX() {
		return BOARD_X;
	}
	static int GetBoardY() {
		return BOARD_Y;
	}
	int GetMidX() {
		return BOARD_X * xcellsize / 2.0;
	}
	int GetMidY() {
		return BOARD_Y * ycellsize / 2.0;
	}
	int GetCellSize() {
		return xcellsize;
	}
	void GetInitBombermanPosition(int &x, int &y) {
		x = xcellsize + xcellsize / 2;
		y = ycellsize + ycellsize / 2;
	}
	void Display(int **p);
	bool removePoints(int **p,int x,int y);

	void GetInitTextPosition(int &x, int &y);
	void GetInitPinkyPosition(int &x, int &y);
	bool bombRadius(int x,int y,int &score);
	bool ifNillLeft(int x,int y);
	bool ifNillRight(int x,int y);
	bool ifNillUp(int x,int y);
	bool ifNillDown(int x,int y);
	/////////
	bool ifNullLeft(int x,int y);
	bool ifNullRight(int x,int y);
	bool ifNullUp(int x,int y);
	bool ifNullDown(int x,int y);
	//void removePoint(Pacman &p);
	//bool CHeckNeighbours(Character &b,int arr[]);
	//	int GetMidx
};

#ifdef WITH_TEXTURES
void RegisterTextures();
#endif

#endif
