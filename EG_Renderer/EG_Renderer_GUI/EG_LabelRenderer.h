#ifndef EG_RENDERER_LABEL_H_
#define EG_RENDERER_LABEL_H_

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "EG_Renderer.h"


class EG_LabelRenderer : public EG_Renderer
{
    private:
        GLuint m_CubemapTextureId;
        GLuint m_CubemapTextureUniLoc;

    public:
        EG_LabelRenderer ();
        ~EG_LabelRenderer ();
        void init(int nShaders);

        void setCameraPosition(glm::vec3 camPos);
        void setReflectionTextureId(GLuint id);
  //      void loadUniformLocations(pipeline& p, int RenderPassID);
};


#endif // EG_RENDER_TECHINQUE_RENDER_TEXTURE_H_


