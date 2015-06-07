#ifndef EG_BILLBOARD_RENDERER_H_
#define EG_BILLBOARD_RENDERER_H_

#include "EG_Renderer.h"


class EG_BillboardRenderer: public EG_Renderer
{
    public:
        EG_BillboardRenderer();
        ~EG_BillboardRenderer();

/*
        GLuint m_modelViewProjectionMatrixUniLoc;
        GLuint m_cameraPositionUniLoc;
        GLuint m_cameraViewDirectionUniLoc;
//        GLuint m_textureUniLoc;

        glm::mat4 m_modelViewProjectionMatrix;
        glm::vec3 m_cameraPosition;
        glm::vec3 m_cameraViewDir;
*/

        Mat4DataPair m_MVPMatrixDataPair;
        Vec3DataPair m_cameraPositionDataPair;
        Vec3DataPair m_cameraViewDirDataPair;

        void init(int nShaders);
        void setModelViewProjectionMatrix(glm::mat4 vpMat);
        void setCameraPosition(glm::vec3 camPos);
        void setCameraViewDir(glm::vec3 dir);

        virtual void loadUniformLocations(pipeline& p, int RenderPassID);
};

#endif // EG_BILLBOARD_RENDERER_H_
