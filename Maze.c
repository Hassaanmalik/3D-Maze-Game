//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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
LoadObj ghostLoadObj;

//camera positions

float camPos[] = {100, 200, 100};



//setting the window height and width
const float w = 800;
const float h = 800;
const float wMid = w/2;
const float hMid = h/2;

// coordinates for the prize
int px = 78;
int py = 5;
int pz = 78;



GLfloat light_pos[] = {0.0,100.0,0,1.0};

//maze variables
int size = 20; //change size variable to global
bool north[22][22];
bool south[22][22];
bool east[22][22];
bool west[22][22];
bool visited[22][22];

float playerX = size/2.0 * 6;
float playerY = 2;
float playerZ = size/2.0 * 6;
float camPos2[] = {playerX,playerY,playerZ};

//Ghost cordinates
float ghostX = 0, ghostY = 0, ghostZ = 0;
int ghostAngle = 0;

//Ghost's eye loaction
char *ghosteye = "north";


int oldMouseX;
int oldMouseY;

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
float viewX=0;
float viewY=0;
float viewZ=0;
const double rad = 3.141592654 / 180.0;

//materials

float m_amb[] ={ 0.05375f, 0.05f, 0.06625f, 0.82f };
float m_diff[] ={ 0.18275f, 0.17f, 0.22525f, 0.82f};
float m_spec[] ={0.332741f, 0.328634f, 0.346435f, 0.82f };
float shiny =38.4f ;

//an array for iamge data
GLubyte* tex1;
GLubyte* tex2;
GLubyte* tex3;

GLuint textures[3];

int width, height, max;

int TrevorsCheatArray[500][4][2];
int NavleensAPest = 0;
//used the link to help me create the maze and solve it
//http://algs4.cs.princeton.edu/41graph/Maze.java.html

GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;

	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file);
		exit(0);
	}
	printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);
	while(c == '#')
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd);
	fscanf(fd, "%d %d %d", &n, &m, &k);

	printf("%d rows  %d columns  max value= %d\n",n,m,k);

	nm = n*m;

	img = (GLubyte*)(malloc(3*sizeof(GLuint)*nm));

	s=255.0/k;


	for(i=0;i<nm;i++)
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	*width = n;
	*height = m;
	*max = k;

	return img;
}

void texture(){
	tex1 = LoadPPM("rockypath.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[0]); //Stores the texture at the array location 0
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Sets up needed texture properites
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1); //Instantiates the texture

	tex2 = LoadPPM("wood256.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[1]); //Stores the texture at the array location 1
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Sets up needed texture properties
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex2); //Instantiates the texture

	tex3 = LoadPPM("interface.ppm", &width, &height, &max);
	glBindTexture(GL_TEXTURE_2D, textures[2]); //Stores the texture at the array location 2
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Sets up needed texture properties
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex3); //Instantiates the texture
}

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
	//int verts [8][3] = {{x,0,z},{x,10,z},{x+c,10,z},{x+c,0,z},{x,0,z+d},{x,10,z+d},{+c,10,z+d},{x+c,0,z+d}};
	// for each face
	TrevorsCheatArray[NavleensAPest][0][0]=verts[0][0];
	TrevorsCheatArray[NavleensAPest][1][0]=verts[3][0];
	TrevorsCheatArray[NavleensAPest][2][0]=verts[4][0];
	TrevorsCheatArray[NavleensAPest][3][0]=verts[7][0];

	TrevorsCheatArray[NavleensAPest][0][1]=verts[0][2];
	TrevorsCheatArray[NavleensAPest][1][1]=verts[3][2];
	TrevorsCheatArray[NavleensAPest][2][1]=verts[4][2];
	TrevorsCheatArray[NavleensAPest][3][1]=verts[7][2];
	NavleensAPest++;
	int vIndex;
	for (int index = 0; index < 6; index ++){
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBegin(GL_POLYGON);
			// for each four corners of a face
		//	for(int i = 0; i < 4; i++){
				vIndex = indices[index][0];
				glTexCoord2f(0,1);
				glVertex3f(verts[vIndex][0],verts[vIndex][1], verts[vIndex][2]);

				vIndex = indices[index][1];
				glTexCoord2f(0,0);
				glVertex3f(verts[vIndex][0],verts[vIndex][1], verts[vIndex][2]);

				vIndex = indices[index][2];
				glTexCoord2f(1,0);
				glVertex3f(verts[vIndex][0],verts[vIndex][1], verts[vIndex][2]);

				vIndex = indices[index][3];
				glTexCoord2f(1,1);
				glVertex3f(verts[vIndex][0],verts[vIndex][1], verts[vIndex][2]);
		//	}
		glEnd();
	}
}


bool prize = false;
//draws the grid
void drawMesh(){

	// mulitples of four to allow for more spacing; can be adjusted by changing n
	for(int x = n; x<=size*n;x+=n){
		for(int z=n; z<=size*n;z+=n){
			if (south[x/n][z/n]){
				drawCube(x,z,halfN,-1);

			}
		 	if(north[x/n][z/n]){
				drawCube(x,z+n,halfN,-1);
			}
			if (west[x/n][z/n]){
				drawCube(x,z,-1,halfN);
			}
			if ((east[x/n][z/n] || x == size*n)){
				drawCube(x+n,z,-1,halfN);
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

void drawFloor(){
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_POLYGON);

		glTexCoord2f(0,1);
		glVertex3f(n,0,n);

		glTexCoord2f(0,0);
		glVertex3f(n,0,size*n);

		glTexCoord2f(1,0);
		glVertex3f(size*n,0,size*n);

		glTexCoord2f(1,1);
		glVertex3f(size*n,0,n);
	glEnd();
}
bool checkValidMove(int projectedX, int projectedZ){
	for(int j = 0; j <= 5; j ++){
	printf("x0: %i x1 %i x2 %i x3 %i \n y0: %i y1 %i y2 %i y3 %i \n",TrevorsCheatArray[j][0][0],TrevorsCheatArray[j][1][0],TrevorsCheatArray[j][2][0],TrevorsCheatArray[j][3][0],TrevorsCheatArray[j][1][0],TrevorsCheatArray[j][1][1],TrevorsCheatArray[j][2][1],TrevorsCheatArray[j][3][1]);

		//for(int i = 0; i < 4; i++){
		//printf("projectedX values is %i \n projected Z is %i \n", projectedX, projectedZ);
			if(projectedX >= TrevorsCheatArray[j][0][0] && projectedX <= TrevorsCheatArray[j][3][0]){
				if(projectedZ >= TrevorsCheatArray[j][0][1] && projectedZ <= TrevorsCheatArray[j][3][1]){
					//printf("true");
					//printf(" Wall Number : % i     X Range: %i min %i max Zrange: %i min %i Max  \n", j,TrevorsCheatArray[j][0][0],TrevorsCheatArray[j][3][0],TrevorsCheatArray[j][0][1],TrevorsCheatArray[j][3][1]);
					return false;

				}
			}
/*			if(projectedZ >= TrevorsCheatArray[j][0][1] && projectedZ <= TrevorsCheatArray[j][3][1]){
				return false;
			}*/
			//if(TrevorsCheatArray[j][i][0]==projectedX || TrevorsCheatArray[j][i][1]==projectedZ){
			//	return false;
				//printf("SHUT UP NAVLEEN");
				//projectedX = camPos2[0];
			//}
			//else{
			//	camPos2[0]=projectedX;
				//printf("SHUT UP NAVLEEN");

			//}

			//if(TrevorsCheatArray[j][i][1]==projectedZ){
				//printf("I FUCKEN KNOW THAT");
			//	return false;
			//	projectedZ = camPos2[2];
			//}
			//else{
			//	camPos2[2]=projectedZ;
			//}

		//}

	}
	return true;
	//glutPostRedisplay();

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

/*void special(int key, int x, int y)
{	switch(key)
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
}*/

void special2(int key, int x, int y)
{
	float tempT = camPos2[0];

	/* arrow key presses move the camera */
	switch(key)
	{
		case GLUT_KEY_LEFT:
			tempT-=1;
			//if(camPos2[0]>-10){
				if(checkValidMove(tempT,camPos2[2])){
					//camPos2[0] -= 1;
					printf(" camPos2[0] %f true \n\n\n\n",camPos2[0]);

				}
				else{
					//camPos2[0] +=1;
					printf("camPos2[0]  %f false \n\n\n\n",camPos2[0]);
				}
				//camPos2[0] -= 1;
			//}
			break;

		case GLUT_KEY_RIGHT:
			//if(camPos2[0]<size+70){
				if(checkValidMove(camPos2[0]+1,camPos2[2])){
					camPos2[0] += 1;
				}
				//camPos2[0] += 1;
			//}
			break;

		case GLUT_KEY_UP:
			//if(camPos2[2]>70){
				if(checkValidMove(camPos2[0],camPos2[2]-1)){
					camPos2[2] -= 1;
				}

			 	//camPos2[2] -= 1;
			//}
			break;

		case GLUT_KEY_DOWN:
			//if(camsPos2[2]<size+70){
				if(checkValidMove(camPos2[0],camPos2[2]+1)){
					camPos2[2] += 1;
				}
//				camPos2[2] += 1;
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

void showWin(const char *text, int length, int x, int y){
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double [16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0,800,0,600,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);
	for(int i = 0; i < length; i ++){
		glColor3f(0,1,0);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
//	glEnable(GL_FOG);

}

bool checkWin(){
	if (camPos2[0] == px && camPos[2]==pz){
		std::string text;
		text = "You Won!";
		showWin(text.data(), text.size(), 300,350);
		return true;
	}
	return false;
}

bool checkLose(){
	if (camPos2[0] == px && camPos[2]==pz){ // change to equal AI
		std::string text;
		text = "You Lost....";
		showWin(text.data(), text.size(), 300,350);
		return true;
	}
	return false;
}


// draw a tropy for the user to get to
void drawPrize(){
	// turn off fog for that one object
	glDisable(GL_FOG);

	// move to that position in the maze
	glTranslatef(px+4,py+3,pz+4);
	glRotatef(90,1,0,0);
	glutSolidCone(1,1,20,20);
	GLUquadricObj *quadratic= gluNewQuadric();
	gluCylinder(quadratic,0.3,0.3, 2, 40,5); 	// draw body
	gluCylinder(quadratic,1,0.5, 1, 40,5);		// draw cup
	glTranslatef(0,0,2); 						// move to the bottom
	glutSolidTorus(0.1,0.4,30, 30); 			// draw base
}

void checkStatus(){
	// if win; need to fix this code so it will exit upon a win condition (ie Trevor?)
	if(checkWin()){
		exit(1);
	}
	if(checkLose()){
		exit(1);
	}
}

void light(){
	glColor3f(1, 1, 1);

	//GLfloat position[] = {0.0,100.0,0,1.0};
	float amb0[4] = {1, 1, 1, 1};
	float diff0[4] = {1, 0, 0, 1};
	float spec0[4] = {0, 0, 1, 1};

	// set the values for the first light source
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    /* m_amb = ghostLoadObj.getAmbient(); */
    /* m_diff = ghostLoadObj.getDiffuse(); */
    /* m_spec = ghostLoadObj.getReflectivity(); */
    /* shiny = ghostLoadObj.getPhongSpecular(); */

	/* glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb); */
	/* glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff); */
	/* glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec); */
	/* glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny); */

}

void fog(){
	glClearColor(0.3, 0.3, 0.3, 0.1);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_FOG);
	{
	GLfloat fogColor[] = {0.4f, 0.4f, 0.4f, 0.1};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_START, 45.0f);
    glFogf(GL_FOG_END, 1000.0f);
    glFogf (GL_FOG_DENSITY, 0.05);
    glHint(GL_FOG_HINT,GL_FASTEST);
	}
}

void init(void){
//	glClearColor(0.3, 0.3, 0.3, 0.1);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);
//	light();


}
void init2(void){
	glClearColor(0.3, 0.3, 0.3, 0.1);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);
	//light();
}
void idle(){
	glutSetWindow(window1);
	glutPostRedisplay();
	glutSetWindow(window2);
	glutPostRedisplay();
}

void ghostAI(int ghost){
    //Check wall colision
    if(ghosteye == "north")//90
        ghostX+=0.01;
    else if(ghosteye == "south")//270
        ghostX-=0.01;
    else if(ghosteye == "east")//0
        ghostZ+=0.01;
    else if(ghosteye == "west")//180
        ghostZ-=0.01;

    //if(check(ghostX, ghostZ)){
        if(ghost == 1){
            ghostAngle ++;
        }else if(ghost == 2){
            ghostAngle --;
        }
    //}

    if(ghostAngle%4 == 1)
        ghosteye = "north";
    else if(ghostAngle%4 == 3)
        ghosteye = "south";
    else if(ghostAngle%4 == 0)
        ghosteye = "east";
    else if(ghostAngle%4 == 2)
        ghosteye = "west";

    //ghostX+=0.01;
    glTranslatef(ghostX, ghostY, ghostZ);
    glRotatef((ghostAngle%4)*90, 0, 1, 0);

    printf("ghostX: %f, ghostY: %f, ghostZ: %f\n", ghostX, ghostY, ghostZ);

    //Draw ghost
    ghostLoadObj.drawObj();
}

void drawObj(){
    glPushMatrix();

    /* glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb); */
    /* glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff); */
    /* glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec); */
    /* glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny); */

    /* StdDraw.filledCircle(N/2.0 + 0.5, N/2.0 + 0.5, 0.375); */
    //object's stating location and rotation
    glRotatef(90, -1, 0, 0);
    glTranslatef(0, 10, 0);
    /* glScalef(.5, .5, .5); */
    /* ghostLoadObj.mtlForObj(); */

    //Object's AI
    ghostAI(1);
    glPopMatrix();
}
void motion(int mouseX, int mouseY){


}
void passive(int mouseX, int mouseY){
	//SetCursorPos(0,0);
	int mid_x = w/2;
	int mid_y = h/2	;
	float angleX =0.0f;
	float angleY=0.0f;
    if ((mid_x - mouseX) > 0 && oldMouseX > mouseX){		// mouse moved to the left
		angleX -= 0.1f;
	}else if ((mid_x - mouseX) < 0 && oldMouseX < mouseX){	// mouse moved to the right
		angleX += 0.1f;
	}
	if ((mid_y - mouseY) > 0){		// mouse moved to the up
		angleY += 0.1f;
	}else if ((mid_y - mouseY) < 0){	// mouse moved to the down
		angleY -= 0.1f;
	}
	viewY += angleY * 2;
	viewX += angleX * 2;

    oldMouseX = mouseX;
	oldMouseY = mouseY;
//	printf("passive: %i,%i\n", mouseX, mouseY);

}
void mouse(int btn, int state, int mouseX, int mouseY){
    //case WM_MOUSEMOVE:
    // save old mouse coordinates
    //oldMouseX = mouseX;
    //oldMouseY = mouseY;

    // get mouse coordinates from Windows
    //mouseX = LOWORD(lParam);
    //mouseY = HIWORD(lParam);

    // these lines limit the camera's range
    //if (mouseY < 60)
    //	mouseY = 60;
    //if (mouseY > 450)
    //	mouseY = 450;
    /*			float angleX =0.0f;
                float angleY=0.0f;

                if ((mouseX - oldMouseX) > 0){		// mouse moved to the right
                angleX += 3.0f;
                }
                else if ((mouseX - oldMouseX) < 0){	// mouse moved to the left
                angleX -= 3.0f;
                }
                if ((mouseY - oldMouseY) > 0){		// mouse moved to the right
                angleY += 3.0f;
                }
                else if ((mouseY - oldMouseY) < 0){	// mouse moved to the left
                angleY -= 3.0f;
                }
                viewY += angleY * 2;
                viewX =+ angleX * 2;*/

    //return 0;
    //break;


}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* glClearColor(0.3, 0.3, 0.3, 0.1); */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
    glColor3f(1,1,1);
    /* fog(); */

    //code for drawing the maze
    //size = 20;
    /* generateWalls(); */
    mazeStarter();
    generateMaze();
    drawMesh();
    drawPrize();
    /* drawObj(); */
    /* if(checkWin()){ */
    /* } */
    glutSwapBuffers();
}
void display2()
{

    //size/2.0 + 6, 0, size/2.0 + 6
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	//fog();
	light();
	gluLookAt(camPos2[0], camPos2[1], camPos2[2], viewX,viewY,0, 0,1,0);
	//printf("The viewX angle is %f \n the viewY angle is %f \n", viewX, viewY);
	glColor3f(1,1,1);


    //code for drawing the maze
    size = 20;
    mazeStarter();
    generateMaze();
    drawMesh();
    drawFloor();

	checkStatus();
	drawPrize();
	drawObj();
	glutSwapBuffers();
}

void glutCallBacks(){
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard); //registers "keyboard" as the keyboard callback function
	//glutSpecialFunc(special);	//registers "special" as the special function callback
	glutIdleFunc(idle);
}



void glutCallBacks2(){
    glutDisplayFunc(display2);	//registers "display" as the display callback function
    glutSpecialFunc(special2);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passive);
    //glutKeyboardFunc(keyboard);
    //glutSpecialFunc(special);
    //initMenu();

}

/* main function - program entry point */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);		//starts up GLUT

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    ghostLoadObj.loadObj("ghost", "ObjFile/");

    glutInitWindowSize(w, h);
    glutInitWindowPosition(50, 50);
    window1 = glutCreateWindow("Maze Top View");	//creates the window
    glutCallBacks();
    init();

    glutInitWindowSize(w,h);
    glutInitWindowPosition(1000,100);
    window2 = glutCreateWindow("Maze 1st Person");
    glutCallBacks2();
    init2();

    glEnable(GL_TEXTURE_2D);
    glGenTextures(3, textures); //generate 3 texture IDs, store them in array "textures"
    texture();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glutMainLoop();				//starts the event loop

    return(0);					//return may not be necessary on all compilers
}
