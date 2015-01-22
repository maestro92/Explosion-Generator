#ifndef WORLD_AXIS_H_
#define WORLD_AXIS_H_

#include <iostream>
#include <cstdlib>

#include "pipeline.h"
#include "mesh.h"
#include "sceneLoader.h"
#include "EG_RenderTechnique.h"

using namespace std;

class EG_WorldAxis
{
    public:
        // Constructor
        EG_WorldAxis();
		~EG_WorldAxis();

        mesh* worldAxis;

        void init();
        void render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID);
   //     void buildAxisMesh();
};

#endif
