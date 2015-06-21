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


void EG_Button::initColoredQuad()
{
    EG_Control::initColoredQuad();
//    m_highlightQuadModel.init(m_width, m_height, BLUE);
//    m_pressedQuadModel.init(m_width, m_height, GREEN);
    m_highlightQuadModel.init(m_rect.w, m_rect.h, BLUE);
    m_pressedQuadModel.init(m_rect.w, m_rect.h, GREEN);

}




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


/*
bool EG_Button::update(string label)
{
    m_label = label;
}
*/

bool EG_Button::update(MouseState & state)
{
    EG_Control::update(state);
    if(m_isInside == true)
    {
        if (state.m_leftButtonDown)
        {
            m_down = true;
//            cout << "pressing the button" << endl;
        }

        /// if it was down, we set it to false
        else if (m_down)
        {
            m_down = false;

            return true;
        }
    }
    return false;
}



bool EG_Button::update(MouseState & state, unsigned int& groupFlag)
{
    if(groupFlag & ( 1 << m_id) || (groupFlag==0) )
    {
        bool flag = update(state);
//        cout << "Flag is " << flag << endl;

        if(flag == true)
        {
            groupFlag = groupFlag | ( 1 << m_id);
         //   EG_Utility::debug()
      //      std::bitset<32> x(groupFlag);
       //     cout << x << endl;
        }
        else
        {
            groupFlag = groupFlag & (~( 1 << m_id));
         //   std::bitset<32> x(groupFlag);
         //   cout << x << endl;
        }
        return flag;
    }
    return false;

    /*
    bool flag = update(state);
 //   cout << "Flag is " << flag << endl;

    if(flag)
    {
        groupFlag = groupFlag | ( 1 << m_id);
     //   EG_Utility::debug()
        std::bitset<32> x(groupFlag);
    //    cout << x << endl;
    }
    else
    {
        groupFlag = groupFlag & (~( 1 << m_id));
        std::bitset<32> x(groupFlag);
  //      cout << x << endl;
    }
    return flag;
*/
}






void EG_Button::render( pipeline& m_pipeline,
                        EG_Renderer* Renderer,
                        int RenderPassID)
{
    if(m_isInside && !m_down)
    {
        p_modelPtr = &m_highlightQuadModel;
        EG_Control::render(m_pipeline, Renderer, RENDER_PASS1, p_modelPtr);

        float offset_x = 0.025 * m_rect.w;
        float offset_y = 0.05 * m_rect.h;

        m_pipeline.pushMatrix();
            m_pipeline.translate( glm::vec3(m_rect.x + offset_x, m_rect.y + offset_y, 0) );
            m_pipeline.scale(0.95,0.9,1.0);
            EG_Control::customMatrixRender(m_pipeline, Renderer, RENDER_PASS1);
        m_pipeline.popMatrix();




/*
     //   p_modelPtr = &m_quadModel;
        m_pipeline.pushMatrix();

            m_pipeline.translate( glm::vec3(m_rect.x, m_rect.y, 0) );
        //    m_pipeline.scale(0.9);
            Renderer->loadUniformLocations(m_pipeline, RenderPassID);
            m_quadModel.render();

        m_pipeline.popMatrix();
  */
    }


    else if (m_down)
    {
        p_modelPtr = &m_pressedQuadModel;
        EG_Control::render(m_pipeline, Renderer, RENDER_PASS1, p_modelPtr);
    }
    else
        EG_Control::render(m_pipeline, Renderer, RENDER_PASS1);



    int offset_x = computeTextStartingX(m_label);
    int offset_y = computeTextStartingY();



 //   cout << "offset_x " << offset_x << endl;
 //   cout << "offset_y " << offset_y << endl;

    EG_Control::m_textEngine.render(m_pipeline, offset_x, offset_y, m_label.c_str());
}







int EG_Button::getType()
{
    return BUTTON;
}

