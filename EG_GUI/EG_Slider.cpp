#include "EG_Slider.h"

const int tickSize = 5;
EG_Slider::EG_Slider(string label, float min, float max,
               int x, int y, int width, int height) : EG_Control(x,y,width,height)
{
    m_defaultValue = 0.0f;
    m_current = NULL;
    m_maxValue = max;
    m_minValue = min;
    m_dragging = false;
    m_label = label;
}

void EG_Slider::setValue(float *value)
{
    m_current = value;
    if (m_current != NULL)
        m_defaultValue = *m_current;
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
        *m_current = (float)(x - m_position.x) * (m_maxValue - m_minValue)/ (float)m_width;

        if(*m_current > m_maxValue)
            *m_current = m_maxValue;
        else if( *m_current < m_minValue )
            *m_current = m_minValue;
    }

    return m_dragging;
}


void EG_Slider::render(pipeline& m_pipeline,
                    EG_RenderTechnique* RenderTechnique,
                    int RenderPassID)
{
    EG_Control::render(m_pipeline, RenderTechnique, RENDER_PASS1);

    int offset_x = (int)((*m_current - m_position.x) * (m_maxValue - m_minValue)/ (float)m_width) + m_position.x;
}

int EG_Slider::getType()
{
    return SLIDER;
}
