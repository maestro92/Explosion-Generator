#ifndef EG_TECHNIQUE_SKINNING_H
#define EG_TECHNIQUE_SKINNING_H

#include "pipeline.h"
#include "define.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>


#include "EG_Technique_Shadow_Render.h"

class EG_RenderTechnique_Skinning : public Technique_Shadow_Render
{
    public:
        static const unsigned int MAX_BONES = 100;

    EG_RenderTechnique_Skinning();
    ~EG_RenderTechnique_Skinning();

    void init(int nShaders);
    void setBoneTransform(unsigned int index, const glm::mat4& transform);

    GLuint m_boneLocation[MAX_BONES];



};
#endif
