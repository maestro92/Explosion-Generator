#include "EG_Button.h"


EG_Button::EG_Button()
{
    m_down = false;
    m_idleTexture = 0;
    m_highlightTexture = 0;
    m_pressedTexture = 0;
    m_vertexColors.resize(4);
}



EG_Button::EG_Button(string label, int x, int y, int width, int height, glm::vec3 c) : EG_Control(x, y, width, height, c)
{
    m_label = label;
    m_down = false;
}

/*
void EG_Button::initQuad()
{
    EG_Control::initQuad();
}
*/



/*
void EG_Button::init(int posX, int posY, int width, int height)
{
    m_position = glm::vec2(posX, posY);
//    m_rectQuad.init(400, 400);
    m_width = width;
    m_height = height;
    m_rectQuad.init(width, height);
}
*/

bool EG_Button::update(string label)
{
    m_label = label;
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



void EG_Button::render( pipeline& m_pipeline,
                        EG_RenderTechnique* RenderTechnique,
                        int RenderPassID)
{
    EG_Control::render(m_pipeline, RenderTechnique, RENDER_PASS1);



    int offset_x = computeStartingX(m_label);
    int offset_y = computeStartingY();



    cout << "offset_x " << offset_x << endl;
    cout << "offset_y " << offset_y << endl;

    EG_Control::m_textEngine.render(m_pipeline, offset_x, offset_y, m_label.c_str());
}






/*
void EG_Button::render(pipeline& m_pipeline, int x, int y)
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
*/


int EG_Button::getType()
{
    return BUTTON;
}

