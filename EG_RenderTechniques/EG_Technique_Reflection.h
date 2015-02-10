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

#include "EG_RenderTechnique.h"





/// LightPOV    render1
/// CameraPOV   render2
class Technique_Reflection : public EG_RenderTechnique
{
    private:
        glm::vec3   m_cameraPosition;
        GLuint      m_reflectionTextureId;

    public:
        GLuint      m_cameraPositionUniLoc;
        GLuint      m_reflectionTextureIdUniLoc;
        int         m_reflectionTextureUnit;

        Technique_Reflection();
        ~Technique_Reflection();
        void init(int Shader_Num);

        void setCameraPosition(glm::vec3 camPos);
        void setReflectionTextureId(GLuint id);
        void loadUniformLocations(pipeline& p, int RenderPassID);

};

#endif
