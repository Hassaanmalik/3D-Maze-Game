#ifndef __LOADOBJ_H__
#define __LOADOBJ_H__

#include <stdio.h>
#include <vector>


class LoadObj{

    

    public:
        //Constructors
        LoadObj(char *objFile);

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

        //Public function
        /*LoadObj setVertices();*/
        void setVertices();
        /*LoadObj getNumberOfFaces();*/
        int getNumberOfFaces();

        /*LoadObj getVertices(int verticeNumber);*/
        Vertice getVertices(int verticeNumber);
        /*LoadObj getVerticesTexture();*/
        //void getVerticesTexture();
        /*LoadObj getVerticesNormal();*/
        //void getVerticesNormal();

        /*LoadObj getFaces(int faceNumber);*/
        Face getFaces(int faceNumber);
        /*LoadObj getFacesTexture();*/
        //void getFacesTexture();
        /*LoadObj getFacesNormal();*/
        //void getFacesNormal();

    private:
        //Private variables
        const char *objFile;
        char *path;
        FILE *file;
        int numberOfFace;
        
        std::vector<Vertice>vertices;
        std::vector<Vertice>vertices_texture;
        std::vector<Vertice>vertices_normal;
        std::vector<Face>faces;
        std::vector<Face>faces_texture;
        std::vector<Face>faces_normal;

        //Private function
        /*LoadObj openFile();*/
        void openFile();

};

#endif
