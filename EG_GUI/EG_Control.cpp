#include "EG_Control.h"


EG_Text EG_Control::m_textEngine;



EG_Control::EG_Control() : EG_Control(0,0,1,1)
{

}

EG_Control::EG_Control(int x, int y, int width, int height)
{
    update(x,y,width,height);
    m_isInside = false;
}


EG_Control::EG_Control(int x, int y, int width, int height, glm::vec3 c) : EG_Control(x, y, width, height, c, c, c, c)
{
}

EG_Control::EG_Control(int x, int y, int width, int height, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4)
{
    m_vertexColors.resize(4);
    m_vertexColors[0] = c1;
    m_vertexColors[1] = c2;
    m_vertexColors[2] = c3;
    m_vertexColors[3] = c4;
}



EG_Control::~EG_Control()
{

}


void EG_Control::initTexturedQuad()
{
    m_quad.init(m_width, m_height);
}

void EG_Control::initColoredQuad()
{
    m_quad.init(m_width, m_height, m_vertexColors[0], m_vertexColors[1], m_vertexColors[2], m_vertexColors[3]);
}


bool EG_Control::update(int x, int y, int width, int height)
{
    m_position = glm::vec2(x, y);
    m_width = width;
    m_height = height;
}

bool EG_Control::update(glm::vec3 c)
{
    update(c,c,c,c);
}

bool EG_Control::update(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4)
{
    m_vertexColors[0] = c1;
    m_vertexColors[1] = c2;
    m_vertexColors[2] = c3;
    m_vertexColors[3] = c4;
}

bool EG_Control::update(MouseState & state)
{
    int x = state.m_pos.x;
    int y = state.m_pos.y;

    m_isInside = false;

    if( (x >= m_position.x && x <= m_position.x + m_width) &&
        (y >= m_position.y && y <= m_position.y + m_height) )
    {
        m_isInside = true;
    }
    return false;
}



int EG_Control::computeStartingX(string s)
{
    int textWidth = m_textEngine.getTextWidth(s.c_str());
    int sx = m_width - textWidth;
    return m_position.x + sx/2;
}

int EG_Control::computeStartingY()
{
    int textHeight = m_textEngine.getTextHeight();
    int sy = m_height - textHeight;

    cout << "m_height " << m_height << endl;
    cout << "textHeight " << textHeight << endl;


    return m_position.y + sy/2;
}



void EG_Control::render(pipeline& m_pipeline,
                        EG_RenderTechnique* RenderTechnique,
                        int RenderPassID)
{
    RenderTechnique->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        glm::vec3 shift(m_position.x, m_position.y, 0);
        m_pipeline.translate(shift);
    //    m_pipeline.scale(m_scale);
        RenderTechnique->loadUniformLocations(m_pipeline, RenderPassID);
        m_quad.render();
    m_pipeline.popMatrix();
    RenderTechnique->disableShader(RenderPassID);
}
