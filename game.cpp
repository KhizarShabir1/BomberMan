//============================================================================
// Name        : .cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Brick Slayer...
//============================================================================

#ifndef Bomberman_CPP_
#define Bomberman_CPP_
#include "Board.h"
#include "util.h"
#include <iostream>
#include<string>
#include<cmath>                // for basic math functions such as cos, sin, sqrt
#include <queue>
using namespace std;
#include "character.h"
#include "ghost.h"

#include "pinky.h"
#include "blinky.h"
#include "inky.h"


#include "pacman.h"
#include "bomb.h"
#include "point.h"
//#include "time.h"
bool bombStatus = false;
Board *b;

enum GameState { // Use to check different states of game...
	Ready, Shot, Over, RemoveCluster
};

//enum Direction {  UP, DOWN, LEFT, RIGHT };

// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}
void DrawEnemy(int x/*starting x*/, int y/*starting y*/,
		ColorNames color/*color*/, float gw = 6/*Enemy Width in Units*/,
		float gh = 7/*Enemy Height in Units*/) {
	int ogw = 6, ogh = 7;

	glPushMatrix();
	float sx = (float) gw / ogw, sy = (float) gh / ogh;
	glTranslatef(x, y, 1);
	glScalef(sx, sy, 1);

	// Draw Enemy
	DrawRoundRect(0, 1, 6, 3, colors[color]);
	DrawCircle(3, 4, 3.01, colors[color]);
	glPushMatrix();
	glScalef(0.9, 1.1, 1);
	//  legs
	DrawCircle(0.75, 1, 0.75, colors[color]);
	DrawCircle(3.25, 1, 0.75, colors[color]);
	DrawCircle(5.85, 1, 0.75, colors[color]);

	glPopMatrix();
	//	glPopMatrix();

	// eyes

	glPushMatrix();
	glScalef(0.9, 1.1, 1);
	DrawCircle(1.85, 3.95, 0.75, colors[WHITE]);
	DrawCircle(4.95, 3.95, 0.75, colors[WHITE]);
	glPopMatrix();

	// eyes
	DrawCircle(1.65, 4.25, 0.45, colors[BLUE]);
	DrawCircle(4.45, 4.25, 0.45, colors[BLUE]);
	glPopMatrix();
}

//Number of Vertices used to draw Bomberman Circle...
// x= r cos (theta), y= r sin(theta)
const int npmvertices = 1220;
GLfloat pmvertices[npmvertices][2];
void InitPMVertices(float radius) {

	float hdegree = (M_PI - M_PI / 2.0) / 360.0;
	float angle = M_PI + M_PI / 6.0;
	for (int i = 0; i < npmvertices; ++i) {
		pmvertices[i][0] = radius * cos(angle);
		pmvertices[i][1] = radius * sin(angle);
		angle += hdegree;
	}
}


void Timer(int m);
//unsigned long long b->b->ftime=0;

void Lives(float sx/*center x*/, float sy/*center y*/,
		float radius/*Radius*/,
		const ColorNames &colorname/*Bomberman Colour*/) {

	glColor3fv(colors[colorname]); // set the circle color
	InitPMVertices(radius);
	glBegin (GL_TRIANGLE_FAN);
	glVertex4f(sx, sy, 0, 1);
	for (int i = 0; i < npmvertices; ++i)
		glVertex4f(sx + pmvertices[i][0], sy + pmvertices[i][1], 0, 1);
	glEnd();
	DrawCircle(sx - radius + radius / 2, sy + (radius - radius / 2),
			radius / 10, colors[BLACK]);
}
void DrawBomberman(float sx/*center x*/, float sy/*center y*/,
		float radius/*Radius*/,
		const ColorNames &colorname/*Bomberman Colour*/) {

	DrawCircle(sx , sy , radius, colors[YELLOW]);
	cout<<"sx "<<sx<<" : sy : "<<sy<<endl;
	if(b->face==1){
		DrawCircle(sx - radius + radius / 2, sy + (radius - radius / 2), radius / 10, colors[BLACK]);//left
		if(b->ftime%2==0)
			DrawTriangle(sx, sy,sx-radius, sy + (radius/2), sx-radius,sy -(radius - radius / 2),colors[BLACK] );//left
		//else
		//DrawTriangle(sx, sy,sx-radius, sy + (radius/4), sx-radius,sy -(radius - radius / 2),colors[YELLOW] );
	}
	if(b->face==2){
		DrawCircle(sx + radius - radius / 2, sy + (radius - radius / 2), radius / 10, colors[BLACK]);//RIGHT
		if(b->ftime%2==0 )
			DrawTriangle(sx, sy,sx+radius, sy + (radius/2), sx+radius,sy -(radius - radius / 2),colors[BLACK] );//Right
		//else
		//	DrawTriangle(sx, sy,sx+radius, sy + (radius/4), sx+radius,sy -(radius - radius / 2),colors[YELLOW] );

	}
	if (b->face==3){
		DrawCircle(sx - radius + radius / 2, sy - (radius - radius / 2), radius / 10, colors[BLACK]);//down
		if(b->ftime%2==0)
			DrawTriangle(sx, sy,sx-radius/4, sy - (radius), sx+radius/2+5,sy -(radius ),colors[BLACK] );//down
		//else
		//DrawTriangle(sx, sy,sx-radius/4, sy - (radius), sx+radius/2,sy -(radius ),colors[YELLOW] );

	}
	if(b->face==4){
		cout<<"SX"<<sx<<endl;
		DrawCircle(sx - radius + radius / 2, sy + (radius - radius / 2), radius / 10, colors[BLACK]);//up
		if(b->ftime%2==0)
			DrawTriangle(sx, sy,sx-radius/4, sy + (radius), sx+radius/2+5,sy +(radius ),colors[BLACK] );//up
		//else
		//DrawTriangle(sx, sy,sx-radius/4, sy + (radius), sx+radius/2,sy +(radius ),colors[YELLOW] );
	}


}


void Explode(int & score)
{
	int x=0,y=0;
	x=b->bomb.getBmbx();
	y=b->bomb.getBmby();
	b->bombRadius(x,y,score);

}
/*
 * Main Canvas drawing function.
 * */















void DrawString(float x, float y, const string& score, float * color);
void GameDisplay()/**/{
if(b->over==false)
{
	//Timer( 5);
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0/*Red Component*/, 0,	//148.0/255/*Green Component*/,
			0/*Blue Component*/, 1 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors

	//


	b->Draw();
	if(b->point==0){
		b->Display(b->arraypoint);
		b->point++;
	}
	b->dispPoints(b->arraypoint);
	int xx=0,yy=0;
	b->pacman.getCoordinates(xx,yy);
	if(b->removePoints(b->arraypoint,xx,yy))

	{
		b->score+=1;
	}


	cout<<b->BOARD_X<<" "<<b->BOARD_Y<<endl;
	int x, y;
	b->GetInitPinkyPosition(x, y);
	cout<<"Blinky position \n";
	cout <<"x= " <<x+5+4*60 << "  y= " <<y+5-9*60 << endl << flush;

	DrawEnemy(b->blinky.getX(), b->blinky.getY(), PURPLE, 0.8 * b->GetCellSize(), 0.8 * b->GetCellSize());
	DrawEnemy(b->pinky.getX(), b->pinky.getY(), LIGHT_PINK, 0.8 * b->GetCellSize(), 0.8 * b->GetCellSize());
	if(b->brickChek>18)
	DrawEnemy(b->clyde.getX(),b->clyde.getY(), GRAY, 0.8 * b->GetCellSize(), 0.8 * b->GetCellSize());
	if(b->brickChek>7)
	DrawEnemy(b->inky.getX(), b->inky.getY(), RED, 0.8 * b->GetCellSize(), 0.8 * b->GetCellSize());


	if(b->blueChk==true)
	{
		DrawEnemy(b->blinky.getX(), b->blinky.getY(), BLUE, 0.8 * b->GetCellSize(), 0.8 * b->GetCellSize());
		DrawEnemy(b->pinky.getX(), b->pinky.getY(), BLUE, 0.8 * b->GetCellSize(), 0.8 * b->GetCellSize());
		if(b->brickChek>7)
		DrawEnemy(b->inky.getX(), b->inky.getY(), BLUE, 0.8 * b->GetCellSize(), 0.8 * b->GetCellSize());
		if(b->brickChek>18)
		DrawEnemy(b->clyde.getX(), b->clyde.getY(), BLUE, 0.8 * b->GetCellSize(), 0.8 * b->GetCellSize());

		if(b->bluetime > 8)
		{
			b->blueChk=false;

			b->bluetime=0;
			b->turn=0;
		}
	}
	if(b->blueChk==true)
	{
		if(((b->blinky.getX()==b->pacman.getX()-25)&&(b->blinky.getY()==b->pacman.getY()-25) ) )
		{
			b->blinky.setX(425);
			b->blinky.setY(125);
			b->score+=200;
		}
		if(((b->pinky.getX()==b->pacman.getX()-25)&&(b->pinky.getY()==b->pacman.getY()-25) ) )
		{
			b->pinky.setX(665);
			b->pinky.setY(245);
			b->score+=200;
		}
		if(((b->inky.getX()==b->pacman.getX()-25)&&(b->inky.getY()==b->pacman.getY()-25) ) && b->brickChek>7)
		{
			b->inky.setX(665);
			b->inky.setY(245);
			b->score+=200;
		}
		if(((b->clyde.getX()==b->pacman.getX()-25)&&(b->clyde.getY()==b->pacman.getY()-25) ) && b->brickChek>18 )
		{
			b->clyde.setX(485);
			b->clyde.setY(665);
			b->score+=200;
		}
	}
	b->GetInitBombermanPosition(x, y);
	cout<<"Bomberman position\n";

	DrawBomberman(b->pacman.getX(), b->pacman.getY(), b->GetCellSize() / 2 - 2, YELLOW);

	cout<<"pacman radius "<< b->GetCellSize() / 2 - 2<<endl;
	cout<<"pcman X is : "<<b->pacman.getX()/60<<"\n pacman y is  "<<12-b->pacman.getY()/60<<endl;
	b->GetInitTextPosition(x, y);
	cout << endl <<"Text Position = "<< x << "  y= " << y << endl << flush;
	DrawString(x, y-15, "Score = "+ Num2Str(b->score), colors[WHITE]);
	DrawString(x+620, y-15, "Lives :", colors[WHITE]);
	//Lives of pacman
	for(int i=0,j=65;i<b->pacman.getLife();i++,j+=65)
	{
		Lives(x+990-j, y, b->GetCellSize() / 2 - 2, YELLOW);
	}


	if((b->blinky.getX()==b->pacman.getX()-25 && b->blinky.getY()==b->pacman.getY()-25)&& b->blueChk==false)
	{
		b->pacman.setLife(b->pacman.getLife()-1);
		b->pacman.setX(90);
		b->pacman.setY(90);
	}
	if((b->pinky.getX()==b->pacman.getX()-25 && b->pinky.getY()==b->pacman.getY()-25)&& b->blueChk==false)
	{
		b->pacman.setLife(b->pacman.getLife()-1);
		b->pacman.setX(90);
		b->pacman.setY(90);
	}
	if((b->inky.getX()==b->pacman.getX()-25 && b->inky.getY()==b->pacman.getY()-25)&& b->blueChk==false && b->brickChek>7)
	{
		b->pacman.setLife(b->pacman.getLife()-1);
		b->pacman.setX(90);
		b->pacman.setY(90);
	}
	if((b->clyde.getX()==b->pacman.getX()-25 && b->clyde.getY()==b->pacman.getY()-25)&& b->blueChk==false && b->brickChek>18)
	{
		b->pacman.setLife(b->pacman.getLife()-1);
		b->pacman.setX(90);
		b->pacman.setY(90);
	}
	if(b->pacman.getLife()<=0)
	{

	b->over=true;
	//DrawString(x+620, y, "GAME OVER  :", colors[WHITE]);
	
		
	}

	b->DisplayBFS();
	b->InitializeBfs();


	//glPopMatrix();
	if (bombStatus == true ) {

		DrawCircle(b->bomb.getBmbx(), b->bomb.getBmby(), 20, colors[WHITE]);
		DrawCircle(b->bomb.getBmbx(), b->bomb.getBmby(), 15, colors[RED]);
		DrawCircle(b->bomb.getBmbx(), b->bomb.getBmby(), 10, colors[b->bmbclr]);
		//bomb++;// To Draw a bomb. You will provide your own x and y relative to Bombermans position

		//DrawLine( bmbx,bmby,bmbx+20,bmby+20, 3, colors[ORANGE]);
		if(b->bomb.getBmbTime()==3)
		{

			bombStatus=false;

			Explode(b->score);
			b->bomb.setBmbTime(0);
			DrawCircle(b->bomb.getBmbx(), b->bomb.getBmby(), 30, colors[RED]);

			DrawLine( b->bomb.getBmbx()-75,b->bomb.getBmby(),b->bomb.getBmbx()+75,b->bomb.getBmby(), 10, colors[RED]);
			DrawLine( b->bomb.getBmbx(),b->bomb.getBmby()+75,b->bomb.getBmbx(),b->bomb.getBmby()-75, 10, colors[RED]);
			DrawCircle(b->bomb.getBmbx(), b->bomb.getBmby(), 20, colors[WHITE]);
			DrawCircle(b->bomb.getBmbx(), b->bomb.getBmby(), 15, colors[BLACK]);
			//DrawString(b->bomb.getBmbx(),  b->bomb.getBmby(), "!! POW !!",colors[BLUE]);

			//DrawString(x+400, y, "!! POW !!", colors[5]);
			b->bmb=0;
		}
}
	if(b->over==true)
			{


			DrawString(x+620, y, "GAME OVER  :", colors[WHITE]);


			}

	}


	glutSwapBuffers(); // do not modify this line..


}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {


	/*if (key == GLUT_DOWN ) {
		cout << "Left Mouse Button Pressed at Coordinates X=" << x << " Y= "
				<< height - y << endl;*/
	if (key
			== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		b->face=1;
		// what to do when left key is pressed...
		b->pacman.getCoordinates(x,y);
		if(b->ifNillLeft(x,y)){

			x=x-60;
			//y=y+5;
			b->pacman.setX(x);

		}


	} else if (key
			== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
		b->face=2;

		int x=0, y=0;
		b->pacman.getCoordinates(x,y);
		//b->GetInitPinkyPosition(x, y);
		if(b->ifNillRight(x,y)){

			x=x+60;
			//y=y+5;
			b->pacman.setX(x);

		}
		//b->GetInitBombermanPosition(x,y);
		//int check=2;
		//DrawBomberman(x, y, b->GetCellSize() / 2 - 2, YELLOW);
		//glutPostRedisplay();
	} else if (key
			== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
		b->face=4;
		b->pacman.getCoordinates(x,y);
		if(b->ifNillUp(x,y)){


			y=y+60;
			//y=y+5;
			b->pacman.setY(y);

		}
	}

	else if (key
			== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
		b->face=3;
		b->pacman.getCoordinates(x,y);
		if(b->ifNillDown(x,y)){

			y=y-60;

			b->pacman.setY(y);

		}

	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}

	if ((key == 'b' || key == 'B')&&	bombStatus != true) //Key for placing the bomb
	{
		bombStatus = true;

		b->bomb.setBmbx(b->pacman.getX());
		b->bomb.setBmby(b->pacman.getY());
		int x=0;
		int y=0;
		x=b->pacman.getX()/60;
		y=12-b->pacman.getY()/60;
		//b->BfsArray[y][x]=5;






	}
	glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */

void Timer(int m) {
	b->dt = (m - b->lastframe) / 1000.0;

	glutPostRedisplay();
	b->lastframe = m;
	b->ftime++;
	b->bmbclr++;
	if(b->bmbclr>120)
	{
		b->bmbclr=0;
	}
	Point temp;
	if(bombStatus==true){

		b->bomb.setBmbTime(b->bmb++);
	}
	if(b->blueChk==true)
	{
		b->bluetime++;
	}
	b->changeModetime++;
	if(b->changeModetime>=24)
	{
		b->changeMode=false;
		b->changeModetime=0;
	}
	if(b->changeModetime<17)
	{
		b->changeMode=false;
	}
	else
	{
		b->changeMode=true;
	}


	b->moveBlinky(b->blueChk,b->changeMode);
	b->movePinky(b->blueChk,b->changeMode);
	if(b->brickChek>7)
	b->moveIinky(b->blueChk,b->changeMode);
	if(b->brickChek>18)
	b->moveClyde(b->blueChk,b->changeMode);

	//	if( b->chk180==0){
	//		b->p2=b->blinky.getPosition();
	//	}
	//
	//	if(b->chk180==2)
	//	{
	//
	//		moveBlinky(b->p2);
	//
	//	}
	//
	//	if(b->chk180==1 && b->chk180==0){

	//moveBlinky(temp);
	//		 b->chk180++;
	//}//

	// implement your functionality here
	// once again we tell the library to call our Timer function after next 1000/FPS

	glutPostRedisplay();
	glutTimerFunc(700.0, Timer, 0);
}

/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {

	b = new Board(60, 60); // create a new board object to use in the Display Function ...

	int width = 1020, height = 840; // i have set my window size to be 800 x 600
	b->InitalizeBoard(width, height);
	b->InitializeBfs();
	b->DisplayBFS();
	//m.allocateM(p);

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("CP's Bomber Bomberman"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
#ifdef WITH_TEXTURES
	RegisterTextures();
#endif
	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* Bomberman_CPP_ */
