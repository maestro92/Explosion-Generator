#ifndef EG_TEXT_RENDERER_H_
#define EG_TEXT_RENDERER_H_

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "EG_Renderer.h"


class EG_TextRenderer : public EG_Renderer
{
    public:
        EG_TextRenderer();
        ~EG_TextRenderer();
        void init(int nShaders);


//        void loadUniformLocations(pipeline& p, int RenderPassID);
        void setTextureUnit(unsigned int TextureUnit);

    private:
        GLuint m_textureUnitUniformLocation;
};


#endif // EG_RENDER_TECHINQUE_RENDER_TEXTURE_H_


