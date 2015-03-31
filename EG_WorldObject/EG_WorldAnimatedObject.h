#ifndef EG_WORLD_ANIMATED_OBJECT_H
#define EG_WORLD_ANIMATED_OBJECT_H

#include "EG_WorldObject.h"


class EG_WorldAnimatedObject : public WorldObject
{
    public:
        vector<glm::mat4> m_boneTransforms;

        EG_WorldAnimatedObject();
        ~EG_WorldAnimatedObject();
/*
        void renderSingle(  pipeline& m_pipeline,
                            EG_RenderTechnique* RenderTechnique,
                            int RenderPassID,
                            EG_Model* model);
*/
};
#endif // EG_WORLD_ANIMATED_OBJECT_H
