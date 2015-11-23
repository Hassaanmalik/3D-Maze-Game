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
	int p1;
	int p2;
	int p3;
	int p4;
};

vector<Vertice>vertices;
vector<Vertice>vertices_texture;
vector<Vertice>vertices_normal;
vector<Face>faces;
vector<Face>faces_texture;
vector<Face>faces_normal;


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
				if(strcmp(lineHeader, "v") == 0){
					Vertice temp_vertice;
					fscanf(file, "%f %f %f\n", 
							&temp_vertice.x, &temp_vertice.y, &temp_vertice.z);

					vertices.push_back(temp_vertice);
				}else if(strcmp(lineHeader, "vn") == 0){
					Vertice temp_vertice_normal;
					fscanf(file, "%f %f %f\n", 
							&temp_vertice_normal.x, &temp_vertice_normal.y, &temp_vertice_normal.z);

					vertices_normal.push_back(temp_vertice_normal);
				}else if(strcmp(lineHeader, "vt") == 0){
					Vertice temp_vertice_texture;
					fscanf(file, "%f %f %f\n", 
							&temp_vertice_texture.x, &temp_vertice_texture.y);

					vertices_texture.push_back(temp_vertice_texture);
				}else if(strcmp(lineHeader, "f") == 0){
					Face temp_face, temp_face_texture, temp_face_normal;
					fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
							&temp_face.p1, &temp_face.p2, &temp_face.p3, &temp_face.p4,
							&temp_face_texture.p1, &temp_face_texture.p2, &temp_face_texture.p3, &temp_face_texture.p4,
							&temp_face_normal.p1, &temp_face_normal.p2, &temp_face_normal.p3, &temp_face_normal.p4);

					faces.push_back(temp_face);
					faces_texture.push_back(temp_face_texture);
					faces_normal.push_back(temp_face_normal);
				}
				/*else if(strcmp(lineHeader, "#") == 0){
				}else if(strcmp(lineHeader, "o") == 0){
				}else if(strcmp(lineHeader, "mtllib") == 0){
				}else if(strcmp(lineHeader, "usemtl") == 0){
				}else if(strcmp(lineHeader, "s") == 0){
				}*/
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