
#ifndef EG_GUI_CONTROL
#define EG_GUI_CONTROL


#include "pipeline.h"
#include "define.h"
#include "EG_Utility.h"
#include "EG_Quad.h"
#include "EG_Text.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "define.h"
#include <iostream>
#include <string>


using namespace std;

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

        EG_Control();
        EG_Control(int posX, int posY, int width, int height);

        void initTextRenderer();

        virtual ~EG_Control();

        virtual void init();
        virtual bool update(int posX, int posY, int width, int height);
        virtual bool update(MouseState & state);
        virtual void render();


        virtual void render(pipeline& m_pipeline,
                            EG_RenderTechnique* RenderTechnique,
                            int RenderPassID);

        virtual int getType() = 0;

        void setPosition(int x, int y);
        void setSize(int width, int height);

        int getWidth();
        int getHeight();

        glm::vec2 m_pos;
        int m_width;
        int m_height;
        bool m_isInside;

        /// http://stackoverflow.com/questions/7083612/defining-a-static-variable-of-base-class
        static EG_Text m_textRenderer;
 //       EG_Quad m_rectQuad;
};


#endif // EG_GUI_CONTROLS
