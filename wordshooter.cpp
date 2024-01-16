//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

string * dictionary;
int dictionarysize = 370099;
#define KEY_ESC 27 // A
void store_on_click(const int num_click);
// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int *board_down;
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
int flow=126;



int current_pos_x=470;
int current_pos_y=11;



enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

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
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

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
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
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
int GetAlphabet() {
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
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
/*
* Main Canvas drawing function.
* */
void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//write your drawing commands here or call your drawing functions...
	
	/*// UPPER ROW OF GAME
	
	DrawAlphabet((alphabets)24, 9,560, awidth, aheight);
	DrawAlphabet((alphabets)14, 69, 560, awidth, aheight);
	DrawAlphabet((alphabets)8, 129, 560, awidth, aheight);
	DrawAlphabet((alphabets)7, 189,560, awidth, aheight);
	DrawAlphabet((alphabets)11, 249, 560, awidth, aheight);
	DrawAlphabet((alphabets)23, 309, 560, awidth, aheight);
	DrawAlphabet((alphabets)12, 369, 560, awidth, aheight);
	DrawAlphabet((alphabets)0, 429, 560, awidth, aheight);
	DrawAlphabet((alphabets)13, 489,560, awidth, aheight);
	DrawAlphabet((alphabets)14, 549, 560, awidth, aheight);
	DrawAlphabet((alphabets)12, 609, 560, awidth, aheight);
	DrawAlphabet((alphabets)21, 669,560, awidth, aheight);
	DrawAlphabet((alphabets)23, 729, 560, awidth, aheight);
	DrawAlphabet((alphabets)17, 789, 560, awidth, aheight);
	DrawAlphabet((alphabets)13, 849,560, awidth, aheight);
	
	// LOWER ROW OF GAME
	
	DrawAlphabet((alphabets)16, 9,499, awidth, aheight);
	DrawAlphabet((alphabets)9, 69,499, awidth, aheight);
	DrawAlphabet((alphabets)10, 129,499, awidth, aheight);
	DrawAlphabet((alphabets)0, 189,499, awidth, aheight);
	DrawAlphabet((alphabets)17, 249,499, awidth, aheight);
	DrawAlphabet((alphabets)11, 309,499, awidth, aheight);
	DrawAlphabet((alphabets)18, 369,499, awidth, aheight);
	DrawAlphabet((alphabets)24, 429,499, awidth, aheight);
	DrawAlphabet((alphabets)18, 489,499, awidth, aheight);
	DrawAlphabet((alphabets)9, 549,499, awidth, aheight);
	DrawAlphabet((alphabets)20, 609,499, awidth, aheight);
	DrawAlphabet((alphabets)18, 669,499, awidth, aheight);
	DrawAlphabet((alphabets)16, 729,499, awidth, aheight);
	DrawAlphabet((alphabets)12, 789,499, awidth, aheight);
	DrawAlphabet((alphabets)19, 849,499, awidth, aheight);

	*/
	
	
	
	
	if(flow>=121) 	// time is set late to diaplay name of creator
	{
	
	// Displaying the name of creator of word shooter	
		
	DrawAlphabet((alphabets)22, 129,379, awidth, aheight); // WORD
	DrawAlphabet((alphabets)14, 189,379, awidth, aheight);
	DrawAlphabet((alphabets)17, 249,379, awidth, aheight);
	DrawAlphabet((alphabets)3, 309,379, awidth, aheight);
	
	DrawAlphabet((alphabets)18, 399,379, awidth, aheight); // SHOOTER
	DrawAlphabet((alphabets)7, 459,379, awidth, aheight);
	DrawAlphabet((alphabets)14, 519,379, awidth, aheight);
	DrawAlphabet((alphabets)14, 579,379, awidth, aheight);
	DrawAlphabet((alphabets)19, 639,379, awidth, aheight);
	DrawAlphabet((alphabets)4, 699,379, awidth, aheight);
	DrawAlphabet((alphabets)17, 759,379, awidth, aheight);
	
	DrawAlphabet((alphabets)12, 339,300, 35, 35); // MADE
	DrawAlphabet((alphabets)0, 374,300, 35, 35);
	DrawAlphabet((alphabets)3, 409,300, 35, 35);
	DrawAlphabet((alphabets)4, 444,300, 35, 35);
	
	DrawAlphabet((alphabets)1, 490,300, 35, 35); // BY
	DrawAlphabet((alphabets)24, 525,300, 35, 35);
	
	DrawAlphabet((alphabets)12, 279,200, 50, 50); // MUJTABA
	DrawAlphabet((alphabets)20, 329,200, 50, 50);
	DrawAlphabet((alphabets)9, 379,200, 50, 50);
	DrawAlphabet((alphabets)19, 429,200, 50, 50);
	DrawAlphabet((alphabets)0, 479,200, 50, 50);
	DrawAlphabet((alphabets)1, 529,200, 50, 50);
	DrawAlphabet((alphabets)0, 579,200, 50, 50);
	
	}
	
	if(flow<=120 && flow >=1) // Run Game for 120 seconde(2 min)
	{
	
	for(int e=0;e<15;e++) // Printing the two rows using a 2D ARRAY
	{
		for(int r=0;r<2;r++)
		{
			DrawAlphabet((alphabets)board[e][r],(e*60),(560-(60*r)), awidth, aheight);
		}
	}
	
	for(int w=499;w>=0;w--)
	{
			int temp = rand() % 26;
			int swap_0=temp+1;
			
			DrawAlphabet((alphabets)board_down[temp],470,11, 60, 60); // Displaying the next alphabet
			DrawAlphabet((alphabets)board_down[w],current_pos_x,current_pos_y, 60, 60); // Displaying the play alphabet
			DrawString(260,80, width, height + 5, "Next Ball", colors[ROSY_BROWN]); // Displaying the next ball line
			DrawAlphabet((alphabets)board_down[temp],270,11, 60, 60); // Displaying the next alphabet
	}
	
	
	DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
	DrawString(700, height - 25, width, height,
		"Time Left:" + Num2Str(flow) + " secs", colors[RED]);
	}
	
	if(flow<=0) // Displaying the GAME OVER
	{
		DrawAlphabet((alphabets)6, 249,360, awidth, aheight);
		DrawAlphabet((alphabets)0, 309, 360, awidth, aheight);

		DrawAlphabet((alphabets)12, 369, 360, awidth, aheight);
		DrawAlphabet((alphabets)4, 429,360, awidth, aheight);
		DrawAlphabet((alphabets)14, 489, 360, awidth, aheight);	
		DrawAlphabet((alphabets)21, 549, 360, awidth, aheight);

		DrawAlphabet((alphabets)4, 609, 360, awidth, aheight);
		DrawAlphabet((alphabets)17, 669, 360, awidth, aheight);
		
		DrawString(419,200, width, height + 5, "Your Score : " + Num2Str(score), colors[BLUE]);
	
	}	

	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}	

/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
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
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) 
{
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls

}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {

	
	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		current_pos_x=(x-30); 	// Assigning the coordinates of 1st and 2nd row
		current_pos_y=(y+290);
		
		//store_on_click(current_number);	
			
		if (state == GLUT_UP)
		{
			
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
		current_pos_x=(x-30);
		current_pos_y=(y+410);
		
	}
	//glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x=0, int y=0) 
{
	if (key == KEY_ESC/* Escape key ASCII*/) 
	{
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {

	flow--;	// It will reduce the time
	glutPostRedisplay();
	glutTimerFunc(10000.0/FPS, Timer, 0);
}

/*
* our gateway main function
* */
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
		cout<< " word "<< i << " =" << dictionary[i] <<endl;

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets
	
	
	board=new int*[30]; // Creating the 2D array

	for(int w=0;w<15;w++)
	board[w]=new int[15];	
	
		
	for(int e=0;e<15;e++) // Initialising it with random alphabets
	{
		for(int r=0;r<2;r++)
		{
			board[e][r]=GetAlphabet();
		}
	}
	
	
	
	board_down=new int[500];	// Creating the 1D array
	
	for(int g=0;g<500;g++)	// Initialising it with random alphabets
	{
		board_down[g]=GetAlphabet();
	}
	


	
	

	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}

#endif /* */
