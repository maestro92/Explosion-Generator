#ifndef EG_MODELABS_H
#define	EG_MODELABS_H


#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "define.h"
#include "EG_Utility.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "texture.h"

using namespace std;

#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_GL_VALUE -1

#define POS_VB 0
#define NORMAL_VB 1
#define TANGENT_VB 2
#define COLOR_VB 3
#define UV_VB 4
#define INDEX_BUFFER 5
#define BONE_VB 6

#define POS_UNI_LOC 0
#define NORMAL_UNI_LOC 1
#define TANGENT_UNI_LOC 2
#define COLOR_UNI_LOC 3
#define UV_UNI_LOC 4
#define BONE_ID_UNI_LOC 5
#define BONE_WEIGHT_UNI_LOC 6


/*
http://gamedev.stackexchange.com/questions/19560/what-is-the-best-way-to-store-meshes-or-3d-models-in-a-class

This basically means that the models are stored in GPU memory instead of CPU memory, minimizing GPU to CPU transfers and greatly improving rendering performance.
*/

#include "EG_Mesh.h"

class EG_ModelABS
{
    enum
    {
        POSITION = 0,
        NORMAL,
        COLOR,
        UV,
    };

    public:
        EG_ModelABS();
        ~EG_ModelABS();

        void addMesh(EG_Mesh m);
        virtual void init();
        virtual void render();

        void clear();
        GLuint m_modelGeometry;
    protected:
        vector<EG_Mesh> m_meshes;

};


#endif // MESH37_H
