//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h> //added math libary to code to allow for use of sin,cos and sqrt

#include <vector>

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

using namespace std;

struct Vertice{
	float x;
	float y;
	float z;
};
struct Face{
	int point1;
	int point2;
	int point3;
	int point4;
};

vector<Vertice>vertices;
vector<Vertice>vertices_texture;
vector<Vertice>vertices_normal;
vector<Face>face;


LoadObj::LoadObj(char *objFile){
	this->objFile = objFile;
}

//Deconstructor
LoadObj::~LoadObj(){
}

void LoadObj::setVertices(){
	openFile();
	if(file){
		while(1){
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if(res == EOF){
				break;
			}else{
				if(strcmp(lineHeader, "#") == 0){
				}else if(strcmp(lineHeader, "v") == 0){
					//Vertice temp_vertices;
					//float x, y, z;
					//fscanf(file, "%f %f %f\n", &x, &y, &z);
					//temp_vertices.x = x;
					//temp_vertices.y = y;
					//temp_vertices.z = z;

					//printf("%f\n", x);
				}else if(strcmp(lineHeader, "vn") == 0){
				}else if(strcmp(lineHeader, "vt") == 0){
				}else if(strcmp(lineHeader, "f") == 0){
				}
			}
		}
		fclose(file);
	}
}

void LoadObj::openFile(){
  	char str[100];
	strcpy(str, "ObjFile/");
	strcat(str, objFile);
	strcat(str, ".obj");
	path = str;
	file = fopen(path, "r");
}