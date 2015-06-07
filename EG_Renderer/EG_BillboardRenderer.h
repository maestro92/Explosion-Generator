#ifndef EG_BILLBOARD_RENDERER_H_
#define EG_BILLBOARD_RENDERER_H_

#include "EG_Renderer.h"




/*
/// http://stackoverflow.com/questions/12009314/how-to-create-an-array-of-templated-class-objects
/// http://stackoverflow.com/questions/13345595/c-store-same-classes-with-different-templates-in-array
struct AbsTempDataPair
{
    virtual ~AbsTempDataPair() = 0;
};

template <typename T>
struct TempDataPair
{
    GLuint uniLoc;
    T value;
    virtual ~TempDataPair(){};
};
*/


class EG_BillboardRenderer: public EG_Renderer
{
    public:
        EG_BillboardRenderer();
        ~EG_BillboardRenderer();

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
