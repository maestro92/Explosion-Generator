#ifndef EG_DEFERRED_SHADING_H_
#define EG_DEFERRED_SHADING_H_

#include "EG_RenderTechnique.h"

class EG_DeferredShading : public EG_RenderTechnique
{
    public:

        GLuint m_colorTextureUnitLocation_;

        GLuint l_modelViewProjectionMatrix_UniLoc_;
        GLuint lightPosition_ModelViewMatrix_UniLoc_;
        GLuint positionMap_UniLoc_;
        GLuint screenSize_UniLoc_;

        GLuint shadowMap_UniLoc_;
        GLuint colorMap_UniLoc_;
        GLuint normalMap_UniLoc_;

/// stack size linker option
/// Project->Build Options->Linker Settings->Other Linker Options
/// -Wl,--stack,52800000


        GLuint lightPosition_ObjectSpace_UniLoc_;

    public:
        EG_DeferredShading();
        ~EG_DeferredShading();
        void init(int shaderCount);
        void SetColorTextureUnit(unsigned int TextureUnit);
        void loadUniformLocations(pipeline& p, int RenderPassID);
/*
    //    GLuint cameraPosition_WorldSpace_UniLoc;
        glm::mat4 beautiful2;
        glm::mat4 beautiful3;
        glm::mat4 beautiful4;
        glm::mat4 beautiful5;
        glm::mat4 beautiful6;
        glm::mat4 beautiful17;

        glm::mat4 beautiful21;
        glm::mat4 beautiful31;
        glm::mat4 beautiful41;
        glm::mat4 beautiful51;
        glm::mat4 beautiful112;
        glm::mat4 beautiful1132;

        glm::mat4 beautiful22222;
        glm::mat4 beautiful333333;
        glm::mat4 beautiful44444;
        glm::mat4 beautiful555555;
*/
/*
        GLuint LightMVPmatrix_UniLoc;
        GLuint shadowMap_UniLoc;
        GLuint LightPosition_MVmatrix_UniLoc;
        GLuint LightPosition_UniLoc;
        GLuint CameraPosition_UniLoc;
*/

    private:

    protected:
};
#endif // EG_TECHNIQUE_DEFERRED_SHADING_H_
