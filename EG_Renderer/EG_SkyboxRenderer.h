#ifndef EG_SKYBOX_RENDERER_H_
#define EG_SKYBOX_RENDERER_H_

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "EG_Renderer.h"


class EG_SkyboxRenderer : public EG_Renderer
{
    private:
        int m_cubemapTextureUnit;
        GLuint m_cubemapTextureUniLoc;

        // GLuint m_cubemapTextureID;


    public:
        EG_SkyboxRenderer();
        ~EG_SkyboxRenderer();
        virtual void init(int nShaders);

        void setCubeMapTextureUnit(int unit);
        virtual void loadUniformLocations(pipeline& p, int RenderPassID);
};


#endif // EG_RENDER_TECHINQUE_RENDER_TEXTURE_H_


