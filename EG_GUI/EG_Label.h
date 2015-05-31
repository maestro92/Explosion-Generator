#ifndef EG_LABEL_H
#define EG_LABEL_H

#include "pipeline.h"
#include "define.h"
#include "EG_Utility.h"
//#include "EG_Quad.h"


#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

#include "EG_Control.h"

using namespace std;


class EG_Label : public EG_Control
{
    public:

        EG_Label();
        EG_Label(string label, int posX, int posY, int width, int height);

        void init();

        bool update(string label);
        virtual void render(pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID);

        virtual int getType();

        string m_label;
};



#endif
