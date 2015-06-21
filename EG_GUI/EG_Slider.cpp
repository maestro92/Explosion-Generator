#include "EG_Slider.h"

const int tickSize = 5;


EG_Slider::EG_Slider() : EG_Slider("", 0, 10, 0, 0, 100, 100)
{

}

EG_Slider::EG_Slider(string label, float min, float max,
               int x, int y, int width, int height) : EG_Control(x,y,width,height)
{
    m_defaultValue = 0.0f;
    m_current = NULL;
    m_maxValue = max;
    m_minValue = min;
    m_dragging = false;
    m_label = label;
    m_valueType = FLOAT_TYPE;
}

void EG_Slider::initColoredQuad()
{
    EG_Control::initColoredQuad();
    m_sliderQuadRect.set(m_rect.x, m_rect.y, tickSize, m_rect.h);
    m_sliderQuadModel.init(tickSize, m_rect.h, m_sliderColor);
}


void EG_Slider::setSliderColor(glm::vec3 c)
{
    m_sliderColor = c;
}


void EG_Slider::setMaxValue(float max)
{
    m_maxValue = max;
}

void EG_Slider::setMinValue(float min)
{
    m_minValue = min;
}

void EG_Slider::setDefaultValue(float value)
{
    m_defaultValue = value;
}



void EG_Slider::setValueType(int t)
{
    m_valueType = t;
}


void EG_Slider::setValue(float *value)
{
    m_current = value;
    if (m_current != NULL)
        m_defaultValue = *m_current;
}


float EG_Slider::getValue()
{
    return *m_current;
}

bool EG_Slider::update(MouseState &state)
{
    EG_Control::update(state);

    int x = state.m_pos.x;
    int y = state.m_pos.y;

    if(m_isInside == true)
    {
        if(state.m_leftButtonDown)
        {
            m_dragging = true;
        }

        if(state.m_rightButtonDown)
        {
            *m_current = m_defaultValue;
        }
    }

    if( !(state.m_leftButtonDown ))
        m_dragging = false;

    if( m_dragging )
    {
        *m_current = (float)(x - m_rect.x) * (m_maxValue - m_minValue)/ (float)m_rect.w + m_minValue;

        if(*m_current > m_maxValue)
            *m_current = m_maxValue;
        else if( *m_current < m_minValue )
            *m_current = m_minValue;
    }

    return m_dragging;
}




bool EG_Slider::update(MouseState& state, unsigned int& groupFlag)
{
    if(groupFlag & ( 1 << m_id) || (groupFlag==0) )
    {
        bool flag = update(state);
//        cout << "Flag is " << flag << endl;

        if(flag == true)
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
    return false;

    /*
    if(groupFlag & ( 1 << m_id))
    {
        return false;
    }

    bool flag = update(state);
    if(flag)
    {
        groupFlag = groupFlag | ( 1 << m_id);
     //   EG_Utility::debug()
        std::bitset<32> x(groupFlag);
        cout << x << endl;
    }
    else
    {
        groupFlag = groupFlag & (~( 1 << m_id));
        std::bitset<32> x(groupFlag);
        cout << x << endl;
    }
    return flag;
    */
}




bool EG_Slider::update1(MouseState & state, unsigned int& groupFlag)
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
     //       cout << x << endl;
        }
        else
        {
            groupFlag = groupFlag & (~( 1 << m_id));
     //       std::bitset<32> x(groupFlag);
       //     cout << x << endl;
        }
        return flag;
    }
    return false;
}


void EG_Slider::render(pipeline& m_pipeline,
                    EG_Renderer* Renderer,
                    int RenderPassID)
{
//    EG_Control::render(m_pipeline, Renderer, RENDER_PASS1);
    p_modelPtr = &m_quadModel;

    EG_Control::render(m_pipeline, Renderer, RENDER_PASS1, m_rect, p_modelPtr);

    int offset_x = (int)((*m_current - m_minValue) / (m_maxValue - m_minValue) * (m_rect.w - tickSize) + m_rect.x);
    int offset_y = 0;

  //  EG_Utility::debug("offset_x", offset_x);

    m_sliderQuadRect.x = offset_x;

  //  glDisable(GL_BLEND);
    p_modelPtr = &m_sliderQuadModel;
    EG_Control::render(m_pipeline,
                        Renderer,
                        RENDER_PASS1, m_sliderQuadRect, p_modelPtr);



 //   glEnable(GL_BLEND);

 //   m_label = "NICE";
 //   offset_x = computeTextStartingX(m_label);
 //   offset_y = computeTextStartingY();
    offset_x = m_rect.x + 2;
    offset_y = m_rect.y + m_rect.h - 15 - 5;


    string s = "";

    if(m_valueType == INT_TYPE)
    {
        int count = (int)(*m_current);
        stringstream ss;
        ss << count;
        string cs = ss.str();
        s = m_label + ": " + cs;
    }
    else
    {
        float num = *m_current;
        ostringstream buff;
        buff << num;
        string cs = buff.str();
        s = m_label + ": " + cs;
    }

    // offset_y = m_rect.y + m_rect.h - EG_Control::m_textEngine.fontSize - 2;
    EG_Control::m_textEngine.render(m_pipeline, offset_x, offset_y, 15, s.c_str());
}

int EG_Slider::getType()
{
    return SLIDER;
}


bool EG_Slider::getDraggingFlag()
{
    return m_dragging;
}
