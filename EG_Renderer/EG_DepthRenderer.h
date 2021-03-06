#ifndef EG_RENDER_TECHNIQUE_RENDER_DEPTH_TO_TEXTURE_H_
#define EG_RENDER_TECHNIQUE_RENDER_DEPTH_TO_TEXTURE_H_

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "EG_Renderer.h"



/// LightPOV    render1
/// CameraPOV   render2
class EG_DepthRenderer : public EG_Renderer
{
    public:
        EG_DepthRenderer();
        ~EG_DepthRenderer();

        void init(int nShaders);
};
#endif
