/*
 * Board.cpp
 *
 *  Created on: May 2, 2014
 *      Author: Sibt ul Hussain
 */

#include "Board.h"
#include "character.h"
#include "pacman.h"
#include "ghost.h"
#include "bomb.h"

#include <cstdio>

const int Board::BOARD_X = 17;
const int Board::BOARD_Y = 14;
//here's Bomberman's crazy board

// Note that all these enum constants from NILL onwards
// have been given numbers in increasing order
// e.g. NILL=0, and so on
// and these numbers are represented in the board array...
enum BoardParts {
	NILL, S_BRICK, G_BRICK, R_BRICK
};
// defining some utility functions...

 int board_array[Board::BOARD_Y][Board::BOARD_X] = { { 0 } };

#ifdef WITH_TEXTURES
const int nbricks = 3;

GLuint texture[nbricks];
GLuint tid[nbricks];
string tnames[] = {"solid.png", "brick.png", "brick-green.png"};
GLuint mtid[nbricks];
int cwidth = 60, cheight = 60; // 60x60 pixels bricks...

void RegisterTextures()
/*Function is used to load the textures from the
 * files and display*/
{
	// allocate a texture name
	glGenTextures(nbricks, tid);

	vector<unsigned char> data;
	//ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each bricks data...

	for (int i = 0; i < nbricks; ++i) {

		// Read current brick

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			//ofile.write((char*) &length, sizeof(int));
		}
		//ofile.write((char*) &data[0], sizeof(char) * data.size());

		cout << " Texture Id=" << tid[i] << endl;
		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
				wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cwidth, cheight, GL_RGB,
				GL_UNSIGNED_BYTE, &data[0]);
	}
	//ofile.close();

}
void Drawbrick(const BoardParts &cname, float fx, float fy, float fwidth,
		float fheight)
/*Draws a specfic brick at given position coordinate
 * sx = position of x-axis from left-bottom
 * sy = position of y-axis from left-bottom
 * cwidth= width of displayed brick in pixels
 * cheight= height of displayed bricki pixels.
 * */
{

	glPushMatrix();
	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname - 1]);
	//	glTranslatef(0, 0, 0);
	//	glRotatef(-M_PI / 2, 0, 0, 1);
	glBegin (GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
#endif
// Destructor
Board::~Board(void) {
	for(int i=0;i<14;i++)
	{
		delete [] BfsArray[i];
	}
	delete []BfsArray;
}
void Board::InitializeBfs()
{

	for (int i = 0; i < BOARD_Y-1 ; ++i) {
		for (int j = 0; j < BOARD_X; ++j) {
			if(board_array[i][j]!=0)
				BfsArray[i][j]=5;

			else
				BfsArray[i][j]=0;

		}
	}

}
void Board::DisplayBFS()
{

	cout<<"BFS ARRAY \n";
	for (int i = 0; i < BOARD_Y-1 ; ++i) {
		for (int j = 0; j < BOARD_X; ++j) {
			cout<<BfsArray[i][j]<<" ";
		}
		cout<<endl;
	}
}



void Board::InitalizeBoard(int w, int h) {
	width = w;
	height = h;
	for (int i = 0; i < BOARD_Y-1 ; ++i) {
		for (int j = 0; j < BOARD_X; ++j) {
			if (i == 0 || i == BOARD_Y - 2 || j == 0 || j == BOARD_X - 1){
				board_array[i][j] = S_BRICK;
				cout<<board_array[i][j]<<" ";}
			else if (i % 2 == 0 && j % 2 == 0){
				board_array[i][j] = S_BRICK;
				cout<<board_array[i][j]<<" ";}
			else{
				board_array[i][j] =
						(GetRandInRange(0, 10)) < 8 ? NILL :
								(GetRandInRange(0, 10)) < 8 ? G_BRICK : R_BRICK;
				cout<<board_array[i][j]<<" ";
			}
		}
		cout<<endl;
	}

}
//Constructor
Board::Board(int xsize, int ysize):pacman(90,90,4),bomb(),blinky(),pinky(),inky(),clyde() {
	xcellsize = xsize;
	ycellsize = ysize;
	pcolor = CHOCOLATE;
	bcolor = ORANGE_RED;
	gcolor = PINK;
	BfsArray=new int*[14];
	for(int i=0;i<14;i++)
	{
		BfsArray[i]=new int[17];
	}
	score=0;
	nextDirec=6;

	arraypoint=new int*[14];
	for(int i=0;i<14;i++)
	{
		arraypoint[i]=new int[17];
	}
	chk180=0;
	chkDir=0;
	bmbclr=0;
	dt=0;
	lastframe=0;
	face=1;
	point=0;
	scatter=0;
	bfstime=0;
	bluetime=0;
	blueChk=false;
	turn=0;
	changeMode=false;
	changeModetime=0;
	brickChek=0;
	bmb=0;
  over=false;
	ftime=0;
	//InitalizeBoard()f
}

void Board::Draw() {
	glColor3f(0, 0, 1);
	glPushMatrix();

#ifdef WITH_TEXTURES
	for (int i = BOARD_Y-2 , y = 0; i >= 0; --i, y += xcellsize) {
		for (int j = 0, x = 0; j < BOARD_X; j++, x += (ycellsize)) {
			//			cout << " " << board_array[i][j] << " " << flush;
			switch (board_array[i][j]) {
			case NILL:
				// Empty space
				break;
			case S_BRICK:
			case G_BRICK:
			case R_BRICK:
				float fwidth = (float) (xcellsize) / width * 2, fheight =
						(float) ycellsize / height * 2;
				float fx = (float) (x - 10) / width * 2 - 1, fy = (float) y
						/ height * 2 - 1;

				/*Drawbrick((BoardParts) board_array[i][j], fx, fy, fwidth,
				 fheight);*/
				Drawbrick((BoardParts) board_array[i][j], (float)x/width-1, (float)y/height-1, fwidth,
						fheight);
				break;
			}
		}
	}
#else
	for (int i = BOARD_Y - 2, y = 0; i >= 0; --i, y += xcellsize) {
		for (int j = 0, x = 0; j < BOARD_X; j++, x += (ycellsize)) {
			//			cout <<      " " << board_array[i][j] << " " << flush;
			switch (board_array[i][j]) {
			case NILL:
			{

			}

			break;
			case S_BRICK:
				DrawRectangle(x , y, ycellsize, xcellsize,
						colors[SLATE_GRAY]);
				//DrawLine(x - 10, y, x - 10 + ycellsize, y, 4, colors[BLACK]);
				break;
			case G_BRICK:
				DrawRectangle(x, y, ycellsize, xcellsize,
						colors[LIGHT_GREEN]);
				break;
			case R_BRICK:
				DrawRectangle(x , y, ycellsize, xcellsize, colors[RED]);
				break;
			}
		}
	}
#endif
	glPopMatrix();

}

bool Board::removePoints(int **g,int x,int y)
{

	x=x/60;
	y=12-(y/60);
	if(g[y][x]==7)
	{
		g[y][x]=0;
		blueChk=true;
	}
	if(g[y][x]==4)
	{
		g[y][x]=0;

		return true;

	}
	else
		return false;

}


void Board::Display(int **p)
{
	for (int i = BOARD_Y - 2, y = 0; i >= 0; --i, y += xcellsize) {
		for (int j = 0, x = 0; j < BOARD_X; j++, x += (ycellsize)) {
			switch (board_array[i][j]) {
			case NILL:
			{
				if(j!=0 ){

					//DrawCircle(x+30,y+30, 3, colors[WHITE]);
					p[i][j]=4;
				}
				p[3][1]=7;
				p[11][6]=7;
				p[10][15]=7;
				p[1][11]=7;

			}



			}
		}
	}
}

void Board::dispPoints(int **p)
{for (int i = 12, y = 0; i >= 0; --i, y += 60) {
	for (int j = 0, x = 0; j < 17; j++, x += (60)) {
		if(p[i][j]==4)
		{
			DrawCircle(x+30,y+30, 3, colors[WHITE]);
		}
		if(p[i][j]==7)
		{
			DrawCircle(x+30,y+30, 8, colors[WHITE]);
		}

	}
}
}

double Board::Distance(Point p1,Point p2)
{
	double dis=0;
	int xx=0,yy=0;
	xx=p1.getX()-p2.getX();
	xx=pow(xx,2);
	yy=p1.getY()-p2.getY();
	yy=pow(yy,2);
	dis=xx+yy;
	dis=sqrt(dis);
	return dis;
}

void Board::frontNeighbours(int row,int col)
{

	cout << "Front Neigbours Called\n";

	cout << "COL NUMBER  :"<< col  << "\nRow Number :" << row << endl;


	if(ifNullUp(col,row))
	{

		BfsArray[row-1][col]=BfsArray[row][col];


		bfs.push(Point(row-1,col));


	}

	if(ifNullDown(col,row))
	{

		BfsArray[row+1][col]=BfsArray[row][col];


		bfs.push(Point(row+1,col));

	}

	if(ifNullLeft(col,row))
	{


		BfsArray[row][col-1]=BfsArray[row][col];

		bfs.push(Point(row,col-1));

	}

	if(ifNullRight(col,row))
	{


		BfsArray[row][col+1]=BfsArray[row][col];

		bfs.push(Point(row,col+1));

	}


}
int Board::PinkyNeighbours()
{


	int x=0;
	int y=0;

	int i=0;
	pinky.getCoordinates(x,y);
	x=x/60;
	y=12-(y/60);

	pinky.getCoordinates(x,y);

	if(ifNillUp(x,y))
	{
		//y+=60;
		x=x/60;
		y=12-y/60;

		BfsArray[y-1][x]=4;


		bfs.push(Point(y-1,x));
		i++;


	}
	x=0,y=0;
	pinky.getCoordinates(x,y);

	if(ifNillDown(x,y))
	{
		y=12-(y/60);
		x=x/60;
		BfsArray[y+1][x]=1;
		cout <<"down neighbour added GN" <<BfsArray[y+1][x] << " In BFS Array\n";
		bfs.push(Point(y+1,x));
		i++;
	}
	x=0;y=0;
	pinky.getCoordinates(x,y);

	if(ifNillLeft(x,y))
	{
		y=12-(y/60);
		x=x/60;
		//x-=60;
		BfsArray[y][x-1]=2;
		cout <<"left neighbour added GN" <<BfsArray[y][x-1] << " In BFS Array\n";
		bfs.push(Point(y,x-1));
		i++;
	}
	x=0;y=0;
	pinky.getCoordinates(x,y);

	if(ifNillRight(x,y))
	{

		y=12-(y/60);
		x=x/60;

		BfsArray[y][x+1]=3;
		cout <<"Right neighbour added GN" <<BfsArray[y][x+1] << " In BFS Array\n";
		bfs.push(Point(y,x+1));
		i++;
	}
	return i;
}
int Board::inkyNeighbours()
{


	int x=0;
	int y=0;

	int i=0;
	inky.getCoordinates(x,y);
	x=x/60;
	y=12-(y/60);

	inky.getCoordinates(x,y);

	if(ifNillUp(x,y))
	{
		//y+=60;
		x=x/60;
		y=12-y/60;

		BfsArray[y-1][x]=4;


		bfs.push(Point(y-1,x));
		i++;


	}
	x=0,y=0;
	inky.getCoordinates(x,y);

	if(ifNillDown(x,y))
	{
		y=12-(y/60);
		x=x/60;
		BfsArray[y+1][x]=1;
		cout <<"down neighbour added GN" <<BfsArray[y+1][x] << " In BFS Array\n";
		bfs.push(Point(y+1,x));
		i++;
	}
	x=0;y=0;
	inky.getCoordinates(x,y);

	if(ifNillLeft(x,y))
	{
		y=12-(y/60);
		x=x/60;
		//x-=60;
		BfsArray[y][x-1]=2;
		cout <<"left neighbour added GN" <<BfsArray[y][x-1] << " In BFS Array\n";
		bfs.push(Point(y,x-1));
		i++;
	}
	x=0;y=0;
	inky.getCoordinates(x,y);

	if(ifNillRight(x,y))
	{

		y=12-(y/60);
		x=x/60;

		BfsArray[y][x+1]=3;
		cout <<"Right neighbour added GN" <<BfsArray[y][x+1] << " In BFS Array\n";
		bfs.push(Point(y,x+1));
		i++;
	}
	return i;
}
int Board::clydeNeighbours()
{


	int x=0;
	int y=0;

	int i=0;
	clyde.getCoordinates(x,y);
	x=x/60;
	y=12-(y/60);

	clyde.getCoordinates(x,y);

	if(ifNillUp(x,y))
	{
		//y+=60;
		x=x/60;
		y=12-y/60;

		BfsArray[y-1][x]=4;


		bfs.push(Point(y-1,x));
		i++;


	}
	x=0,y=0;
	clyde.getCoordinates(x,y);

	if(ifNillDown(x,y))
	{
		y=12-(y/60);
		x=x/60;
		BfsArray[y+1][x]=1;
		cout <<"down neighbour added GN" <<BfsArray[y+1][x] << " In BFS Array\n";
		bfs.push(Point(y+1,x));
		i++;
	}
	x=0;y=0;
	clyde.getCoordinates(x,y);

	if(ifNillLeft(x,y))
	{
		y=12-(y/60);
		x=x/60;
		//x-=60;
		BfsArray[y][x-1]=2;
		cout <<"left neighbour added GN" <<BfsArray[y][x-1] << " In BFS Array\n";
		bfs.push(Point(y,x-1));
		i++;
	}
	x=0;y=0;
	clyde.getCoordinates(x,y);

	if(ifNillRight(x,y))
	{

		y=12-(y/60);
		x=x/60;

		BfsArray[y][x+1]=3;
		cout <<"Right neighbour added GN" <<BfsArray[y][x+1] << " In BFS Array\n";
		bfs.push(Point(y,x+1));
		i++;
	}
	return i;
}
int Board::GhostNeighbours()
{


	int x=0;
	int y=0;

	int i=0;
	blinky.getCoordinates(x,y);
	x=x/60;
	y=12-(y/60);
	cout<<"GHOST neighbour called \n";
	cout << " blinky COL NUMBER  :"<< x  << "\nRow Number :" << y << endl;
	blinky.getCoordinates(x,y);

	if(ifNillUp(x,y))
	{
		//y+=60;
		x=x/60;
		y=12-y/60;

		BfsArray[y-1][x]=4;
		cout <<"up neighbour added GN" <<BfsArray[y-1][x] << " In BFS Array\n";

		bfs.push(Point(y-1,x));
		i++;


	}
	x=0,y=0;
	blinky.getCoordinates(x,y);

	if(ifNillDown(x,y))
	{
		y=12-(y/60);
		x=x/60;
		BfsArray[y+1][x]=1;
		cout <<"down neighbour added GN" <<BfsArray[y+1][x] << " In BFS Array\n";
		bfs.push(Point(y+1,x));
		i++;
	}
	x=0;y=0;
	blinky.getCoordinates(x,y);

	if(ifNillLeft(x,y))
	{
		y=12-(y/60);
		x=x/60;
		//x-=60;
		BfsArray[y][x-1]=2;
		cout <<"left neighbour added GN" <<BfsArray[y][x-1] << " In BFS Array\n";
		bfs.push(Point(y,x-1));
		i++;
	}
	x=0;y=0;
	blinky.getCoordinates(x,y);

	if(ifNillRight(x,y))
	{

		y=12-(y/60);
		x=x/60;

		BfsArray[y][x+1]=3;
		cout <<"Right neighbour added GN" <<BfsArray[y][x+1] << " In BFS Array\n";
		bfs.push(Point(y,x+1));
		i++;
	}
	return i;
}


void Board::moveClyde(bool & blue,bool & chgMod){
	int step=0;
	if(blue==false && chgMod==false){
		step=ClydePath();
	}
	if(blue==true && turn==0 )
	{
		int tur=0;
		tur=ClydePath();
		if(tur==4)
		{
			step=1;
		}
		else if(tur==1)
		{
			step=4;
		}
		else if(tur==2)
		{
			step=3;
		}
		else if(tur==3)
		{
			step=2;
		}
		turn++;

	}

	if(blue==true || chgMod==true  )
	{
		step=ScatterClyde();
	}





	if(step==4)
	{


		clyde.setY( clyde.getY()+60);

	}
	else if(step==1)
	{
		clyde.setY(clyde.getY()-60);
	}
	else if(step==2)
	{
		clyde.setX( clyde.getX()-60);
	}
	else if(step==3)
	{
		clyde.setX( clyde.getX()+60);
	}


}

void Board::moveBlinky(bool & blue,bool & chgMod){
	int step=0;
	if(blue==false && chgMod==false){
		step=BFsearch();
	}
	if(blue==true && turn==0 )
	{
		int tur=0;
		tur=BFsearch();
		if(tur==4)
		{
			step=1;
		}
		else if(tur==1)
		{
			step=4;
		}
		else if(tur==2)
		{
			step=3;
		}
		else if(tur==3)
		{
			step=2;
		}
		turn++;

	}

	if(blue==true || chgMod==true  )
	{
		step=ScatterBlinky();
	}





	if(step==4)
	{


		blinky.setY( blinky.getY()+60);

	}
	else if(step==1)
	{
		blinky.setY( blinky.getY()-60);
	}
	else if(step==2)
	{
		blinky.setX(blinky.getX()-60);
	}
	else if(step==3)
	{
		blinky.setX( blinky.getX()+60);
	}


}

void Board::moveIinky(bool & blue,bool & chgMod){
	int step=0;
	if(blue==false && chgMod==false){
		step=Inkypath();
	}
	if(blue==true && turn==0 )
	{
		int tur=0;
		tur=Inkypath();
		if(tur==4)
		{
			step=1;
		}
		else if(tur==1)
		{
			step=4;
		}
		else if(tur==2)
		{
			step=3;
		}
		else if(tur==3)
		{
			step=2;
		}
		turn++;

	}

	if(blue==true || chgMod==true  )
	{
		step=ScatterIlinky();
	}





	if(step==4)
	{


		inky.setY( inky.getY()+60);

	}
	else if(step==1)
	{
		inky.setY( inky.getY()-60);
	}
	else if(step==2)
	{
		inky.setX( inky.getX()-60);
	}
	else if(step==3)
	{
		inky.setX( inky.getX()+60);
	}


}
void Board::movePinky(bool & blue,bool & chgMod){
	int step=0;
	if(blue==false  && chgMod==false){
		step=Pinkypath();
	}
	if(blue==true && turn==0 )
	{
		int tur=0;
		tur=Pinkypath();
		if(tur==4)
		{
			step=1;
		}
		else if(tur==1)
		{
			step=4;
		}
		else if(tur==2)
		{
			step=3;
		}
		else if(tur==3)
		{
			step=2;
		}
		turn++;

	}

	if(blue==true || chgMod==true )
	{
		step=ScatterPinky();
	}




	if(step==4)
	{


		pinky.setY( pinky.getY()+60);

	}
	else if(step==1)
	{
		pinky.setY( pinky.getY()-60);
	}
	else if(step==2)
	{
		pinky.setX( pinky.getX()-60);
	}
	else if(step==3)
	{
		pinky.setX( pinky.getX()+60);
	}


}






int Board::ClydePath()
{
	double dis=0;

	double minDis=5000;
	int minDisDirec=10;
	int stx=0,sty=0;
	clyde.getCoordinates(stx,sty);
	stx=stx/60;
	sty=12-(sty/60);
	//bfs.push(Point(sty,stx));
	//frontNeighbours(sty,stx,10);
	clydeNeighbours();
	while (!bfs.empty() )
	{

		Point p1;
		p1=bfs.front();

		int col=0,row=0;
		row=p1.getX();
		col=p1.getY();

		frontNeighbours(row,col);

		//			//return b->BfsArray[p2.getX()][p2.getY()];
		//		return minDisDirec;
		int x=0;
		x=pacman.getX();

		x=x/60;
		cout<<"Pacman in bfs get colx "<<x<<" called \n";
		int y=0;
		y=pacman.getY();

		y=12-(y/60);

		dis=Distance(p1,Point(y,x+2));
		if(dis<minDis)
		{
			minDis=dis;
			//			yy=12-(yy/60);
			//			xx=xx/60;
			minDisDirec=BfsArray[row][col];
		}
		//bfs.pop();
		bfs.pop();
		if(bfs.empty())
		{
			Point p2;
			//			p2=b->pacman.getIndex();
			//
			while(!bfs.empty()) bfs.pop();
			//			//return b->BfsArray[p2.getX()][p2.getY()];
			//		return minDisDirec;

			return minDisDirec;

		}


	}

}
int Board::BFsearch()
{
	double dis=0;

	double minDis=5000;
	int minDisDirec=10;
	int stx=0,sty=0;
	blinky.getCoordinates(stx,sty);
	stx=stx/60;
	sty=12-(sty/60);
	//bfs.push(Point(sty,stx));
	//frontNeighbours(sty,stx,10);
	GhostNeighbours();
	while (!bfs.empty() )
	{

		Point p1;
		p1=bfs.front();

		int col=0,row=0;
		row=p1.getX();
		col=p1.getY();

		frontNeighbours(row,col);

		dis=Distance(p1,pacman.getIndex());
		if(dis<minDis)
		{
			minDis=dis;
			//			yy=12-(yy/60);
			//			xx=xx/60;
			minDisDirec=BfsArray[row][col];
		}
		//bfs.pop();
		bfs.pop();
		if(bfs.empty())
		{
			Point p2;
			//			p2=b->pacman.getIndex();
			//
			while(!bfs.empty()) bfs.pop();
			//			//return b->BfsArray[p2.getX()][p2.getY()];
			//		return minDisDirec;
			int x=0;
			x=pacman.getX();

			x=x/60;
			cout<<"Pacman in bfs get colx "<<x<<" called \n";
			int y=0;
			y=pacman.getY();

			y=12-(y/60);
			cout<<"Pacman in bfs get rowy "<<y<<" called \n";
			return BfsArray[y][x];

		}


	}

}
int Board::Pinkypath()
{
	InitializeBfs();
	double dis=0;

	double minDis=5000;
	int minDisDirec=10;
	int stx=0,sty=0;
	pinky.getCoordinates(stx,sty);
	stx=stx/60;
	sty=12-(sty/60);
	//bfs.push(Point(sty,stx));
	//frontNeighbours(sty,stx,10);
	PinkyNeighbours();
	while (!bfs.empty() )
	{

		Point p1;
		p1=bfs.front();

		int col=0,row=0;
		row=p1.getX();
		col=p1.getY();

		frontNeighbours(row,col);

		dis=Distance(p1,Point(1,1));
		if(dis<minDis)
		{
			minDis=dis;
			//			yy=12-(yy/60);
			//			xx=xx/60;
			minDisDirec=BfsArray[row][col];
		}
		//bfs.pop();
		bfs.pop();
		if(bfs.empty())
		{

			//			p2=b->pacman.getIndex();
			//
			while(!bfs.empty()) bfs.pop();
			//			//return b->BfsArray[p2.getX()][p2.getY()];
			//		return minDisDirec;


			return minDisDirec;

		}


	}

}
int Board::Inkypath()
{
	InitializeBfs();
	double dis=0;

	double minDis=5000;
	int minDisDirec=10;
	int stx=0,sty=0;
	inky.getCoordinates(stx,sty);
	stx=stx/60;
	sty=12-(sty/60);
	//bfs.push(Point(sty,stx));
	//frontNeighbours(sty,stx,10);
	inkyNeighbours();
	while (!bfs.empty() )
	{

		Point p1;
		p1=bfs.front();

		int col=0,row=0;
		row=p1.getX();
		col=p1.getY();

		frontNeighbours(row,col);

		dis=Distance(p1,Point(11,1));
		if(dis<minDis)
		{
			minDis=dis;
			//			yy=12-(yy/60);
			//			xx=xx/60;
			minDisDirec=BfsArray[row][col];
		}
		//bfs.pop();
		bfs.pop();
		if(bfs.empty())
		{

			//			p2=b->pacman.getIndex();
			//
			while(!bfs.empty()) bfs.pop();
			//			//return b->BfsArray[p2.getX()][p2.getY()];
			//		return minDisDirec;


			return BfsArray[11][1];

		}


	}

}
int Board::ScatterBlinky()
{
	double dis=0;

	double minDis=5000;
	int minDisDirec=10;
	int stx=0,sty=0;
	blinky.getCoordinates(stx,sty);
	stx=stx/60;
	sty=12-(sty/60);
	//bfs.push(Point(sty,stx));
	//frontNeighbours(sty,stx,10);
	GhostNeighbours();
	while (!bfs.empty() )
	{

		Point p1;
		p1=bfs.front();

		int col=0,row=0;
		row=p1.getX();
		col=p1.getY();

		frontNeighbours(row,col);

		dis=Distance(p1,Point(13,17));
		if(dis<minDis)
		{
			minDis=dis;
			//			yy=12-(yy/60);
			//			xx=xx/60;
			minDisDirec=BfsArray[row][col];
		}
		//bfs.pop();
		bfs.pop();
		if(bfs.empty())
		{

			//
			while(!bfs.empty()) bfs.pop();

			return minDisDirec;


		}


	}

}
int Board::ScatterClyde()
{
	double dis=0;

	double minDis=5000;
	int minDisDirec=10;
	int stx=0,sty=0;
	clyde.getCoordinates(stx,sty);
	stx=stx/60;
	sty=12-(sty/60);
	//bfs.push(Point(sty,stx));
	//frontNeighbours(sty,stx,10);
	clydeNeighbours();
	while (!bfs.empty() )
	{

		Point p1;
		p1=bfs.front();

		int col=0,row=0;
		row=p1.getX();
		col=p1.getY();

		frontNeighbours(row,col);

		dis=Distance(p1,Point(13,17));
		if(dis<minDis)
		{
			minDis=dis;
			//			yy=12-(yy/60);
			//			xx=xx/60;
			minDisDirec=BfsArray[row][col];
		}
		//bfs.pop();
		bfs.pop();
		if(bfs.empty())
		{

			//
			while(!bfs.empty()) bfs.pop();

			return minDisDirec;


		}

	}
}
int Board::ScatterIlinky()
{
	InitializeBfs();
	double dis=0;

	double minDis=5000;
	int minDisDirec=10;
	int stx=0,sty=0;
	inky.getCoordinates(stx,sty);
	stx=stx/60;
	sty=12-(sty/60);
	//bfs.push(Point(sty,stx));
	//frontNeighbours(sty,stx,10);
	inkyNeighbours();
	while (!bfs.empty() )
	{

		Point p1;
		p1=bfs.front();

		int col=0,row=0;
		row=p1.getX();
		col=p1.getY();

		frontNeighbours(row,col);

		dis=Distance(p1,Point(1,1));
		if(dis<minDis)
		{
			minDis=dis;
			//			yy=12-(yy/60);
			//			xx=xx/60;
			minDisDirec=BfsArray[row][col];
		}
		//bfs.pop();
		bfs.pop();
		if(bfs.empty())
		{

			//
			while(!bfs.empty()) bfs.pop();

			return BfsArray[1][1];


		}


	}

}
int Board::ScatterPinky()
{
	double dis=0;

	double minDis=5000;
	int minDisDirec=10;
	int stx=0,sty=0;
	pinky.getCoordinates(stx,sty);
	stx=stx/60;
	sty=12-(sty/60);
	//bfs.push(Point(sty,stx));
	//frontNeighbours(sty,stx,10);
	PinkyNeighbours();
	while (!bfs.empty() )
	{

		Point p1;
		p1=bfs.front();

		int col=0,row=0;
		row=p1.getX();
		col=p1.getY();

		frontNeighbours(row,col);

		dis=Distance(p1,Point(13,17));
		if(dis<minDis)
		{
			minDis=dis;
			//			yy=12-(yy/60);
			//			xx=xx/60;
			minDisDirec=BfsArray[row][col];
		}
		//bfs.pop();
		bfs.pop();
		if(bfs.empty())
		{

			//
			while(!bfs.empty()) bfs.pop();

			return minDisDirec;


		}


	}

}



bool Board::ifNillRight(int x,int y)
{

	//x=x+38;
	x=x/60;

	y=12-(y/60);

	if(y>12 || x+1>16 || y<0 || x<0)
		return false;
	cout<<"if nill Right :x ; "<<x+1<<endl;
	cout<<"if nill Right :y ; "<<y<<endl;
	if(board_array[y][x+1]==NILL)
	{
		cout<<"right moved by nill \n";
		return true;
	}
	else
		return false;

}
bool Board::ifNullRight(int x,int y)
{

	//x=x+38;


	if(y>12 || x+1>17 || y<0 || x<0)
		return false;
	cout<<"if nill Right :x ; "<<x+1<<endl;
	cout<<"if nill Right :y ; "<<y<<endl;
	if(BfsArray[y][x+1]==NILL)
	{
		cout<<"right moved by null \n";
		return true;
	}
	else
		return false;

}

bool Board::ifNillUp(int x,int y)
{


	x=x/60;

	y=12-(y/60);
	if(y+1>13 || x>17 || y<0 || x<0)
		return false;
	cout<<"if nill up :x ; "<<x<<endl;
	cout<<"if nill up :y ; "<<y+1<<endl;
	if(board_array[y-1][x]==NILL)
	{
		cout<<"Up moved by nill \n";
		return true;
	}
	else
		return false;
}

bool Board::ifNullUp(int x,int y)
{



	if(y+1>13 || x>17 || y<0 || x<0)
		return false;
	cout<<"if nill up :x ; "<<x<<endl;
	cout<<"if nill up :y ; "<<y-1<<endl;
	if(BfsArray[y-1][x]==NILL)
	{
		cout<<"Up moved by null \n";
		return true;
	}
	else
		return false;
}

bool Board::ifNillDown(int x,int y)
{
	//y=y+38;
	x=x/60;



	y=12-(y/60);
	if(y-1>13 || x>17 || y<0 || x<0)
		return false;
	cout<<"if nill down :y ; "<<y+1<<endl;
	cout<<"if nill down :x ; "<<x<<endl;
	if(board_array[y+1][x]==NILL )
	{
		cout<<"down moved by nill \n";
		return true;
	}
	else
		return false;
}

bool Board::ifNullDown(int x,int y)
{
	//y=y+38;

	if(y-1>13 || x>17 || y<0 || x<0)
		return false;
	cout<<"if nill down :y ; "<<y+1<<endl;
	cout<<"if nill down :x ; "<<x<<endl;
	if(BfsArray[y+1][x]==NILL )
	{
		cout<<"down moved by null \n";
		return true;
	}
	else
		return false;
}
bool Board::ifNillLeft(int x,int y)
{


	//x=x-10;
	x=x/60;

	y=12-(y/60);
	if(y>13 || x-1>16 || y<0 || x<0)
		return false;
	cout<<"if nill left :x ; "<<x-1<<endl;
	cout<<"if nill left :y ; "<<y<<endl;


	if(board_array[y][x-1]==NILL)
	{
		cout<<"left moved by nill \n";
		return true;
	}
	else
		return false;

}
bool Board::ifNullLeft(int x,int y)
{



	if(y>13 || x-1>16 || y<0 || x<0)
		return false;
	cout<<"if nill left :x ; "<<x-1<<endl;
	cout<<"if nill left :y ; "<<y<<endl;


	if(BfsArray[y][x-1]==NILL)
	{
		cout<<"left moved by null \n";
		return true;
	}
	else
		return false;

}
bool Board::bombRadius(int x,int y,int &score)
{
	x=x/60;
	y=12-(y/60);
	//for green tiles
	if(board_array[y+1][x]==2)
	{
		board_array[y+1][x]=0;
		brickChek++;
		score+=50;
	}
	if(board_array[y-1][x]==2)
	{
		board_array[y-1][x]=0;
		score+=50;
		brickChek++;
	}
	if(board_array[y][x+1]==2)
	{
		board_array[y][x+1]=0;
		score+=50;
		brickChek++;
	}
	if(board_array[y][x-1]==2)
	{
		board_array[y][x-1]=0;
		score+=50;
		brickChek++;
	}
	if(board_array[y+1][x]==2)
	{
		board_array[y+1][x]=0;
		score+=50;
		brickChek++;
	}
	//red bricks
	if(board_array[y+1][x]==3)  //bomb below red
	{
		board_array[y+1][x]=0;
		score+=100;
		brickChek++;
		if(board_array[y+1+1][x]==2)
		{
			board_array[y+1+1][x]=0;
			score+=50;
			brickChek++;
		}
		if(board_array[y+1][x+1]==2)
		{
			board_array[y+1][x+1]=0;
			score+=50;
			brickChek++;
		}
		if(board_array[y+1][x-1]==2)
		{
			board_array[y+1][x-1]=0;
			score+=50;
			brickChek++;
		}
	}
	if(board_array[y-1][x]==3)
	{
		board_array[y-1][x]=0;
		score+=100;
		brickChek++;
		if(board_array[y-1-1][x]==2)
		{
			board_array[y-1-1][x]=0;
			score+=50;
			brickChek++;
		}
		if(board_array[y-1][x+1]==2)
		{
			board_array[y-1][x+1]=0;
			score+=50;
			brickChek++;
		}
		if(board_array[y-1][x-1]==2)
		{
			board_array[y-1][x-1]=0;
			score+=50;
			brickChek++;
		}

	}
	if(board_array[y][x-1]==3)
	{
		board_array[y][x-1]=0;
		score+=100;
		brickChek++;
		if(board_array[y-1][x-1]==2)
		{
			board_array[y-1][x-1]=0;
			score+=50;
			brickChek++;
		}
		if(board_array[y+1][x-1]==2)
		{
			board_array[y+1][x-1]=0;
			score+=50;
			brickChek++;
		}
		if(board_array[y][x-1-1]==2)
		{
			board_array[y][x-1-1]=0;
			score+=50;
			brickChek++;
		}

	}
	if(board_array[y][x+1]==3)
	{
		board_array[y][x+1]=0;
		score+=100;
		brickChek++;
		if(board_array[y-1][x+1]==2)
		{
			board_array[y-1][x+1]=0;
			score+=50;
			brickChek++;
		}
		if(board_array[y+1][x+1]==2)
		{
			board_array[y+1][x+1]=0;
			score+=50;
			brickChek++;
		}
		if(board_array[y][x+1+1]==2)
		{
			board_array[y][x+1+1]=0;
			score+=50;
			brickChek++;
		}

	}
	//ghost
	if((blinky.getX()==bomb.getBmbx()-25 &&blinky.getY()==bomb.getBmby()-25)  )
	{
		blinky.setX(425);
		blinky.setY(125);
		score+=200;
	}
	if((pinky.getX()==bomb.getBmbx()-25 &&pinky.getY()==bomb.getBmby()-25)  )
	{
		pinky.setX(665);
		pinky.setY(245);
		score+=200;
	}
	if((inky.getX()==bomb.getBmbx()-25 &&inky.getY()==bomb.getBmby()-25)   )
	{
		inky.setX(665);
		inky.setY(665);
		score+=200;
	}
	if((pacman.getX()==bomb.getBmbx() &&pacman.getY()==bomb.getBmby()	)  )
	{
		pacman.setX(90);
		pacman.setY(90);
		pacman.setLife(pacman.getLife()-1);
	}


	if((blinky.getX()==bomb.getBmbx()-25+60 &&blinky.getY()==bomb.getBmby()-25)  )
	{
		blinky.setX(425);
		blinky.setY(125);
		score+=200;
	}
	if((pinky.getX()==bomb.getBmbx()-25+60 &&pinky.getY()==bomb.getBmby()-25)  )
	{
		pinky.setX(665);
		pinky.setY(245);
		score+=200;
	}
	if((inky.getX()==bomb.getBmbx()-25+60 &&inky.getY()==bomb.getBmby()-25)  )
	{
		inky.setX(665);
		inky.setY(665);
		score+=200;
	}
	if((pacman.getX()==bomb.getBmbx()+60 &&pacman.getY()==bomb.getBmby()	)  )
	{
		pacman.setX(90);
		pacman.setY(90);
		pacman.setLife(pacman.getLife()-1);
	}






	if((blinky.getX()==bomb.getBmbx()-25-60 &&blinky.getY()==bomb.getBmby()-25)  )
	{
		blinky.setX(425);
		blinky.setY(125);
		score+=200;
	}
	if((pinky.getX()==bomb.getBmbx()-25-60 &&pinky.getY()==bomb.getBmby()-25)  )
	{
		pinky.setX(665);
		pinky.setY(245);
		score+=200;
	}
	if((inky.getX()==bomb.getBmbx()-25-60 &&inky.getY()==bomb.getBmby()-25)  )
	{
		inky.setX(665);
		inky.setY(665);
		score+=200;
	}
	if((pacman.getX()==bomb.getBmbx()-60 &&pacman.getY()==bomb.getBmby()	)  )
	{
		pacman.setX(90);
		pacman.setY(90);
		pacman.setLife(pacman.getLife()-1);
	}

	if((blinky.getX()==bomb.getBmbx()-25 &&blinky.getY()==bomb.getBmby()-25-60)  )
	{
		blinky.setX(425);
		blinky.setY(125);
		score+=200;
	}
	if((pinky.getX()==bomb.getBmbx()-25 &&pinky.getY()==bomb.getBmby()-25-60)  )
	{
		pinky.setX(665);
		pinky.setY(245);
		score+=200;
	}
	if((inky.getX()==bomb.getBmbx()-25 &&inky.getY()==bomb.getBmby()-25-60)  )
	{
		inky.setX(665);
		inky.setY(665);
		score+=200;
	}
	if((pacman.getX()==bomb.getBmbx() &&pacman.getY()==bomb.getBmby()+60	)  )
	{
		pacman.setX(90);
		pacman.setY(90);
		pacman.setLife(pacman.getLife()-1);
	}

	if(((blinky.getX()==bomb.getBmbx()-25 )&&(blinky.getY()==bomb.getBmby()-25+60) ) )
	{
		blinky.setX(425);
		blinky.setY(125);
		score+=200;
	}
	if(((pinky.getX()==bomb.getBmbx()-25 )&&(pinky.getY()==bomb.getBmby()-25+60) ) )
	{
		pinky.setX(665);
		pinky.setY(245);
		score+=200;
	}
	if(((inky.getX()==bomb.getBmbx()-25 )&&(inky.getY()==bomb.getBmby()-25+60) ) )
	{
		pinky.setX(665);
		pinky.setY(665);
		score+=200;
	}

	if((pacman.getX()==bomb.getBmbx() &&pacman.getY()==bomb.getBmby()-60	)  )
	{
		pacman.setX(90);
		pacman.setY(90);
		pacman.setLife(pacman.getLife()-1);
	}



	return true;
}



void Board::GetInitPinkyPosition(int &x, int &y) {
	x = xcellsize * 3;
	y = (BOARD_Y - 3) * ycellsize;
}
void Board::GetInitTextPosition(int &x, int &y) {
	x = xcellsize;
	y = (BOARD_Y - 1) * ycellsize + ycellsize / 2;
}
