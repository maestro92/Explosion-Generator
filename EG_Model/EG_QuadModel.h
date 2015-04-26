#ifndef EG_QUAD_MODEL_H_
#define EG_QUAD_MODEL_H_

#include "sceneLoader.h"

class EG_QuadModel
{
    public:
        // constructor
        EG_QuadModel();

        // Destructor
		~EG_QuadModel();
        mesh* quad;
        void init();
        void init(int w, int h);
        void init(int w, int h, float cx, float cy, float cw);
};


#endif
