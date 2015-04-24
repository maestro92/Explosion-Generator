#ifndef EG_DEFERRED_SHADING_H_
#define EG_DEFERRED_SHADING_H_

#include "EG_RenderTechnique.h"

class EG_DeferredShading : public EG_RenderTechnique
{
    public:
        glm::vec3 m_stencilFlag;
        GLuint m_stencilFlagUniformLocation;

        EG_DeferredShading();
        ~EG_DeferredShading();

        virtual void init(int nShaders);
        virtual void initStencilUniformLocation(int shaderId);
        virtual void setStencilFlag(glm::vec3 stencilFlag);
        void loadUniformLocations(pipeline& p, int RenderPassID);


};
#endif // EG_TECHNIQUE_DEFERRED_SHADING_H_
