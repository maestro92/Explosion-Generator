#ifndef EG_RENDER_TECHINQUE_RENDER_TEXTURE_H_
#define EG_RENDER_TECHINQUE_RENDER_TEXTURE_H_

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "EG_Quad.h"
#include "EG_RenderTechnique.h"


class EG_RenderTechnique_RenderTexture : public EG_RenderTechnique
{
    public:
        GLuint m_textureUnitUniformLocation;
        GLuint m_pixelSizeUniformLocation;
        EG_RenderTechnique_RenderTexture();
        ~EG_RenderTechnique_RenderTexture();
        void init(int Shader_Num);

        EG_Quad m_quad;

        void setTextureUnit(unsigned int TextureUnit);
        void setPixelSize(unsigned int Width, unsigned int Height);

        void render(unsigned int TextureId, GLuint FboTarget, int Width, int Height, pipeline& m_pipeline);
        void renderFullScreen(unsigned int TextureId, GLuint FboTarget, int Width, int Height);
};


#endif // EG_RENDER_TECHINQUE_RENDER_TEXTURE_H_


