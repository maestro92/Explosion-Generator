#ifndef EG_UTILITY_H
#define EG_UTILITY_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>


#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

using namespace std;

class EG_utility
{
    public:
        EG_utility();
        ~EG_utility();

        /// texture related
        SDL_Surface* Load_CubeMapTexture(string filename);
        unsigned int Create_Texture(int w, int h, bool isDepth);
        unsigned int Create_CubemapTexture();
        unsigned int Create_CubemapTexture(string* filenames);



};


#endif
