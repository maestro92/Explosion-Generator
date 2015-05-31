#ifndef EG_INSTANCED_RENDERER
#define EG_INSTANCED_RENDERER


#include "EG_SceneRenderer.h"

class EG_InstancedRenderer : public EG_SceneRenderer
{
    public:
        EG_InstancedRenderer();
        ~EG_InstancedRenderer();
        void init(int nShaders);
};



#endif // EG_INSTANCED_RENDERER
