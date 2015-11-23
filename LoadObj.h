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

        //Public function
        /*LoadObj setVertices();*/
        void setVertices();

    private:
        //Private variables
        const char *objFile;
        char *path;
        FILE *file;
        
        /*vector<Vertice>vertices;
        vector<Vertice>vertices_texture;
        vector<Vertice>vertices_normal;
        vector<Face>face;*/

        //Private function
        /*LoadObj openFile();*/
        void openFile();

};

#endif
