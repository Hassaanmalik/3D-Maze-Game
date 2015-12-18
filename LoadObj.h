#ifndef __LOADOBJ_H__
#define __LOADOBJ_H__

#include <vector>


class LoadObj{



    public:
        //Constructors
        LoadObj();
        //LoadObj(char *objFile);

        //Deconstructor
        ~LoadObj();

        //Public variables
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
        struct Color{
            float r;
            float g;
            float b;
        };

        //Public function

        /*LoadObj loadObj(char *fileName, char *filePath);*/
        void loadObj(char *fileName, char *filePath);
        /*LoadObj mtlForOBJ();*/
        void mtlForOBJ();
        /*LoadObj drawObj();*/
        void drawObj();


    private:
        //Private variables
        const char *fileName, *filePath;

        //**** Change all the following to arrays if loading multiple objects and material from one file ****/
        int numberOfFace;

        //Variables for Object Vertices
        std::vector<Vertice>vertices;
        std::vector<Vertice>vertices_texture;
        std::vector<Vertice>vertices_normal;

        //Variables for Object Faces
        std::vector<Face>faces;
        std::vector<Face>faces_texture;
        std::vector<Face>faces_normal;

        //Variables for Object Material
        float phongSpecular, refraction, dissolve;
        int illumination;
        Color ambient;
        Color diffuse;
        Color specularReflectivity;
        Color emission;
        //**** Change all the above to arrays if loading multiple objects and material from one file ****/


        //Private function

        /*LoadObj getNumberOfFaces();*/
        int getNumberOfFaces();


        //Functions to return Vertices
        /*LoadObj getVertices(int verticeNumber);*/
        Vertice getVertices(int verticeNumber);
        /*LoadObj getVerticesTexture();*/
        //Vertice getVerticesTexture();
        /*LoadObj getVerticesNormal(int verticeNumber);*/
        Vertice getVerticesNormal(int verticeNumber);


        //Functions to return Faces
        /*LoadObj getFaces(int faceNumber);*/
        Face getFaces(int faceNumber);
        /*LoadObj getFacesTexture();*/
        //Face getFacesTexture();
        /*LoadObj getFacesNormal(int faceNumber);*/
        Face getFacesNormal(int faceNumber);


        //Functions to return Material
        /*LoadObj getPhongSpecular();*/
        float getPhongSpecular();
        /*LoadObj getRefraction();*/
        //float getRefraction();
        /*LoadObj getIllumination();*/
        //int getIllumination();
        /*LoadObj getAmbient();*/
        float* getAmbient();
        /*LoadObj getDiffuse();*/
        float* getDiffuse();
        /*LoadObj getReflectivity();*/
        float* getReflectivity();
        /*LoadObj getEmission();*/
        float* getEmission();

        /*LoadObj openFile(char *filePath, char *fileType);*/
        char *openFile(char *fileType);

        /*LoadObj setObjVariables();*/
        void setObjVariables();

        /*LoadObj setObjMaterial();*/
        void setObjMaterial();

};

#endif
