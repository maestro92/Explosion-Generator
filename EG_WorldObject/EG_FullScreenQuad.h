#ifndef EG_FULLSCREEN_QUAD
#define EG_FULLSCREEN_QUAD


#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include "EG_utility.h"
#include "EG_Shader.h"
#include "pipeline.h"
#include "EG_WorldObject.h"
#include "EG_QuadModelABS.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

using namespace std;

class EG_FullScreenQuad : public WorldObject
{
    public:
        EG_FullScreenQuad();
        ~EG_FullScreenQuad();

        void render(EG_Renderer* Renderer, int pass);
        void render(EG_Renderer* Renderer, int pass, GLuint FboTarget);
};


#endif

