/*
	HW3 3D Objects

	Draws some 3D objects, notibly, trees
	Base code copied and modified from ex8.

	Key bindings:
	WASD move through space
	Arrow keys change view
	0 Resets view
	ESC Exit

*/
// a bunch of includes
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glut.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif
#include <time.h>

// Cosine and Sine in degrees
// copying this from ex8 bc it's pretty useful
#define Cos(x) (cos((x)*3.14159265/180))
#define Sin(x) (sin((x)*3.14159265/180))

// All the convenience functions from ex8
// I define them here so I can put them at the bottom
// and forget about them
void Print(const char* format, ...);
void ErrCheck(const char* where);
void Fatal(const char* format, ...);

// not sure I'll use this but called when nothing else to do
void idle();
// copied from ex8 called when window resized
void reshape();
// called when a key is pressed
void key(unsigned char ch, int x, int y);
// called when special key pressed like arrows
void special(int key, int x, int y);
// GLUT calls to display scene
void display();
// grass for cool effect/texture?
void grass();
// I forgot the openGL method that does this
void Color(double r, double g, double b);
// reproject
void Project();
// I want a nice picket fence, no reason why really
void fence();
void fence_line();
void fence_post();
void picket();

// important variablessszzzz
int th = 0; // um, view angle stuff
int ph = 0; // the other view angle rotation
double dim = 10.0; // size of world / distance of camera?
double asp = 1; // aspect ratio
double ground = -3; // I want the scene to start more towards
// the bottom of the screen so move everything down some

// displays the scene
void display() {
	// clear the window and the depth buffer
	// I want a cool background color so here
	glClearColor(0.06, .92, .89, .5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// enable z buffering
	glEnable(GL_DEPTH_TEST);
	// undo previous nonsense
	glLoadIdentity();
	// rotate the world
	glRotatef(ph, 1, 0, 0);
	glRotatef(th, 0, 1, 0);

	// draw, the grouuunndddd
	// also, if you change the starting dim
	// remember to also change these points
	Color(15, 86, 7);
	glBegin(GL_QUADS);
	glVertex3f(-10, ground, -10);
	glVertex3f(-10, ground, 10);
	glVertex3f(10, ground, 10);
	glVertex3f(10, ground, -10);
	glEnd();

	// I want a fence, for, idk, protect my property?
	// from the nothingness?
	fence();
	picket();

	// draw the grass
	grass();
	ErrCheck("display");
	glFlush();
	glutSwapBuffers();
}

// draw a nice fence
void fence() {
	glPushMatrix();
	glEnable(GL_CULL_FACE);

	glTranslatef(10, ground, 9);
	fence_line();
	glTranslatef(18, 0, -18);
	fence_line();
	glTranslatef(-1, 0, -1);
	glRotatef(90, 0, 1, 0);
	fence_line();
	glTranslatef(18, 0, 18);
	fence_line();

	glDisable(GL_CULL_FACE);
	glPopMatrix();
}

void fence_line() {
	int i;
	for(i = 0; i < 9; i++) {
		glTranslatef(-2, 0, 0);
		fence_post();
	}
}

void fence_post() {
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);

	glVertex3f(-.5, 0, .25);
	glVertex3f(-.5, 0, -.25);
	glVertex3f(.5, 0, -.25);
	glVertex3f(.5, 0, .25);

	glVertex3f(-.5, 0, .25);
	glVertex3f(.5, 0, .25);
	glVertex3f(.5, 3, .25);
	glVertex3f(-.5, 3, .25);

	glVertex3f(.5, 0, .25);
	glVertex3f(.5, 0, -.25);
	glVertex3f(.5, 3, -.25);
	glVertex3f(.5, 3, .25);

	glVertex3f(.5, 0, -.25);
	glVertex3f(-.5, 0, -.25);
	glVertex3f(-.5, 3, -.25);
	glVertex3f(.5, 3, -.25);

	glVertex3f(-.5, 0, -.25);
	glVertex3f(-.5, 0, .25);
	glVertex3f(-.5, 3, .25);
	glVertex3f(-.5, 3, -.25);

	glVertex3f(-.5, 3, -.25);
	glVertex3f(-.5, 3, .25);
	glVertex3f(0, 4, .25);
	glVertex3f(0, 4, -.25);

	glVertex3f(0, 4, -.25);
	glVertex3f(0, 4, .25);
	glVertex3f(.5, 3, .25);
	glVertex3f(.5, 3, -.25);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(-.5, 3, .25);
	glVertex3f(.5, 3, .25);
	glVertex3f(0, 4, .25);

	glVertex3f(0, 4, -.25);
	glVertex3f(.5, 3, -.25);
	glVertex3f(-.5, 3, -.25);

	glEnd();

	glColor3f(0, 0, 0);
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);

	glVertex3f(.5, 0, .25);
	glVertex3f(.5, 3, .25);
	glVertex3f(0, 4, .25);
	glVertex3f(-.5, 3, .25);
	glVertex3f(-.5, 0, .25);

	glEnd();
	glBegin(GL_LINE_STRIP);

	glVertex3f(.5, 0, -.25);
	glVertex3f(.5, 3, -.25);
	glVertex3f(0, 4, -.25);
	glVertex3f(-.5, 3, -.25);
	glVertex3f(-.5, 0, -.25);

	glEnd();
	glBegin(GL_LINE_STRIP);

	glVertex3f(-.5, 0, .25);
	glVertex3f(.5, 0, .25);
	glVertex3f(.5, 0, -.25);
	glVertex3f(-.5, 0, -.25);

	glEnd();
	glBegin(GL_LINES);

	glVertex3f(.5, 3, .25);
	glVertex3f(.5, 3, -.25);

	glVertex3f(-.5, 3, .25);
	glVertex3f(-.5, 3, -.25);

	glVertex3f(0, 4, .25);
	glVertex3f(0, 4, -.25);

	glEnd();
}

// so, glut draws everytime and grass
// is random, but I don't want it to be random
// so first make all the grass and save it
// so the grass is consitent with the scene
struct grass_pos {
	double x;
	double y;
	int rotation;
};
int grasscount = 500;
// won't let you modify at scope so
// remember to make this same as grasscount
struct grass_pos grasses[500];
void save_grass() {
	int i;
	for(i = 0; i < grasscount; i++) {
		struct grass_pos pos;
		pos.x = (rand()%((int)dim*20)-dim/.1)/10.0;
		pos.y = (rand()%((int)dim*20)-dim/.1)/10.0;
		pos.rotation = rand()%360;

		grasses[i] = pos;
	}
}


// draw some GRAAAASSSSSSS
void grass() {
	int i;
	// idk why but I don't think this is working...
	// it's enabled everywhere it should be?
	// IG the grass is too close together so bits
	// aren't accurate enough
	// oh hehe, it's because arrows were going opposite
	// direction, so z was opposite direction I thought it
	// was, oops lol. that makes a lot of sense
	glEnable(GL_DEPTH_TEST);
	for(i = 0; i < grasscount; i++) {
		// first, the blade, then, the blade
		glPushMatrix();
		// draw all the grasses
		glTranslatef(grasses[i].x, ground, grasses[i].y);
		glRotatef(grasses[i].rotation, 0, 1, 0);
		// funny effect, wigglin grass :)
		glRotatef(rand()%30/10.0, 0, 0, 1);
		glRotatef(rand()%30/10.0, 1, 0, 0);
		// now just draw some grass
		Color(85, 234, 16);
		glBegin(GL_QUADS);
		glVertex3f(-.1, 0, 0);
		glVertex3f(.1, 0, 0);
		glVertex3f(.1, .5, 0);
		glVertex3f(-.1, .5, 0);
		glEnd();
		glBegin(GL_TRIANGLES);
		glVertex3f(.1, .5, 0);
		glVertex3f(0, 1, .1);
		glVertex3f(-.1, .5, 0);
		glEnd();
		glColor3f(0, 0, 0);
		glLineWidth(3);
		glBegin(GL_LINE_STRIP);
		glVertex3f(-.1, 0, 0);
		glVertex3f(-.1, .5, 0);
		glVertex3f(0, 1, .1);
		glVertex3f(.1, .5, 0);
		glVertex3f(.1, 0, 0);
		glEnd();

		glPopMatrix();
	}
}


// called when special key pressed
void special(int key, int x, int y) {
	//  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_RIGHT)
		th += 5;
	//  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_LEFT)
		th -= 5;
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
		ph += 5;
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 5;
	//  PageUp key - increase dim
	else if (key == GLUT_KEY_PAGE_UP) 
		dim += 0.1;
	//  PageDown key - decrease dim
	else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
		dim -= 0.1;
	th %= 360;
	ph %= 360;
	Project();
	glutPostRedisplay();
}

void Project() {
	//  Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();
	// orthogonal project
	glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}

// called when a key is pressed
void key(unsigned char ch, int x, int y) {
	// exit on esc
	if(ch==27)
		exit(0);
	glutPostRedisplay();
}

// GLUT calls when nothing else to do
void idle() {

	glutPostRedisplay();
}

// when window is resized do this stuff
void reshape(int width, int height) {
	//  Set the viewport to the entire window
	glViewport(0,0, RES*width,RES*height);
	
	//  Orthogonal projection
	asp = (height>0) ? (double)width/height : 1;
	
	Project();
}

// um, outer rim thingy...idk what these are called
// wow, this is getting really long
// I'mma move it to the bottom
void picket() {
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glTranslatef(0, ground, 0);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);

	// front of the fence

	glVertex3f(9.5, 1, -9.5);
	glVertex3f(9.5, 1.5, -9.5);
	glVertex3f(9.5, 1.5, 9.5);
	glVertex3f(9.5, 1, 9.5);

	glVertex3f(9.5, 2, -9.5);
	glVertex3f(9.5, 2.5, -9.5);
	glVertex3f(9.5, 2.5, 9.5);
	glVertex3f(9.5, 2, 9.5);
	
	glVertex3f(-9.5, 2, -9.5);
	glVertex3f(-9.5, 2.5, -9.5);
	glVertex3f(9.5, 2.5, -9.5);
	glVertex3f(9.5, 2, -9.5);

	glVertex3f(-9.5, 1, -9.5);
	glVertex3f(-9.5, 1.5, -9.5);
	glVertex3f(9.5, 1.5, -9.5);
	glVertex3f(9.5, 1, -9.5);

	glVertex3f(9.5, 2, 9.5);
	glVertex3f(9.5, 2.5, 9.5);
	glVertex3f(-9.5, 2.5, 9.5);
	glVertex3f(-9.5, 2, 9.5);

	glVertex3f(9.5, 1, 9.5);
	glVertex3f(9.5, 1.5, 9.5);
	glVertex3f(-9.5, 1.5, 9.5);
	glVertex3f(-9.5, 1, 9.5);

	glVertex3f(-9.5, 2, 9.5);
	glVertex3f(-9.5, 2.5, 9.5);
	glVertex3f(-9.5, 2.5, -9.5);
	glVertex3f(-9.5, 2, -9.5);

	glVertex3f(-9.5, 1, 9.5);
	glVertex3f(-9.5, 1.5, 9.5);
	glVertex3f(-9.5, 1.5, -9.5);
	glVertex3f(-9.5, 1, -9.5);

	// back of the fence

	glVertex3f(-9, 1, -9);
	glVertex3f(-9, 1.5, -9);
	glVertex3f(-9, 1.5, 9);
	glVertex3f(-9, 1, 9);

	glVertex3f(-9, 2, -9);
	glVertex3f(-9, 2.5, -9);
	glVertex3f(-9, 2.5, 9);
	glVertex3f(-9, 2, 9);

	glVertex3f(9, 1, 9);
	glVertex3f(9, 1.5, 9);
	glVertex3f(9, 1.5, -9);
	glVertex3f(9, 1, -9);
	
	glVertex3f(9, 2, 9);
	glVertex3f(9, 2.5, 9);
	glVertex3f(9, 2.5, -9);
	glVertex3f(9, 2, -9);
	
	glVertex3f(9, 2, -9);
	glVertex3f(9, 2.5, -9);
	glVertex3f(-9, 2.5, -9);
	glVertex3f(-9, 2, -9);
	
	glVertex3f(9, 1, -9);
	glVertex3f(9, 1.5, -9);
	glVertex3f(-9, 1.5, -9);
	glVertex3f(-9, 1, -9);
	
	glVertex3f(-9, 2, 9);
	glVertex3f(-9, 2.5, 9);
	glVertex3f(9, 2.5, 9);
	glVertex3f(9, 2, 9);
	
	glVertex3f(-9, 1, 9);
	glVertex3f(-9, 1.5, 9);
	glVertex3f(9, 1.5, 9);
	glVertex3f(9, 1, 9);

	// top of the fence

	

	glVertex3f(9, 2.5, 9);
	glVertex3f(9.5, 2.5, 9.5);
	glVertex3f(9.5, 2.5, -9.5);
	glVertex3f(9, 2.5, -9);

	glVertex3f(9, 1.5, 9);
	glVertex3f(9.5, 1.5, 9.5);
	glVertex3f(9.5, 1.5, -9.5);
	glVertex3f(9, 1.5, -9);

	// bottom of the fence



	glEnd();
	glPopMatrix();
}

// start up GLUT and tell it what to do
// copied from ex8
int main(int argc, char* argv[]) {
	// I'm gonna use random a lot in this so
	// initialize
	time_t t;
	srand((unsigned) time(&t));
	// initialize grass
	save_grass();
	// initialize GLUT
	glutInit(&argc, argv);
	// window size
	// double buffer with z buffering
	// and I'm gonna go ahead and do face culling also
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// name it Kelley Kelley HW3
	glutCreateWindow("Kelley Kelley HW3");
	#ifdef USEGLEW
	//  Initialize GLEW
	if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
	#endif
	// I don't think I'm going to use idle but might as well
	// have it available
	glutIdleFunc(idle);
	// GLUT this is our display function
	glutDisplayFunc(display);
	// reshape function
	glutReshapeFunc(reshape);
	// special key function
	glutSpecialFunc(special);
	// keys
	glutKeyboardFunc(key);
	// start GLUT baby
	glutMainLoop();
	return 0;
}

// I forgot the openGL method and this is easier
// than finding it
void Color(double r, double g, double b) {
	glColor3f(r/255, g/255, b/255);
}

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...) {
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Check for OpenGL errors
 */
void ErrCheck(const char* where) {
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 *  Print message to stderr and exit
 */
void Fatal(const char* format , ...) {
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}