#ifndef EG_RECT_H_
#define EG_RECT_H_

#include "define.h"

struct EG_Rect
{
    glm::vec3 m_position;
    float m_width;
    float m_height;


    void update(glm::vec3 pos)
    {
        m_position.x = pos.x;
        m_position.y = pos.y;
        m_position.z = 0;
    }


    void update(float w, float h)
    {
        m_width = w;
        m_height = h;
    }

    void update(int x, int y, float w, float h)
    {
        m_position.x = x;
        m_position.y = y;
        m_width = w;
        m_height = h;
    }

};

#endif // EG_RECT_H_
