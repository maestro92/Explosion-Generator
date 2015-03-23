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

#include "EG_RenderTechnique.h"



/// LightPOV    render1
/// CameraPOV   render2
class EG_RenderTechnique_RenderDepthToTexture : public EG_RenderTechnique
{
    public:

        /// first pass for depth in Light's point of View
        /// Second pass in Camera's point of View
        EG_RenderTechnique_RenderDepthToTexture();
        ~EG_RenderTechnique_RenderDepthToTexture();

        void init(int Shader_Num);

};
#endif
