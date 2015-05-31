#ifndef MESH_H_
#define MESH_H_

#include <iostream>
#include "meshData.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"

//#include "gl/gl.h"
//#include "gl/glu.h"
#include <vector>
#include <string>

using namespace std;


class mesh{
	vector<vertexData> data;        // vertex data
	vector<textureData> textures;   // texture data
	vector<unsigned int> indices;   //
	unsigned int VBO;   // vertex buffer object
	unsigned int IND;   // index buffer object
	public:
		mesh(vector<vertexData>* vd,vector<unsigned int>* id,vector<textureData>* td=NULL);
		~mesh();
		void draw();
        void draw2();
		void draw(unsigned int programId);
        void drawSpecial();
        void drawLines();
};
#endif
