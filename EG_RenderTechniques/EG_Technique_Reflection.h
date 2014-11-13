#ifndef EG_REFLECTION_H
#define EG_REFLECTION_H


#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "EG_Technique.h"



/// LightPOV    render1
/// CameraPOV   render2
class Technique_Reflection : public Technique
{
    public:
        GLuint CameraPosition_UniformLocation;
        GLuint CubeMap_UniformLocation;
        Technique_Reflection();
        ~Technique_Reflection();
        void init(int Shader_Num);

};

#endif
