#include "EG_Label.h"

EG_Label::EG_Label()
{

}

EG_Label::EG_Label(string label, int posX, int posY, int width, int height)
{

}


void EG_Label::init()
{

}


bool EG_Label::update(string label)
{
    m_label = label;
    return true;
}




void EG_Label::render(pipeline& m_pipeline,
                    EG_Renderer* Renderer,
                    int RenderPassID)
{
//    m_textRenderer.render(m_pipeline, Renderer, RenderPassID, int(m_pos.x), int(m_pos.y), "nice");
}


int EG_Label::getType()
{
    return LABEL;
}
