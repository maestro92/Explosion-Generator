#include "EG_Control.h"


EG_Text EG_Control::m_textEngine;



EG_Control::EG_Control() : EG_Control(0,0,1,1)
{

}

EG_Control::EG_Control(int x, int y, int width, int height)
{
    m_vertexColors.resize(4);
    setRect(x,y,width,height);
    m_isInside = false;
}


EG_Control::EG_Control(int x, int y, int width, int height, glm::vec3 c) : EG_Control(x, y, width, height, c, c, c, c)
{
}

EG_Control::EG_Control(int x, int y, int width, int height, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4)
{
    m_vertexColors.resize(4);
    setColor(c1,c2,c3,c4);
}



EG_Control::~EG_Control()
{

}


void EG_Control::setID(int& ID)
{
    m_id = ID;
    ID++;
}


void EG_Control::setColor(glm::vec3 c)
{
    setColor(c,c,c,c);
}

void EG_Control::setColor(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4)
{
    m_vertexColors[0] = c1;
    m_vertexColors[1] = c2;
    m_vertexColors[2] = c3;
    m_vertexColors[3] = c4;
}


void EG_Control::setLabel(string label)
{
    m_label = label;
}

void EG_Control::initTexturedQuad()
{
    m_quadModel.init(m_rect.w, m_rect.h);
}

void EG_Control::initColoredQuad()
{
    m_quadModel.init(m_rect.w, m_rect.h, m_vertexColors[0], m_vertexColors[1], m_vertexColors[2], m_vertexColors[3]);
}

void EG_Control::initColoredQuad(int w, int h, glm::vec3 c)
{
    m_quadModel.init(w, h, c, c, c, c);
}


void EG_Control::setPosition(int x, int y)
{
//    m_position = glm::vec2(x, y);

    m_rect.setXY(x,y);
}

void EG_Control::setSize(int width, int height)
{
 //   m_width = width;
 //   m_height = height;

    m_rect.w = width;
    m_rect.h = height;
}

void EG_Control::setRect(int x, int y, int w, int h)
{
    setPosition(x,y);
    setSize(w,h);
}


/*
bool EG_Control::update(int x, int y, int width, int height)
{
    m_position = glm::vec2(x, y);
    m_width = width;
    m_height = height;
}
*/
/*
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
*/

bool EG_Control::update(MouseState & state)
{
    int x = state.m_pos.x;
    int y = state.m_pos.y;

//    EG_Utility::debug("state.m_pos", state.m_pos);
 //   EG_Utility::debug("m_position", glm::vec2(m_rect.x, m_rect.y));


    m_isInside = false;



    if( (x >= m_rect.x && x <= m_rect.x + m_rect.w) &&
        (y >= m_rect.y && y <= m_rect.y + m_rect.h) )
    {
        m_isInside = true;
    }

/*
    if( (x >= m_rect.m_position.x && x <= m_rect.m_position.x + m_rect.m_width) &&
        (y >= m_rect.m_position.y && y <= m_rect.m_position.y + m_rect.m_height) )
    {
        m_isInside = true;
    }
*/
    return false;
}


bool EG_Control::update(MouseState & state, unsigned int& groupFlag)
{
    bool flag = update(state);
    if(flag)
    {
        groupFlag = groupFlag | ( 1 << m_id);
     //   EG_Utility::debug()
     //   std::bitset<32> x(groupFlag);
     //   cout << x << endl;
    }
    else
    {
        groupFlag = groupFlag & (~( 1 << m_id));
     //   std::bitset<32> x(groupFlag);
     //   cout << x << endl;
    }
    return flag;
}


int EG_Control::computeTextStartingX(string s)
{
    int textWidth = m_textEngine.getTextWidth(s.c_str());
    int sx = m_rect.w - textWidth;
    return m_rect.x + sx/2;
}

int EG_Control::computeTextStartingY()
{
    int textHeight = m_textEngine.getTextHeight();
    int sy = m_rect.h - textHeight;

//    cout << "m_height " << m_height << endl;
//    cout << "textHeight " << textHeight << endl;


    return m_rect.y + sy/2;
}


/*
int EG_Control::computeTextStartingX(string s)
{
    int textWidth = m_textEngine.getTextWidth(s.c_str());
    int sx = m_rect.w - textWidth;
    return m_rect.x + sx/2;
}

int EG_Control::computeTextStartingY()
{
    int textHeight = m_textEngine.getTextHeight();
    int sy = m_rect.h - textHeight;

//    cout << "m_height " << m_height << endl;
//    cout << "textHeight " << textHeight << endl;


    return m_rect.y + sy/2;
}
*/


void EG_Control::render(pipeline& m_pipeline,
                        EG_Renderer* Renderer,
                        int RenderPassID)
{
    Renderer->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
    //    glm::vec3 trans(m_position.x, m_position.y, 0);
    //    m_pipeline.translate(trans);
    //    m_pipeline.scale(m_scale);
        m_pipeline.translate( glm::vec3(m_rect.x, m_rect.y, 0) );
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        m_quadModel.render();
    m_pipeline.popMatrix();
    Renderer->disableShader(RenderPassID);
}



void EG_Control::customMatrixRender(pipeline& m_pipeline,
                        EG_Renderer* Renderer,
                        int RenderPassID)
{
    Renderer->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        m_quadModel.render();
    m_pipeline.popMatrix();
    Renderer->disableShader(RenderPassID);
}


void EG_Control::render(pipeline& m_pipeline,
                        EG_Renderer* Renderer,
                        int RenderPassID, EG_ModelABS* model)
{
    Renderer->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        glm::vec3 trans(m_rect.x, m_rect.y, 0);
        m_pipeline.translate(trans);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        model->render();
    m_pipeline.popMatrix();
    Renderer->disableShader(RenderPassID);
}

void EG_Control::render(pipeline& m_pipeline,
                        EG_Renderer* Renderer,
                        int RenderPassID, EG_Rect r, EG_ModelABS* model)
{
    Renderer->enableShader(RenderPassID);
    m_pipeline.pushMatrix();
        m_pipeline.translate(glm::vec3(r.x, r.y, 0));
      //  m_pipeline.scale(0.9);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        model->render();
    m_pipeline.popMatrix();
    Renderer->disableShader(RenderPassID);
}
