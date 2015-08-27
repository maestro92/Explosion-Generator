#ifndef EG_QUAD_MODEL_ABS_H_
#define EG_QUAD_MODEL_ABS_H_


#include "EG_ModelABS.h"

using namespace std;

class EG_QuadModelABS : public EG_ModelABS
{
    public:
        // constructor
        EG_QuadModelABS();
        EG_QuadModelABS(int i);

        // Destructor
		~EG_QuadModelABS();

        /// textures
        void initIndices(vector<unsigned int>& indices);
        void init();
        void init(int w, int h);
        void init(int w, int h, float cx, float cy, float cw);


        /// color
        void init(int w, int h, glm::vec3 c);
        void init(int w, int h, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);

        void initCentered();

        void init(float l, float r,
                  float b, float t,
                  float cx, float cy, float cw);
};


#endif
