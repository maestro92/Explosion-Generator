#include "EG_ListBox.h"

const int m_itemHeight = 18;

EG_ListBox::EG_ListBox() : EG_ListBox(0,0,1,1)
{
}

EG_ListBox::EG_ListBox(int x, int y, int width, int height) : EG_Control(x, y, width, height)
{
    m_curIndex = 0;
}


void EG_ListBox::initColoredQuad()
{
    EG_Control::initColoredQuad();
    m_curQuadData.set(0,0, m_rect.w, m_itemHeight);
    m_curQuadModel.init(m_rect.w, m_itemHeight, BLUE);
}


void EG_ListBox::addItem(string item)
{
    m_items.push_back(item);
}

void EG_ListBox::removeItem(int index)
{
/// vector for loop vs iterator
/// http://stackoverflow.com/questions/131241/why-use-iterators-instead-of-array-indices

    int i=0;
    for(vector<string>::iterator it = m_items.begin(); it != m_items.end(); it++)
    {
        if(i==index)
        {
            m_items.erase(it);
            break;
        }
        i++;
    }

    /// if the last item is the item being removed

    if(index >= (int) m_items.size())
        m_curIndex = (int)m_items.size() - 1;

    /*
    for(int i=0; i<m_items.size(); i++)
    {
            if(i == index)
            {
                m_items.erase()

            }
    }
*/
}

void EG_ListBox::setCurrent(int index)
{
    m_curIndex = index;
}

int EG_ListBox::getIndex()
{
    return m_curIndex;
}


int EG_ListBox::getCount()
{
    return (int)m_items.size();
}


bool EG_ListBox::update(MouseState & state)
{
    EG_Control::update(state);

    int x = state.m_pos.x;
    int y = state.m_pos.y;

    if( m_isInside == true && state.m_leftButtonDown)
    {
        int tempIndex = (y - m_rect.y) / m_itemHeight;

        if( tempIndex >= 0 && tempIndex < (int)m_items.size())
        {
            m_curIndex = tempIndex;
            return true;
        }
    }
    return false;
}


void EG_ListBox::render(pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID)
{
    EG_Control::render(m_pipeline, Renderer, RENDER_PASS1);
    int offset_x;
    int offset_y = 0;
 //   EG_Control::m_textEngine.render(m_pipeline, offset_x, offset_y, m_label.c_str());
    if( m_curIndex >= 0)
    {
        offset_x = 0;

        offset_y = m_curIndex * m_itemHeight;


        Renderer->enableShader(RenderPassID);
            m_pipeline.pushMatrix();
                glm::vec3 shift(offset_x, offset_y, 0);
                m_pipeline.translate(shift);
                Renderer->loadUniformLocations(m_pipeline, RenderPassID);
                m_curQuadModel.render();
            m_pipeline.popMatrix();
        Renderer->disableShader(RenderPassID);
    }


    for (int i = 0; i< (int) m_items.size(); i++)
    {

   //     EG_Control::m_textEngine.render(m_pipeline, offset_x, offset_y, m_label.c_str());
    }
}


int EG_ListBox::getType()
{
    return LIST_BOX;
}

