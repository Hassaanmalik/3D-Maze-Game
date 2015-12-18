//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>

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


LoadObj::LoadObj(){
}

//Deconstructor
LoadObj::~LoadObj(){
}

//**************************//
//**** Public Functions ****//
//**************************//

void LoadObj::loadObj(char *fileName, char *filePath){
	this->fileName = fileName;
	this->filePath = filePath;
	setObjVariables();
}

/*LoadObj mtlForOBJ();*/
void LoadObj::mtlForOBJ(){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbient());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffuse());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getReflectivity());
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, getPhongSpecular());
}

/*LoadObj drawObj();*/
void LoadObj::drawObj(){
	int numberOfFaces = getNumberOfFaces();

	Face face, face_normal;
	Vertice vertices, vertices_normal;
	//glColor4f(0.3,0.3,0.3,0);
	for(int i = 1; i < numberOfFaces; i++){
		face = getFaces(i);
		face_normal = getFacesNormal(i);

		glBegin(GL_POLYGON);
			vertices = getVertices(face.p1);
			vertices_normal = getVerticesNormal(face_normal.p1);
			glNormal3f(vertices_normal.x, vertices_normal.y, vertices_normal.z);
			glVertex3f(vertices.x, vertices.y, vertices.z);

			vertices = getVertices(face.p2);
			vertices_normal = getVerticesNormal(face_normal.p4);
			glNormal3f(vertices_normal.x, vertices_normal.y, vertices_normal.z);
			glVertex3f(vertices.x, vertices.y, vertices.z);

			vertices = getVertices(face.p3);
			vertices_normal = getVerticesNormal(face_normal.p4);
			glNormal3f(vertices_normal.x, vertices_normal.y, vertices_normal.z);
			glVertex3f(vertices.x, vertices.y, vertices.z);

			vertices = getVertices(face.p4);
			vertices_normal = getVerticesNormal(face_normal.p4);
			glNormal3f(vertices_normal.x, vertices_normal.y, vertices_normal.z);
			glVertex3f(vertices.x, vertices.y, vertices.z);
		glEnd();
	}
}


//***************************//
//**** Private Functions ****//
//***************************//

/*LoadObj getNumberOfFaces();*/
int LoadObj::getNumberOfFaces(){
	return numberOfFace;
}

/*LoadObj getVertices(int verticeNumber);*/
LoadObj::Vertice LoadObj::getVertices(int verticeNumber){
	return vertices.at(verticeNumber-1);
}
/*LoadObj getVerticesNormal(int verticeNumber);*/
LoadObj::Vertice LoadObj::getVerticesNormal(int verticeNumber){
	return vertices_normal.at(verticeNumber-1);
}

/*LoadObj getFaces(int faceNumber);*/
LoadObj::Face LoadObj::getFaces(int faceNumber){
	return faces.at(faceNumber-1);
}
/*LoadObj getFacesNormal(int faceNumber);*/
LoadObj::Face LoadObj::getFacesNormal(int faceNumber){
	return faces_normal.at(faceNumber-1);
}


/*LoadObj getPhongSpecular();*/
float LoadObj::getPhongSpecular(){
	return phongSpecular;
}
/*LoadObj getAmbient();*/
float* LoadObj::getAmbient(){
	float localAmbient[4] = {ambient.r, ambient.g, ambient.b, dissolve};
    printf("ambient: %f, %f, %f, %f", localAmbient[0], localAmbient[1], localAmbient[2], localAmbient[3]);
	return localAmbient;
}
/*LoadObj getDiffuse();*/
float* LoadObj::getDiffuse(){
	float localDiffuse[4] = {diffuse.r, diffuse.g, diffuse.b, dissolve};
    printf("diffuse: %f, %f, %f, %f", localDiffuse[0], localDiffuse[1], localDiffuse[2], localDiffuse[3]);
	return localDiffuse;
}
/*LoadObj getReflectivity();*/
float* LoadObj::getReflectivity(){
	float localReflectivity[4] = {specularReflectivity.r, specularReflectivity.g, specularReflectivity.b, dissolve};
    printf("reflectivity: %f, %f, %f, %f", localReflectivity[0], localReflectivity[1], localReflectivity[2], localReflectivity[3]);
	return localReflectivity;
}
/*LoadObj getEmission();*/
float* LoadObj::getEmission(){
	float localEmission[4] = {emission.r, emission.g, emission.b, dissolve};
    printf("emission: %f, %f, %f, %f", localEmission[0], localEmission[1], localEmission[2], localEmission[3]);
	return localEmission;
}

char *LoadObj::openFile(char *fileType){
  	char *str = (char *)malloc(strlen(filePath)+strlen(fileName)+strlen(fileType)+1);
	strcpy(str, filePath);
	strcat(str, fileName);
	strcat(str, fileType);
	return str;
}

void LoadObj::setObjVariables(){
	char *fileType = ".obj";
	char *str = openFile(fileType);
	FILE *objfile = fopen(str, "r");

	numberOfFace = 0;
	if(objfile){
		while(1){
			char lineHeader[32];
			int res = fscanf(objfile, "%s", lineHeader);
			if(res == EOF){
				break;
			}else{
				if(strcmp(lineHeader, "#") == 0){
				}else if(strcmp(lineHeader, "mtllib") == 0){
					setObjMaterial();
				}/*else if(strcmp(lineHeader, "o") == 0){
					//To Use multiple materials from one file
						//I need to make all the variables into array
				}*/else if(strcmp(lineHeader, "v") == 0){//Sets the vertices for the faces
					Vertice temp_vertice;
					fscanf(objfile, "%f %f %f\n",
							&temp_vertice.x, &temp_vertice.y, &temp_vertice.z);

					vertices.push_back(temp_vertice);
				}else if(strcmp(lineHeader, "vn") == 0){//Sets the vertices for face normals
					Vertice temp_vertice_normal;
					fscanf(objfile, "%f %f %f\n",
							&temp_vertice_normal.x, &temp_vertice_normal.y, &temp_vertice_normal.z);

					vertices_normal.push_back(temp_vertice_normal);
				}else if(strcmp(lineHeader, "vt") == 0){//Sets the vertices for face textures
					Vertice temp_vertice_texture;
					fscanf(objfile, "%f %f %f\n",
							&temp_vertice_texture.x, &temp_vertice_texture.y);

					vertices_texture.push_back(temp_vertice_texture);
				}else if(strcmp(lineHeader, "f") == 0){//Sets the face, face textures and face normals
					Face temp_face, temp_face_texture, temp_face_normal;
					fscanf(objfile, "%i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i\n",
							&temp_face.p1, &temp_face_texture.p1, &temp_face_normal.p1,
							&temp_face.p2, &temp_face_texture.p2, &temp_face_normal.p2,
							&temp_face.p3, &temp_face_texture.p3, &temp_face_normal.p3,
							&temp_face.p4, &temp_face_texture.p4, &temp_face_normal.p4);
					faces.push_back(temp_face);
					faces_texture.push_back(temp_face_texture);
					faces_normal.push_back(temp_face_normal);
					numberOfFace++;
				}/*else if(strcmp(lineHeader, "usemtl") == 0){
					//Need to implement newmtl from setObjMaterial
				}else if(strcmp(lineHeader, "s") == 0){
					//Find out what this does and how to use this
				}*/
			}
		}
		fclose(objfile);
	}
}

/*LoadObj setObjMaterial();*/
void LoadObj::setObjMaterial(){
	char *str = openFile(".mtl");
	FILE *mtlfile = fopen(str, "r");

	if(mtlfile){
		while(1){
			char lineHeader[32];
			int res = fscanf(mtlfile, "%s", lineHeader);
			//printf("%s\n", lineHeader);
			if(res == EOF){
				break;
			}else{

				if(strcmp(lineHeader, "#") == 0){
				}/*else if(strcmp(lineHeader, "newmtl") == 0){
					//To Use multiple materials from one file
						//I need to make all the variables into array
				}*/else if(strcmp(lineHeader, "Ns") == 0){//Sets the phong BRDF exponent for the reflective component, value between 1 and 1000 is required.
					fscanf(mtlfile, "%f\n", &phongSpecular);
				}else if(strcmp(lineHeader, "Ka") == 0){//Sets the ambient constant color of the material
					fscanf(mtlfile, "%f %f %f\n", &ambient.r, &ambient.g, &ambient.b);
				}else if(strcmp(lineHeader, "Kd") == 0){//Sets the diffuse constant color of the material
					fscanf(mtlfile, "%f %f %f\n", &diffuse.r, &diffuse.g, &diffuse.b);
				}else if(strcmp(lineHeader, "Ks") == 0){//Sets the specular reflectivity constant color of the material
					fscanf(mtlfile, "%f %f %f\n", &specularReflectivity.r, &specularReflectivity.g, &specularReflectivity.b);
				}else if(strcmp(lineHeader, "Ke") == 0){//Sets the emission constant color of the material
					fscanf(mtlfile, "%f %f %f\n", &emission.r, &emission.g, &emission.b);
				}else if(strcmp(lineHeader, "Ni") == 0){//Sets the refraction index, values greater then 1. A value of 1 will cause no refraction
					fscanf(mtlfile, "%f\n", &refraction);
				}else if(strcmp(lineHeader, "d") == 0){//Sets the dissolve factor, values between 0 and 1. 0 is completely transparent, 1 is opaque
					fscanf(mtlfile, "%f\n", &dissolve);
				}else if(strcmp(lineHeader, "illum") == 0){//Sets the illumination, values (0, 1, 2). 0 desable light, 1 ambient and diffuse (specular reflectivity is black), 2 for full light
					fscanf(mtlfile, "%i\n", &illumination);
				}/*else if(strcmp(lineHeader, "sharpness") == 0){
					//Find out what this does and how to use this
				}else if(strcmp(lineHeader, "map_Kd") == 0){
					//Find out what this does and how to use this
				}else if(strcmp(lineHeader, "map_Ks") == 0){
					//Find out what this does and how to use this
				}else if(strcmp(lineHeader, "map_Ka") == 0){
					//Find out what this does and how to use this
				}else if(strcmp(lineHeader, "map_Bump") == 0){
					//Find out what this does and how to use this
				}else if(strcmp(lineHeader, "map_d") == 0){
					//Find out what this does and how to use this
				}else if(strcmp(lineHeader, "refl") == 0){
					//Find out what this does and how to use this
				}*/
			}
		}
		fclose(mtlfile);
	}
}
