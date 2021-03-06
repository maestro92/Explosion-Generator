#ifndef EG_TEXT
#define EG_TEXT

#include "pipeline.h"
#include "define.h"
#include "EG_Utility.h"
#include "EG_QuadModelABS.h"
#include "EG_Renderer.h"
#include "EG_GeneralRenderer.h"

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
                    int x, int y, float fontSize, const char *in_text, ...);


    //    void render1(pipeline& m_pipeline,
    //                int x, int y, const char *in_text, ...);
/*
        void render(pipeline& m_pipeline,
                    EG_Renderer* Renderer,
                    int RenderPassID,
                    int x, int y, const char *in_text, ...);
*/



        GLuint getTextWidth(const char* text);
        GLuint getTextHeight();

        int getTextWidthOffset(int i);
        int getTextWidthOffset(int i, int fontSize);
        GLuint getFontTexture();

        GLuint fontBase;
        GLuint fontTexture;

        int m_fontSize = 20;
        int m_fontSpace = -9;


    //    EG_TextRenderer    r_textRenderer;
        EG_GeneralRenderer r_textRenderer;
};


static vector<EG_QuadModelABS> m_fontQuadsABS;
static unordered_map<char, int> m_charToIndexMapping;

#endif
