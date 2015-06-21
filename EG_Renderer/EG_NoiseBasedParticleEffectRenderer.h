#ifndef EG_NOISE_BASED_PARTICLE_EFFECT_RENDERER_H_
#define EG_NOISE_BASED_PARTICLE_EFFECT_RENDERER_H_


#include "EG_Renderer.h"

using namespace std;

class EG_NoiseBasedParticleEffectRenderer : public EG_Renderer
{
    public:
        EG_NoiseBasedParticleEffectRenderer();
        ~EG_NoiseBasedParticleEffectRenderer();
        void init(int nShaders);

        /// Blit Shader
        FloatDataPair m_depthPair;
        FloatDataPair m_scrollOffsetTimePair;

        void setDepth(float depth);
        void setScrollOffsetTime(float value);


        /// Particle Shader
        Vec4DataPair m_colorPair;
        FloatDataPair m_fadeRatePair;
        IntDataPair m_depthTexturePair;
        IntDataPair m_spriteTexturePair;
        FloatDataPair m_timePair;
        FloatDataPair m_pointSizePair;
        Vec2DataPair m_inverseSizePair;

        Mat4DataPair m_MVPMatrixDataPair;
        Vec3DataPair m_cameraPositionDataPair;
        Vec3DataPair m_cameraViewDirDataPair;

        void setColor(glm::vec4 color);
        void setFadeRate(float value);
        void setDepthTextureUnit(int unit);
        void setSpriteTextureUnit(int unit);
        void setTime(float time);
        void setPointSize(float size);
        void setInverseSize(glm::vec2 size);

        void setModelViewProjectionMatrix(glm::mat4 vpMat);
        void setCameraPosition(glm::vec3 camPos);
        void setCameraViewDir(glm::vec3 dir);



        /// Composite Shader
        FloatDataPair m_depthPair3;
        IntDataPair m_backgroundTexturePair;
        IntDataPair m_particlesTexturePair;
        Vec2DataPair m_inverseSizePair3;

        void setDepth3(float depth);
        void setInverseSize3(glm::vec2 size);
        void setBackgroundTextureUnit(int unit);
        void setParticlesTextureUnit(int unit);

        virtual void loadUniformLocations(pipeline& p, int pass);
};


#endif // EG_NOISE_BASED_PARTICLE_EFFECT_RENDERER_H_
