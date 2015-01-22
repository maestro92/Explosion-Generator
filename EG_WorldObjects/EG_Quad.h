#ifndef EG_FULL_SCREEN_QUAD_H_
#define EG_FULL_SCREEN_QUAD_H_

#include "sceneLoader.h"

class EG_Quad
{
    public:
        // constructor
        EG_Quad();

        // Destructor
		~EG_Quad();
        mesh* quad;
        void init();

};


#endif
