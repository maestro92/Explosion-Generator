#ifndef EG_INSTANCED_RENDERER
#define EG_INSTANCED_RENDERER


#include "EG_Technique_Shadow_Render.h"

class EG_InstancedRenderer : public Technique_Shadow_Render
{
    public:
        EG_InstancedRenderer();
        ~EG_InstancedRenderer();
        void init(int nShaders);
};



#endif // EG_INSTANCED_RENDERER
