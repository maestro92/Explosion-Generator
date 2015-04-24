#ifndef EG_TEXT
#define EG_TEXT

#include "pipeline.h"
#include "define.h"
#include "EG_Utility.h"
#include "EG_Quad.h"
#include "EG_RenderTechnique.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL/SDL_opengl.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;


class EG_Text
{
    public:

        EG_Text();

        void initialize();
        void buildCharToIndexMapping();
        void buildTextureFont();

        // allows us to do printf("FPS: \f", fps);

        void render(pipeline& m_pipeline,
                    int x, int y, const char *in_text, ...);


        void render(pipeline& m_pipeline,
                    EG_RenderTechnique* RenderTechnique,
                    int RenderPassID,
                    int x, int y, const char *in_text, ...);





        GLuint getTextWidth(const char* text);
        int getTextWidthOffset(int i);
        GLuint getTextHeight(const char* text);
        GLuint getFontTexture();


        GLuint fontBase;
        GLuint fontTexture;

};


static vector<EG_Quad> m_fontQuads;
static unordered_map<char, int> m_charToIndexMapping;

#endif
