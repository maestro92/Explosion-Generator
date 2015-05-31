#ifndef EG_CUBE_MODEL_H_
#define EG_CUBE_MODEL_H_


#include "EG_ModelABS.h"

using namespace std;

class EG_CubeModel : public EG_ModelABS
{
    public:
        EG_CubeModel();
        ~EG_CubeModel();

        /// textures
        void initIndices(vector<unsigned int>& indices);
        void init();
        void init(int w, int h);
        void init(int w, int h, float cx, float cy, float cw);

        /// color
        void init(int w, int h, glm::vec3 c);
        void init(int w, int h, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);
};




#endif // EG_CUBE_MODEL_H_
