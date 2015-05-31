#ifndef EG_TEXTURE_RENDERER_H_
#define EG_TEXTURE_RENDERER_H_

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "EG_QuadModelABS.h"
#include "EG_Renderer.h"


class EG_TextureRenderer : public EG_Renderer
{
    public:
        EG_TextureRenderer();
        ~EG_TextureRenderer();

        GLuint m_textureUnitUniformLocation;
        GLuint m_pixelSizeUniformLocation;

        void init(int nShaders);

//        EG_QuadModel m_quad1;
   //     EG_QuadModelABS m_quad;

        void setTextureUnit(unsigned int TextureUnit);
        void setPixelSize(unsigned int Width, unsigned int Height);

        void render(unsigned int TextureId, GLuint FboTarget, int Width, int Height, pipeline& m_pipeline);
        void renderFullScreen(unsigned int TextureId, GLuint FboTarget, int Width, int Height);
};


#endif // EG_TEXTURE_RENDERER_H_


