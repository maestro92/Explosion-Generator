#ifndef EG_RENDERER_BUTTON_H_
#define EG_RENDERER_BUTTON_H_

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "EG_QuadModel.h"
#include "EG_RenderTechnique.h"


class EG_Renderer_Button : public EG_RenderTechnique
{
    public:
        EG_Renderer_Button();
        ~EG_Renderer_Button();
        void init(int nShaders);

        void setTextureUnit(unsigned int TextureUnit);
        void setColor(glm::vec3 color);

    private:
        GLuint m_colorUniformLocation;
        GLuint m_textureUnitUniformLocation;
};


#endif // EG_RENDERER_BUTTON_H_


