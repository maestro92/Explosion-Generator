#include "EG_Button.h"

/*
EG_Button::EG_Button()
{
    m_down = false;
}



EG_Button::EG_Button(string label, int posX, int posY, int width, int height) :
    EG_Control(posX, posY, width, height)
{
    m_label = label;
    m_down = false;

    m_pos = glm::vec2(posX, posY);
//    quad.init();
}


void EG_Button::init()
{
    m_rectQuad.init(m_width, m_height);
}

void EG_Button::init(int posX, int posY, int width, int height)
{
    m_pos = glm::vec2(posX, posY);
//    m_rectQuad.init(400, 400);
    m_width = width;
    m_height = height;
    m_rectQuad.init(width, height);

}



bool EG_Button::update(MouseState & state)
{
    EG_Control::update(state);
    if(m_isInside == true)
    {
        if (state.m_leftButtonDown)
            m_down = true;

        /// if it was down, we set it to false
        else if (m_down)
        {
            m_down = false;

            return true;
        }
    }
    return false;
}


void EG_Button::render(unsigned int shaderID, pipeline& m_pipeline, int x, int y, const char *in_text, ...)
{
  //  glDisable(GL_CULL_FACE);
  //  glDisable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);
 //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);

  // m_rectQuad.init(400, 400);
    m_pipeline.pushMatrix();

        m_pipeline.translate(50,100,0);
        m_pipeline.updateMatrices(shaderID);
        m_rectQuad.quad->draw(shaderID);

    m_pipeline.popMatrix();
}


int EG_Button::getType()
{
    return BUTTON;
}
*/
