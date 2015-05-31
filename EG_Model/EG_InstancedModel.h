#ifndef EG_INSTANCED_MODEL_H_
#define EG_INSTANCED_MODEL_H_

#include "EG_Model.h"

using namespace std;

/*
#define POS_VB 0
#define NORMAL_VB 1
#define TANGENT_VB 2
#define COLOR_VB 3
#define UV_VB 4
#define INDEX_BUFFER 5
*/
#define WVP_MAT_VB 6
#define WORLD_MAT_VB 7

class EG_InstancedModel : public EG_Model
{
    public:
        EG_InstancedModel();
        bool loadModel(string filename);
        bool initFromAiScene(const aiScene* s, const string& Filename);

        using EG_Model::render;
        void render(unsigned int NumInstances, glm::mat4* WVPMats, glm::mat4* WorldMats);
};


#endif
