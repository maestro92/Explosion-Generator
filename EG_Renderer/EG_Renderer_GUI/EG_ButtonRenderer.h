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
#include "EG_Renderer.h"


class EG_ButtonRenderer : public EG_Renderer
{
    public:
        EG_ButtonRenderer();
        ~EG_ButtonRenderer();
        void init(int nShaders);

        void setTextureUnit(unsigned int TextureUnit);
        void setColor(glm::vec3 color);

    private:
        GLuint m_colorUniformLocation;
        GLuint m_textureUnitUniformLocation;
};


#endif // EG_RENDERER_BUTTON_H_


