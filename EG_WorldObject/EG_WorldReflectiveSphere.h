#ifndef WORLD_REFLECTIVE_SPHERE_H_
#define WORLD_REFLECTIVE_SPHERE_H_

#include "EG_WorldSphere.h"
#include "pipeline.h"
#include "sceneLoader.h"
#include "EG_DeferredShadingReflection.h"


class EG_WorldReflectiveSphere : public WorldSphere
{
    public:
        // constructor
        EG_WorldReflectiveSphere();

        // Destructor
		virtual ~EG_WorldReflectiveSphere();

        void render(pipeline &m_pipeline,
                    EG_DeferredShadingReflection& RenderTechnique,
                    int RenderPassID,
                    unsigned int textureId,
                    meshLoader* model,
                    glm::vec3 EyePoint);
};


#endif
