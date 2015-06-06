
#ifndef EG_BILLBOARD_LIST_H_
#define EG_BILLBOARD_LIST_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "EG_Utility.h"
#include "pipeline.h"
#include "EG_Renderer.h"

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"

#include <iostream>
#include <string>

using namespace std;

class EG_BillboardList
{
    public:
        EG_BillboardList();
        ~EG_BillboardList();

        void init(string tex);

    //    void render();
        void render(pipeline& p, EG_Renderer* Renderer, int pass);

        void createPositionBuffer();

        GLuint m_VB;
        GLuint m_billboardTexture;

};



#endif // EG_BILLBOARD_LIST_H_
