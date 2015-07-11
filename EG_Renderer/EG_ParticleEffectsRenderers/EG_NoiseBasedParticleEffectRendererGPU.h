#ifndef EG_NOISE_BASED_PARTICLE_EFFECT_RENDERER_GPU_H_
#define EG_NOISE_BASED_PARTICLE_EFFECT_RENDERER_GPU_H_

#include "EG_Renderer.h"

using namespace std;

class EG_NoiseBasedParticleEffectRendererGPU : public EG_Renderer
{
    public:
        EG_NoiseBasedParticleEffectRendererGPU();
        ~EG_NoiseBasedParticleEffectRendererGPU();
        void init(int nShaders);
        void init();

        /// Advection Shader
        FloatDataPair m_timePair;
        Vec3DataPair m_sizeDataPair;
        Vec3DataPair m_extentDataPair;

        void setTime(float time);
        void setSize(glm::vec3 size);
        void setExtent(glm::vec3 extent);



        /// Particle Shader
        Vec4DataPair m_colorPair;
        FloatDataPair m_fadeRatePair;
        IntDataPair m_depthTexturePair;
        IntDataPair m_spriteTexturePair;
        FloatDataPair m_timePair2;
        FloatDataPair m_pointSizePair;
        Vec2DataPair m_inverseSizePair;

        Mat4DataPair m_MVMatrixDataPair;
        Mat4DataPair m_MVPMatrixDataPair;
        Vec3DataPair m_cameraPositionDataPair;
        Vec3DataPair m_cameraViewDirDataPair;

        void setColor(glm::vec4 color);
        void setFadeRate(float value);
        void setDepthTextureUnit(int unit);
        void setSpriteTextureUnit(int unit);
        void setTime2(float time);
        void setPointSize(float size);
        void setInverseSize(glm::vec2 size);

        void setModelViewProjectionMatrix(glm::mat4 mvpMat);
        void setModelViewMatrix(glm::mat4 mvMat);
        void setCameraPosition(glm::vec3 camPos);
        void setCameraViewDir(glm::vec3 dir);






        /// Composite Shader
        Vec2DataPair m_inverseSizePair3;
        IntDataPair m_backgroundTexturePair;
        IntDataPair m_particlesTexturePair;


        void setInverseSize3(glm::vec2 size);
        void setBackgroundTextureUnit(int unit);
        void setParticlesTextureUnit(int unit);

        virtual void loadUniformLocations(pipeline& p, int pass);




};


#endif // EG_NOISE_BASED_PARTICLE_EFFECT_RENDERER_H_
