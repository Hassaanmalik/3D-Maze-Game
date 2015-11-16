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

//camera positions
float camPos[] = {0, 0, 0};

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

//draws the grid
void drawMesh(){
	for(int x = 1; x<=size;x++){
		for(int z=1; z<=size;z++){
			if(south[x][z]){
				glColor4f(1,0,0,0.5);
				glBegin(GL_QUADS);
					glVertex3f(x,10,z);
					glVertex3f(x,0,z);
					glVertex3f(x+1,0,z);
					glVertex3f(x+1,10,z);
				glEnd();
			}
			if(north[x][z]){
				glColor4f(0,1,0,0.5);
				glBegin(GL_QUADS);
					glVertex3f(x,10,z+1);
					glVertex3f(x,0,z+1);
					glVertex3f(x+1,0,z+1);
					glVertex3f(x+1,10,z+1);
				glEnd();
			}
			if(west[x][z]){
				glColor4f(0,0,1,0.5);
				glBegin(GL_QUADS);
					glVertex3f(x,10,z);
					glVertex3f(x,0,z);
					glVertex3f(x,0,z+1);
					glVertex3f(x,10,z+1);
				glEnd();
			}
			if(east[x][z]){
				glColor4f(0.76,0,1,0.5);
				glBegin(GL_QUADS);
					glVertex3f(x+1,10,z);
					glVertex3f(x+1,0,z);
					glVertex3f(x+1,0,z+1);
					glVertex3f(x+1,10,z+1);
				glEnd();
			}
		}
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

void init(void){
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);

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
	mazeStarter();
	generateMaze();
	drawMesh();

	glutSwapBuffers();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Maze");	//creates the window
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}