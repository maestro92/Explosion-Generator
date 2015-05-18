
#ifndef EG_GUI_CONTROL_H_
#define EG_GUI_CONTROL_H_


#include "pipeline.h"
#include "define.h"
#include "EG_Utility.h"
#include "EG_QuadModel.h"
#include "EG_Text.h"
#include "EG_Rect.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "define.h"
#include <iostream>
#include <string>
#include <bitset>

using namespace std;


//static glm::vec3 WHITE;
#define BLACK   glm::vec3(0.0,0.0,0.0)
#define BLUE    glm::vec3(0.0,0.0,1.0)
#define GREEN   glm::vec3(0.0,1.0,0.0)
#define TEAL    glm::vec3(0.0,1.0,1.0)
#define RED     glm::vec3(1.0,0.0,0.0)
#define PURPLE  glm::vec3(1.0,0.0,1.0)
#define YELLOW  glm::vec3(1.0,1.0,0.0)
#define DARK_GRAY    glm::vec3(0.25,0.25,0.25)
#define GRAY    glm::vec3(0.5,0.5,0.5)
#define WHITE   glm::vec3(1.0,1.0,1.0)


struct MouseState
{
    int m_leftButtonDown;
    int m_rightButtonDown;
    int m_middleButtonDown;

    glm::vec2 m_pos;

    MouseState()
    {
        m_leftButtonDown = 0;
        m_rightButtonDown = 0;
        m_middleButtonDown = 0;

        m_pos = glm::vec2(0,0);
    }
};


class EG_Control
{
    public:

        enum CONTROL_TYPE
        {   LABEL = 0,
            BUTTON,
            LIST_BOX,
            SLIDER
        };
    public:
        EG_Control();
        EG_Control(int x, int y, int width, int height);

        EG_Control(int x, int y, int width, int height,
                   glm::vec3 c);

        EG_Control(int x, int y, int width, int height,
                   glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);

        void initTextRenderer();

        virtual ~EG_Control();

/*
        virtual void initQuad();
        virtual void initQuad(glm::vec3 c);
        virtual void initQuad(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);
*/

        virtual void initTexturedQuad();
        virtual void initColoredQuad();
        virtual void initColoredQuad(int w, int h, glm::vec3 c);
//        virtual void initColoredQuad(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);



    //    virtual bool update(int x, int y, int width, int height);
     //   virtual bool update(glm::vec3 c);
    //    virtual bool update(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);
        virtual bool update(MouseState & state);
        virtual bool update(MouseState & state, unsigned int& groupFlag);

        virtual void render(pipeline& m_pipeline,
                            EG_RenderTechnique* RenderTechnique,
                            int RenderPassID);

        virtual void customMatrixRender(pipeline& m_pipeline,
                        EG_RenderTechnique* RenderTechnique,
                        int RenderPassID);

        virtual void render(pipeline& m_pipeline,
                        EG_RenderTechnique* RenderTechnique,
                        int RenderPassID, EG_ModelABS* model);

        virtual void render(pipeline& m_pipeline,
                        EG_RenderTechnique* RenderTechnique,
                        int RenderPassID, EG_Rect r, EG_ModelABS* model);


        virtual int getType() = 0;

        void setID(int& ID);
        void setPosition(int x, int y);
        void setSize(int width, int height);
        void setRect(int x, int y, int w, int h);
        void setColor(glm::vec3 c);
        void setColor(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);
        void setLabel(string label);
        int computeTextStartingX(string s);
        int computeTextStartingY();



        int getWidth();
        int getHeight();

        EG_ModelABS* p_modelPtr;
     //   glm::vec2 m_position;
     //   int m_width;
     //   int m_height;
        bool m_isInside;
        string m_label;

        int m_id;

        EG_Rect m_rect;

        /// http://stackoverflow.com/questions/7083612/defining-a-static-variable-of-base-class

        static EG_Text m_textEngine;
        EG_QuadModelABS m_quadModel;
        vector<glm::vec3> m_vertexColors;
};


#endif // EG_GUI_CONTROLS
