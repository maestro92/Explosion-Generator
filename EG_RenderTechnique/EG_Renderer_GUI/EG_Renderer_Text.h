#ifndef EG_RENDERER_TEXT_H_
#define EG_RENDERER_TEXT_H_

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


class EG_Renderer_Text : public EG_RenderTechnique
{
    public:
        EG_Renderer_Text();
        ~EG_Renderer_Text();
        void init(int nShaders);


//        void loadUniformLocations(pipeline& p, int RenderPassID);
        void setTextureUnit(unsigned int TextureUnit);

    private:
        GLuint m_textureUnitUniformLocation;
};


#endif // EG_RENDER_TECHINQUE_RENDER_TEXTURE_H_

