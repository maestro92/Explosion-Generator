#ifndef EG_SKINNING_RENDERER_H
#define EG_SKINNING_RENDERER_H

#include "pipeline.h"
#include "define.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>


#include "EG_SceneRenderer.h"

class EG_SkinningRenderer : public EG_SceneRenderer
{

    public:
        static const unsigned int MAX_BONES = 100;

    EG_SkinningRenderer();
    ~EG_SkinningRenderer();

    void init(int nShaders);
//    void setBoneTransform(unsigned int pass, unsigned int index, const glm::mat4& transform);
    void setBoneTransforms(vector<glm::mat4>& transforms);

    void loadUniformLocations(pipeline& p, int RenderPassID);

  //  GLuint m_boneLocation[MAX_BONES];

    vector< vector<GLuint> > m_boneUniformLocations;

    void renewVector();
    vector<glm::mat4> m_boneTransforms;

};
#endif
