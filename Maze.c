//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h> //added math libary to code to allow for mouse and others

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

int test=0;
bool lightCheck = true;
bool done = false;

//setting the window height and width
const float w = 800;
const float h = 800;
const float wMid = w/2;
const float hMid = h/2;

// coordinates for the prize
int px = 10;
int py = 5;
int pz = 8;
// for easy do (83, 0, 78);

bool northPlayer = true;
bool eastPlayer = false;
bool westPlayer = false;
bool southPlayer = false;

int viewXOrigin = 0;
int viewYOrigin = 0;

bool ghostCaught = false;


GLfloat light_pos[] = {77,50.0,82,1.0};
GLfloat light_pos1[] = {87,50.0,82,1.0};//starting light positions surrounding the player
GLfloat light_pos2[] = {82,50.0,77,1.0};
GLfloat light_pos3[] = {82,50.0,87,1.0};


//maze variables
int size = 20; //change size variable to global
bool north[22][22];
bool south[22][22];
bool east[22][22];
bool west[22][22];
bool visited[22][22];

float playerX = 82;
float playerY = 5;//starting player positions
float playerZ = 82;
float camPos2[] = {playerX,playerY,playerZ};

//float camPos2[] = {0,0,0};

//Ghost cordinates
float ghostX[3] = {playerX, playerX, playerX}, ghostY[3] = {3, 3, 3}, ghostZ[3] = {playerZ, playerZ, playerZ};
int ghostAngle[3] = {0,0,0};
int ghostStart = 0;

//Ghost's eye loaction
char *ghosteye[3] = {"north","north","north"};


//boolean for walls
bool wallOn = true;

int waitTime = 20;

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


float viewX=0;
float viewY=0;
float viewZ=0;
const double rad = 3.141592654 / 180.0;

//materials
//wall material
float m_amb[] ={ 0.05375f, 0.05f, 0.06625f, 0.82f };
float m_diff[] ={ 0.18275f, 0.17f, 0.22525f, 0.82f};
float m_spec[] ={0.332741f, 0.328634f, 0.346435f, 0.82f };
float shiny =38.4f ;

//floor material
float m_amb1[] ={0.0f, 0.0f, 0.0f, 1.0f };
float m_diff1[] ={0.01f, 0.01f, 0.01f, 1.0f };
float m_spec1[] ={0.50f, 0.50f, 0.50f, 1.0f };
float shiny1 =32.0f ;


//trophy material
float m_amb2[] ={0.24725f, 0.1995f, 0.0745f, 1.0f };
float m_diff2[] ={0.75164f, 0.60648f,0.60648f, 1.0f };
float m_spec2[] ={0.628281, 0.555802f,0.366065f, 1.0f };
float shiny2 =0.4f ;

//an array for iamge data
GLubyte* tex1;
GLubyte* tex2;
GLubyte* tex3;

GLuint textures[3];

int width, height, max;

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
 //       printf("%s is not a PPM file!\n",file);
        exit(0);
    }
//    printf("%s is a PPM file\n", file);
    fscanf(fd, "%c",&c);
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n",b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);

//    printf("%d rows  %d columns  max value= %d\n",n,m,k);

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
    tex1 = LoadPPM("bricks.ppm", &width, &height, &max);
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
    //for each face
    int vIndex;

    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

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
    glPopMatrix();
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
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec1);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny1);

    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glBegin(GL_POLYGON);

    glTexCoord2f(0,1);
    glVertex3f(n,0,n);

    glTexCoord2f(0,0);
    glVertex3f(n,0,size*n+n);

    glTexCoord2f(1,0);
    glVertex3f(size*n+n,0,size*n+n);

    glTexCoord2f(1,1);
    glVertex3f(size*n+n,0,n);
    glEnd();
    glPopMatrix();
}

bool hitTest(float cX, float cZ, float pX, float pZ){
    /* printf("cX: %f, cZ: %f, pX: %f, pZ: %f\n", cX, cZ, pX, pZ); */
    int i = floor(cX/n);
    int j = floor(cZ/n);
    /* printf("i: %i,j: %i, ",i,j); */
    /* printf("north: %d, ", north[i][j]); */
    /* printf("south: %d, ", south[i][j]); */
    /* printf("east: %d, ", east[i][j]); */
    /* printf("west: %d\n", west[i][j]); */
    if(pZ > cZ){//North for camera and North for maze
        /* printf("North\n"); */
        return north[i][j];
    }else if(pZ < cZ){//South for camera and South for maze
        /* printf("South\n"); */
        return south[i][j];
    }else if(pX > cX){//East for camera and East for maze
        /* printf("East\n"); */
        return east[i][j];
    }else if(pX < cX){//West for camera and West for maze
        /* printf("West\n"); */
        return west[i][j];
    }
}
void keyboard(unsigned char key, int x, int y){

    switch (key){

        case 'q'://quit the program
        case 27:
            exit (0);
            break;
        case 'r'://resets the maze
        case 'R':
            calcMode = true;
            cleanArrays();
            camPos2[0] =playerX;
            camPos2[1] =playerY;
            camPos2[2] =playerZ;
            northPlayer = true;
            southPlayer = false;
            eastPlayer = false;
            westPlayer = false;
            ghostCaught = false;
            done = false;
            ghostX[0] = playerX; ghostY[0] = 3; ghostZ[0] = playerZ; ghostAngle[0] = 0; ghosteye[0] = "north";
            ghostX[1] = playerX; ghostY[1] = 3; ghostZ[1] = playerZ; ghostAngle[1] = 0; ghosteye[1] = "north";
            ghostStart = 0;
            break;
        case 'l':
        case 'L':
            if(lightCheck){
                lightCheck = false;//switch lighting on or off
            }
            else{
                lightCheck = true;
            }
            break;
        case 'w':
        case 'W':
            if(wallOn){
                wallOn=false;
            }
            else{
                wallOn=true;
            }
    }

    glutPostRedisplay();
}
void special2(int key, int x, int y)
{
    /* arrow key presses move the camera */
    switch(key)
    {
        //moves the player in a forward direction with the lights following
        case GLUT_KEY_UP:
            //if(hitTest(camPos2[0],camPos2[2])){

            if(northPlayer && !hitTest(camPos2[0], camPos2[2], camPos2[0], camPos2[2]-1)){
                camPos2[2] -=1;
                light_pos[2] -=1;
                light_pos1[2] -= 1;
                light_pos2[2] -= 1;
                light_pos3[2] -= 1;
            }
            else if(southPlayer && !hitTest(camPos2[0], camPos2[2], camPos2[0], camPos2[2]+1)){
                camPos2[2] +=1;
                light_pos[2] +=1;
                light_pos1[2] += 1;
                light_pos2[2] += 1;
                light_pos3[2] += 1;
            }
            else if(eastPlayer && !hitTest(camPos2[0], camPos2[2], camPos2[0]+1, camPos2[2])){
                camPos2[0] +=1;
                light_pos[0] +=1;
                light_pos1[0] += 1;
                light_pos2[0] += 1;
                light_pos3[0] += 1;
            }
            else if(westPlayer && !hitTest(camPos2[0], camPos2[2], camPos2[0]-1, camPos2[2])){
                camPos2[0] -=1;
                light_pos[0] -=1;
                light_pos1[0] -= 1;
                light_pos2[0] -= 1;
                light_pos3[0] -= 1;
            }
            //}
            //else{
            /* camPos2[2] -= 1; */
            //}
            break;
            //moves the player in a backward direction with the lights following
        case GLUT_KEY_DOWN:
            //if(hitTest(camPos2[0],camPos2[2])){

            if(northPlayer && !hitTest(camPos2[0], camPos2[2], camPos2[0], camPos2[2]+1)){
                camPos2[2] += 1;
                light_pos[2] += 1;
                light_pos1[2] += 1;
                light_pos2[2] += 1;
                light_pos3[2] += 1;

            }
            else if(southPlayer && !hitTest(camPos2[0], camPos2[2], camPos2[0], camPos2[2]-1)){
                camPos2[2] -=1;
                light_pos[2] -=1;
                light_pos1[2] -= 1;
                light_pos2[2] -= 1;
                light_pos3[2] -= 1;
            }
            else if(eastPlayer && !hitTest(camPos2[0], camPos2[2], camPos2[0]-1, camPos2[2])){
                camPos2[0] -= 1;
                light_pos[0] -= 1;
                light_pos1[0] -= 1;
                light_pos2[0] -= 1;
                light_pos3[0] -= 1;
            }
            else if(westPlayer && !hitTest(camPos2[0], camPos2[2], camPos2[0]+1, camPos2[2])){
                camPos2[0] +=1;
                light_pos[0] +=1;
                light_pos1[0] += 1;
                light_pos2[0] += 1;
                light_pos3[0] += 1;
            }

            //}
            //else{
            /* camPos2[2] += 1; */
            //}
            break;

            //reorients the player to their current left
        case GLUT_KEY_LEFT:
            glutWarpPointer(w/2,h/2);//centers the cursor in the screen

            viewXOrigin = viewXOrigin - 90;
            if(northPlayer){
                westPlayer = true;
                northPlayer = false;
            }
            else if(westPlayer){
                southPlayer = true;
                westPlayer = false;
            }
            else if(southPlayer){
                eastPlayer = true;
                southPlayer = false;
            }
            else if(eastPlayer){
                northPlayer = true;
                eastPlayer = false;
            }
            /* camPos2[0] -= 1; */
            break;

            //reorients the player to their current right
        case GLUT_KEY_RIGHT:
            glutWarpPointer(w/2,h/2);//centers cursor on the middle of the screen

            viewXOrigin += 90;
            if(northPlayer){
                eastPlayer = true;
                northPlayer = false;
            }
            else if(eastPlayer){
                southPlayer = true;
                eastPlayer = false;
            }
            else if(southPlayer){
                westPlayer = true;
                southPlayer = false;
            }
            else if(westPlayer){
                northPlayer = true;
                westPlayer = false;
            }
            ///camPos2[0]+=1;
            break;
            //zoooms out
        case GLUT_KEY_HOME:
            camPos2[1] += 1;
            break;
            //zooms in
        case GLUT_KEY_END:
            //if(camPos2[1]>10){
            camPos2[1] -= 1;
            //}
            break;

    }
    ghostStart ++;
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
	int i = (int)camPos2[0];
	int j = (int)camPos2[2];
	//printf ("px: %i, cam %i\n", px, i);
	//printf ("pz: %i, cam %i\n", pz, j);
   	if (px-1 <=i && i >= px +1 && pz  == j){
        std::string text, reset;
        text = "You Won!";
        showWin(text.data(), text.size(), 350,350);
        reset = "Press 'r' to reset";
        showWin(reset.data(), reset.size(), 330,330);
        done = true;
        return true;
    }
    return false;
}

bool checkLose(){
    if(ghostStart >= waitTime){
        if (((ghostX[0] - 2 <= camPos2[0]  &&  camPos2[0] <= ghostX[0] + 2) && (ghostZ[0] - 2 <= camPos2[2]  &&  camPos2[2] <= ghostZ[0] + 2)) ||
                ((ghostX[1] - 2 <= camPos2[0]  &&  camPos2[0] <= ghostX[1] + 2) && (ghostZ[1] - 2 <= camPos2[2]  &&  camPos2[2] <= ghostZ[1] + 2))){ // change to equal AI
            std::string text, reset;
            text = "You Lost....";
            showWin(text.data(), text.size(), 350,350);
            reset = "Press 'r' to reset";
            showWin(reset.data(), reset.size(), 330,330);
            ghostCaught = true;
            done = true;
            return true;
        }
    }
    return false;
}


// draw a tropy for the user to get to
void drawPrize(){
    // turn off fog for that one object
    glDisable(GL_FOG);

    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec2);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny2);


    // move to that position in the maze
    glTranslatef(px+4,0,pz+4);
    glRotatef(90,1,0,0);
    glutSolidCone(1,1,20,20);
    GLUquadricObj *quadratic= gluNewQuadric();
    gluCylinder(quadratic,0.3,0.3, 2, 40,5); 	// draw body
    gluCylinder(quadratic,1,0.5, 1, 40,5);		// draw cup
    glTranslatef(0,0,2); 						// move to the bottom
    glutSolidTorus(0.1,0.4,30, 30); 			// draw base

    glPopMatrix();
}

void checkStatus(){
    // if win; need to fix this code so it will exit upon a win condition (ie Trevor?)
   /* if(checkWin()){
      //  exit(1);
    	printf("win\n");
    }
    if(checkLose()){
        exit(1);
    } */
    checkWin();
    checkLose();
}

void light(){
    glColor3f(1, 1, 1);

    //GLfloat position[] = {0.0,100.0,0,1.0};
    float amb0[4] = {1, 1, 1, 1};
    float diff0[4] = {1, 1, 1, 1};
    float spec0[4] = {1, 1, 1, 1};

    // set the values for the first light source
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);


    float amb1[4] = {1, 1, 1, 1};
    float diff1[4] = {1, 1, 1, 1};
    float spec1[4] = {1, 1, 1, 1};

    // set the values for the second light source
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);


    float amb2[4] = {1, 1, 1, 1};
    float diff2[4] = {1, 1, 1, 1};
    float spec2[4] = {1, 1, 1, 1};

    // set the values for the third light source
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, amb2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diff2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spec2);


    float amb3[4] = {1, 1, 1, 1};
    float diff3[4] = {1, 1, 1, 1};
    float spec3[4] = {1, 1, 1, 1};

    // set the values for the fourth light source
    glLightfv(GL_LIGHT3, GL_POSITION, light_pos3);
    glLightfv(GL_LIGHT3, GL_AMBIENT, amb3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diff3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, spec3);


    if(lightCheck){
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHT3);
    }
    else{
        glDisable(GL_LIGHTING);
    }

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
        glFogf (GL_FOG_DENSITY, 0.10);
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
    glutWarpPointer(w/2,h/2);//begins game with cursor in center of screen

    glClearColor(0.3, 0.3, 0.3, 0.1);
    glColor3f(1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 2, 1, 1000);
    //glRotatef(45,camPos2[0],camPos2[1],camPos2[2]);
    //light();
}
void idle(){
    /* glutSetWindow(window1); */
    /* glutPostRedisplay(); */
    glutSetWindow(window2);
    glutPostRedisplay();
}

void ghostAI(int ghost){
    //Check wall colision
    if (!ghostCaught){
        bool hit = false;
        float ghostMovment = 0.1;
        if(ghosteye[ghost] == "north"){//90
            hit = hitTest(ghostX[ghost], ghostZ[ghost], ghostX[ghost]+ghostMovment, ghostZ[ghost]);
            if(!hit)
                ghostX[ghost]+=ghostMovment;
        }else if(ghosteye[ghost] == "south"){//270
            hit = hitTest(ghostX[ghost], ghostZ[ghost], ghostX[ghost]-ghostMovment, ghostZ[ghost]);
            if(!hit)
                ghostX[ghost]-=ghostMovment;
        }else if(ghosteye[ghost] == "east"){//0
            hit = hitTest(ghostX[ghost], ghostZ[ghost], ghostX[ghost], ghostZ[ghost]+ghostMovment);
            if(!hit)
                ghostZ[ghost]+=ghostMovment;
        }else if(ghosteye[ghost] == "west"){//180
            hit = hitTest(ghostX[ghost], ghostZ[ghost], ghostX[ghost], ghostZ[ghost]-ghostMovment);
            if(!hit)
                ghostZ[ghost]-=ghostMovment;
        }
        /* printf("hit: %d\n", hit); */
        if(hit){
            if(ghost == 0){
                ghostAngle[ghost] += rand() % 3 + 1;
            }else if(ghost == 1){
                ghostAngle[ghost] -= rand() % 3 + 1;
            }else if(ghost == 2){
                //Follow player
            }
        }

        if(ghostAngle[ghost]%4 == 3)
            ghosteye[ghost] = "north";
        else if(ghostAngle[ghost]%4 == 1)
            ghosteye[ghost] = "south";
        else if(ghostAngle[ghost]%4 == 2)
            ghosteye[ghost] = "east";
        else if(ghostAngle[ghost]%4 == 0)
            ghosteye[ghost] = "west";

        //ghostX+=0.01;
        glTranslatef(playerX-ghostX[ghost], 3-ghostY[ghost], playerZ-ghostZ[ghost]);
        glRotatef((ghostAngle[ghost]%4)*90, 0, 1, 0);

        /* printf("ghostX: %f, ghostY: %f, ghostZ: %f\n", ghostX, ghostY, ghostZ); */
    }
        //Draw ghost
    ghostLoadObj.drawObj();
}

void drawObj(){
    glPushMatrix();
    //object's stating location and rotation
    /* glRotatef(90, -1, 0, 0); */
    glTranslatef(playerX, 3, playerZ);
    glScalef(0.4, 0.4, 0.4);
    /* ghostLoadObj.mtlForObj(); */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec1);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny1);

    //Object's AI
    ghostAI(0);
    ghostAI(1);
    //ghostAI(2);
    glPopMatrix();
}
void motion(int mouseX, int mouseY){


}
void passive(int mouseX, int mouseY){
    int mid_x = w/2;
    int mid_y = h/2	;//sets values for center of screen
    float angleX =0.0f;
    float angleY=0.0f;


    angleX = (mouseX - mid_x)/10;//sets angle x based on the x position of the mouse with a restricted field of vision
    angleY =  (mid_y - mouseY)/5;//sets angle y based on the y position of the mouse with a restricted field of vision

    viewY = angleY;
    viewX = angleX;



}
void mouse(int btn, int state, int mouseX, int mouseY){
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


    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);
    glLightfv(GL_LIGHT3, GL_POSITION, light_pos3);


    fog();

    light();


    //based on the players current orientation in regards to the maze sets the ability to move the cameras orientation based on the mouse position.
    if(northPlayer){
        glRotatef(viewXOrigin + viewX, 0.0f, 1.0, 0.0f);
        glRotatef(-viewY, 1.0f, 0.0f, 0.0f);
        glTranslatef(-camPos2[0], -camPos2[1], -camPos2[2]);
    }
    if(southPlayer){
            glRotatef(viewXOrigin + viewX, 0.0f, 1.0, 0.0f);
            glRotatef(viewY, 1.0f, 0.0f, 0.0f);
            glTranslatef(-camPos2[0], -camPos2[1], -camPos2[2]);
    }
    //NOTE: hidden easter egg, the player can only look up and down when faceing 2 directions (north and south), if the player realises this they can use it to their
    //advantage to keep track of where they are going and where they are coming from
    if(westPlayer){
            glRotatef(viewXOrigin + viewX, 0.0f, 1.0, 0.0f);
            glTranslatef(-camPos2[0], -camPos2[1], -camPos2[2]);
    }
    if(eastPlayer){
            glRotatef(viewXOrigin + viewX, 0.0f, 1.0, 0.0f);
            glTranslatef(-camPos2[0], -camPos2[1], -camPos2[2]);


    }
    //glRotatef(45,0,1,0);
    //glRotatef(viewXOrigin,0,1,0);
    //gluLookAt(camPos2[0], camPos2[1], camPos2[2], viewX,viewY-viewXOrigin,0, 0,1,0);


   // printf("The viewX angle is %f \n the viewY angle is %f \n", viewX, viewY);
    glColor3f(1,1,1);


    //code for drawing the maze
    size = 20;
    if(wallOn){
        mazeStarter();
        generateMaze();
        drawMesh();
    }
    drawFloor();

    checkStatus();
    drawPrize();
    if(ghostStart >= waitTime)
        drawObj();
    glutSwapBuffers();
}

/*void glutCallBacks(){
    glutDisplayFunc(display);	//registers "display" as the display callback function
    glutKeyboardFunc(keyboard); //registers "keyboard" as the keyboard callback function
    //glutSpecialFunc(special);	//registers "special" as the special function callback
    glutIdleFunc(idle);
}*/



void glutCallBacks2(){

    glutDisplayFunc(display2);	//registers "display" as the display callback function
    if (!done){
        glutSpecialFunc(special2);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);
        glutPassiveMotionFunc(passive);
    }
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    //glutSpecialFunc(special);
    //initMenu();

}

/* main function - program entry point */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);		//starts up GLUT

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    ghostLoadObj.loadObj("ghost", "ObjFile/");

    /* glutInitWindowSize(w, h); */
    /* glutInitWindowPosition(50, 50); */
    /* window1 = glutCreateWindow("Maze Top View");	//creates the window */
    /* glutCallBacks(); */
    /* init(); */

    printf("Rules:\n");
    printf("- The point of the game is to move through the maze and find the trophy before the ghost catches you!\n");
    printf("Keyboard Actions:\n");
    printf("- Press'r'  to reset the maze\n");
    printf("- Press'w'  to turn walls on/off\n");
    printf("- Press the 'left' or 'right' key to move on the x axis\n");
    printf("- Press the 'up' or 'down' key to move on the z axis\n");
    printf("- Press the 'page up' or 'page down' key to move on the y axis\n");
    printf("- Press the 'l' key to turn on the lighting... if you need it\n");
    printf("Mouse Actions:\n");
    printf("- Move the mouse around to be able to see where you're going from player POV\n");

    glutInitWindowSize(w,h);
    glutInitWindowPosition(1000,100);
    window2 = glutCreateWindow("Maze 1st Person");
    glutCallBacks2();
    init2();

    glEnable(GL_TEXTURE_2D);
    glGenTextures(2, textures); //generate 2 texture IDs, store them in array "textures"
    texture();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glutMainLoop();				//starts the event loop

    return(0);					//return may not be necessary on all compilers
}
