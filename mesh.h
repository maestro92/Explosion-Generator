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
	vector<vertexData> data;
	vector<textureData> textures;
	vector<unsigned int> indices;
	unsigned int VBO;
	unsigned int IND;
	public:
		mesh(vector<vertexData>* vd,vector<unsigned int>* id,vector<textureData>* td=NULL);
		~mesh();
		void draw(unsigned int programId);
};
#endif
