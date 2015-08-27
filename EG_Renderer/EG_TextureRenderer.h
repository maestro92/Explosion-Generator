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

        GLuint m_textureUnitUniLoc;
        GLuint m_pixelSizeUniLoc;
        int m_width;
        int m_height;
        int m_texUnit;


        EG_QuadModelABS o_fullScreenQuad;

        void init();
        void init(int nShaders);
        void setTextureUnit(unsigned int TextureUnit);
        void setPixelSize(unsigned int Width, unsigned int Height);
        virtual void loadUniformLocations(pipeline& p, int pass);
        void renderFullScreen(GLuint TextureID);
        void renderFullScreen(GLuint TextureID, GLuint FboTarget);


   //     void render(unsigned int TextureId, GLuint FboTarget, int Width, int Height, pipeline& m_pipeline);
   //     void renderFullScreen(unsigned int TextureId, GLuint FboTarget, int Width, int Height);
};


#endif // EG_TEXTURE_RENDERER_H_


