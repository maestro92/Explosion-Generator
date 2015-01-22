#ifndef SCENELOADER_H
#define SCENELOADER_H
#include <vector>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL/SDL_image.h>
#include "mesh.h"

class meshLoader{
	std::vector<mesh*> meshes;
	void recursiveProcess(aiNode* node,const aiScene* scene);
	void processMesh(aiMesh* mesh,const aiScene* scene);
	unsigned int loadTexture(const char* filename);
	public:

	    string path;

        meshLoader(string filename);
		// meshLoader(const char* filename);
		~meshLoader();
        void draw();
		void draw(unsigned int programId);

        void drawLines();

		std::vector<mesh*>& getMeshes();
};

#endif

