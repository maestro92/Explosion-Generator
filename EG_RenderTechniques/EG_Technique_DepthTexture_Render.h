#ifndef EG_TECHNIQUE_DEPTH_TEXTURE_RENDER_H
#define EG_TECHNIQUE_DEPTH_TEXTURE_RENDER_H

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "EG_Technique.h"



/// LightPOV    render1
/// CameraPOV   render2
class Technique_DepthTexture_Render : public Technique
{
    public:

        /// first pass for depth in Light's point of View
        /// Second pass in Camera's point of View

        Technique_DepthTexture_Render();
        ~Technique_DepthTexture_Render();

        void init(int Shader_Num);


        // Shaders
        shader* Depth_CameraRender;
};
#endif
