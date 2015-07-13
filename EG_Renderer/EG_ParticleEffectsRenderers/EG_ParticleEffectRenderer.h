#ifndef EG_PARTICLE_EFFECT_RENDERER_H_
#define EG_PARTICLE_EFFECT_RENDERER_H_

#include "EG_Renderer.h"

class EG_ParticleEffectRenderer : public EG_Renderer
{
    public:
        EG_ParticleEffectRenderer();
        ~EG_ParticleEffectRenderer();
        void init(int nShaders);
        virtual void loadUniformLocations(pipeline& p, int pass);
};
#endif // EG_FIRE_EFFECT_RENDERER_H_
