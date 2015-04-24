#include "EG_Control.h"


EG_Text EG_Control::m_textRenderer;

EG_Control::EG_Control() : EG_Control(0,0,1,1)
{

}

EG_Control::EG_Control(int x, int y, int width, int height)
{
    update(x,y,width,height);
    m_isInside = false;
}

EG_Control::~EG_Control()
{

}


void EG_Control::init()
{

}




bool EG_Control::update(int x, int y, int width, int height)
{
    m_pos = glm::vec2(x, y);
    m_width = width;
    m_height = height;
}

bool EG_Control::update(MouseState & state)
{
    int x = state.m_pos.x;
    int y = state.m_pos.y;

    m_isInside = false;

    if( (x >= m_pos.x && x <= m_pos.x + m_width) &&
        (y >= m_pos.y && y <= m_pos.y + m_height) )
    {
        m_isInside = true;
    }
    return false;
}


void EG_Control::render()
{

}


void EG_Control::render(pipeline& m_pipeline,
                        EG_RenderTechnique* RenderTechnique,
                        int RenderPassID)
{

}
