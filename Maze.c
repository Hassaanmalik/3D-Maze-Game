//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h> //added math libary to code to allow for use of sin,cos and sqrt

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "LoadObj.h"

//Object
LoadObj objLoadObj("ghost");

//camera positions
float camPos[] = {0, 0, 0};
float camPos2[] = {10,5,1};


//setting the window height and width
const float w = 800;
const float h = 800;
const float wMid = w/2;
const float hMid = h/2;

//maze variables
int size;
int spacing = 10;
bool north[22][22];
bool south[22][22];
bool east[22][22];
bool west[22][22];
bool visited[22][22];

//bool on off's
bool calcMode = true;

//creating window
int window1;
int window2;

// faces for the cube
int indices[6][4] = { {1, 2, 6, 5}, {1, 5, 4, 0}, {5, 6, 7, 4}, {2, 6, 7, 3}, {0, 4, 7, 3}, {1, 0, 3, 2} };

// spacing for maze
int n = 6;
int halfN = n/2;

//camera variables
float movementSpeedFactor;
float pitchSensitivity;
float yawSensitivity;
float position;
float rotation;
float speed;
const double rad = 3.141592654 / 180.0;


//used the link to help me create the maze and solve it 
//http://algs4.cs.princeton.edu/41graph/Maze.java.html
void mazeStarter(){
	if(calcMode==true){
		//this first 2 loops set the borders as already visited 
		//so that the algorithm doesnt touch these
		for(int x = 0; x< size+2; x++){
			visited[x][0] = true;
			visited[x][size+1] = true;
		}
		for(int z =0; z<size +2; z++){
			visited[0][z] = true;
			visited[size+1][z] = true;
		}

		//initialize all the walls 
		for(int x=0; x< size+2;x++){
			for (int z=0; z<size+2;z++){
				north[x][z] = true;
				south[x][z] = true;
				east[x][z] = true;
				west[x][z] = true;
			}
		}
	}
}

void runMazeAlg(int x, int z){
	visited[x][z] = true;

	//this runs through the the unvisted neighbors
	while(!(visited[x][z+1]) || !(visited[x+1][z]) || !(visited[x][z-1]) || !(visited[x-1][z])){
		while (true){
			// get random neighbor
			int r = rand()%4;
            if (r == 0 && !visited[x][z+1]) {
                north[x][z] = false;
                south[x][z+1] = false;
                runMazeAlg(x, z + 1);
                break;
            }
            else if (r == 1 && !visited[x+1][z]) {
                east[x][z] = false;
                west[x+1][z] = false;
                runMazeAlg(x+1, z);
                break;
            }
            else if (r == 2 && !visited[x][z-1]) {
                south[x][z] = false;
                north[x][z-1] = false;
                runMazeAlg(x, z-1);
                break;
            }
			else if (r == 3 && !visited[x-1][z]) {
                west[x][z] = false;
                east[x-1][z] = false;
                runMazeAlg(x-1, z);
                break;
            }
		}
	}
}
//starts making the maze
void generateMaze(){
	if(calcMode==true){
		runMazeAlg(1,1);
	}
	calcMode=false;
}

// draws the cube for each point which has a wall
void drawCube (int x, int z, int c, int d){
	// the vertices as computed from the x and z coordinates
	int verts [8][3] = {{x,0,z},{x,10,z},{x+halfN+c,10,z},{x+halfN+c,0,z},{x,0,z+halfN+d},{x,10,z+halfN+d},{x+halfN+c,10,z+halfN+d},{x+halfN+c,0,z+halfN+d}};
	// for each face
	for (int index = 0; index < 6; index ++){
		glBegin(GL_POLYGON);	
			// for each four corners of a face
			for(int i = 0; i < 4; i++){		
				int vIndex = indices[index][i]; 
				int x = verts[vIndex][0];
				int y = verts[vIndex][1];
				int z = verts[vIndex][2];
				glVertex3f(x,y,z);
			}
		glEnd();
	}
}

//draws the grid
void drawMesh(){
	// mulitples of four to allow for more spacing; can be adjusted by changing n
	for(int x = n; x<=size*n;x+=n){
		for(int z=n; z<=size*n;z+=n){
			if (south[x/n][z/n]){
				glColor4f(1,1,1,0.5);
			/*	glBegin(GL_QUADS);
					glVertex3f(x,10,z);
					glVertex3f(x,0,z);
					glVertex3f(x+4,0,z);
					glVertex3f(x+4,10,z);
				glEnd(); */
				glColor4f(1,0,0,0.5); 
				drawCube(x,z,halfN,-1);
	
			}
		 	if(north[x/n][z/n]){
				glColor4f(1,1,1,0.5);
		 	/*	glBegin(GL_QUADS);
					glVertex3f(x,10,z+4);
					glVertex3f(x,0,z+4);
					glVertex3f(x+4,0,z+4);
					glVertex3f(x+4,10,z+4);
				glEnd(); */
				glColor4f(0,1,0,0.5);	
				drawCube(x,z+n,halfN,-1);

			}
			if (west[x/n][z/n]){
				glColor4f(1,1,1,0.5);
			/*	glBegin(GL_QUADS);
					glVertex3f(x,10,z);
					glVertex3f(x,0,z);
					glVertex3f(x,0,z+4);
					glVertex3f(x,10,z+4);
				glEnd(); */
				glColor4f(0,0,1,0.5); 
				drawCube(x,z,-1,halfN);
	
			}
			if (east[x/n][z/n] || x == size*n){
				glColor4f(1,1,1,0.5);
			/*	glBegin(GL_QUADS);
					glVertex3f(x+4,10,z);
					glVertex3f(x+4,0,z);
					glVertex3f(x+4,0,z+4);
					glVertex3f(x+4,10,z+4);
				glEnd(); */
				glColor4f(0.76,0,1,0.5); 
				drawCube(x+n,z,-1,halfN);
			}
		}
	}
}

void drawObj(){
	int numberOfFaces = objLoadObj.getNumberOfFaces();
	//printf("%i\n", numberOfFaces);
	LoadObj::Face face;
	LoadObj::Vertice vertices;
	for(int i = 0; i < numberOfFaces-1; i++){
		face = objLoadObj.getFaces(i);
		//printf("%i, %i, %i, %i\n", face.p1, face.p2, face.p3, face.p4);

		glBegin(GL_LINE_LOOP);
			vertices = objLoadObj.getVertices(face.p1-1);
			//printf("%f, %f, %f\n", vertices.x, vertices.y, vertices.z);
			glVertex3f(vertices.x, vertices.y, vertices.z);

			vertices = objLoadObj.getVertices(face.p2-1);
			//printf("%f, %f, %f\n", vertices.x, vertices.y, vertices.z);
			glVertex3f(vertices.x, vertices.y, vertices.z);

			vertices = objLoadObj.getVertices(face.p3-1);
			//printf("%f, %f, %f\n", vertices.x, vertices.y, vertices.z);
			glVertex3f(vertices.x, vertices.y, vertices.z);

			vertices = objLoadObj.getVertices(face.p4-1);
			//printf("%f, %f, %f\n", vertices.x, vertices.y, vertices.z);
			glVertex3f(vertices.x, vertices.y, vertices.z);
		glEnd();
	}
}

void cleanArrays(){
	//reset all the arrays 
	for(int x=0; x< size+2;x++){
		for (int z=0; z<size+2;z++){
			north[x][z] = false;
			south[x][z] = false;
			east[x][z] = false;
			west[x][z] = false;
			visited[x][z] = false;
		}
	}
}

void keyboard(unsigned char key, int x, int y){

	switch (key){

		case 'q'://quit the program
		case 27:
			exit (0);
			break;
		case 'r':
			calcMode = true;
			cleanArrays();
			break;
			
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	/* arrow key presses move the camera */
	switch(key)
	{
		case GLUT_KEY_LEFT:
			if(camPos[0]>-10){
				camPos[0] -= 2;
			}
			break;

		case GLUT_KEY_RIGHT:
			if(camPos[0]<size+70){
				camPos[0] += 2;
			}
			break;

		case GLUT_KEY_UP:
			camPos[2] -= 2;
			break;

		case GLUT_KEY_DOWN:
			if(camPos[2]<size+70){
				camPos[2] += 2;
			}
			break;
		
		case GLUT_KEY_HOME:
			camPos[1] += 50;
			break;

		case GLUT_KEY_END:
			if(camPos[1]>10){
				camPos[1] -= 50;
			}
			break;

	}
	glutPostRedisplay();
}

void special2(int key, int x, int y)
{
	/* arrow key presses move the camera */
	switch(key)
	{
		case GLUT_KEY_LEFT:
			//if(camPos2[0]>-10){
				camPos2[0] -= 1;
			//}
			break;

		case GLUT_KEY_RIGHT:
			//if(camPos2[0]<size+70){
				camPos2[0] += 1;
			//}
			break;

		case GLUT_KEY_UP:
			//if(camPos2[2]>70){
			 	camPos2[2] -= 1;
			//}
			break;

		case GLUT_KEY_DOWN:
			//if(camPos2[2]<size+70){
				camPos2[2] += 1;
			//}
			break;
		
		case GLUT_KEY_HOME:
			camPos2[1] += 1;
			break;

		case GLUT_KEY_END:
			//if(camPos2[1]>10){
			camPos2[1] -= 1;
			//}
			break;

	}
	glutPostRedisplay();
}


void init(void){
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);

}
void init2(void){
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);

}
void idle(){
	glutSetWindow(window1);
	glutPostRedisplay();
	glutSetWindow(window2);
	glutPostRedisplay();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
	glColor3f(1,1,1);

	//code for drawing the maze
	size = 20;
	//generateWalls();
	mazeStarter();
	generateMaze();
	drawMesh();
	drawObj();
	glutSwapBuffers();
}
void display2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos2[0], camPos2[1], camPos2[2], 0,0,0, 0,1,0);
	glColor3f(1,1,1);

	//code for drawing the maze
	size = 20;
	mazeStarter();
	generateMaze();
	drawMesh();

	glutSwapBuffers();
}

void glutCallBacks(){
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard); //registers "keyboard" as the keyboard callback function
	glutSpecialFunc(special);	//registers "special" as the special function callback
	glutIdleFunc(idle);
}



void glutCallBacks2(){
	glutDisplayFunc(display2);	//registers "display" as the display callback function
	glutSpecialFunc(special2);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(special);
	//initMenu();

}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	
	objLoadObj.setVertices();

	glutInitWindowSize(w, h);
	glutInitWindowPosition(100, 100);
	window1 = glutCreateWindow("Maze Top View");	//creates the window
	glutCallBacks();
	init();

	glutInitWindowSize(w,h);
	glutInitWindowPosition(1000,100);
	window2 = glutCreateWindow("Maze 1st Person");
	glutCallBacks2();
	init2();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
