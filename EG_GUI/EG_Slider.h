#ifndef EG_SLIDER_H_
#define EG_SLIDER_H_

#include "EG_Control.h"


using namespace std;

class EG_Slider : public EG_Control
{
    public:
        EG_Slider();
        EG_Slider(string label, float min, float max,
               int x, int y, int width, int height);

        void setValue(float *value);

        void setMaxValue(float max);
        void setMinValue(float min);
        float getValue();

        virtual void initColoredQuad();

        virtual bool update(MouseState &state);

        virtual void render(pipeline& m_pipeline,
                            EG_RenderTechnique* RenderTechnique,
                            int RenderPassID);

        virtual int getType();

        bool getDraggingFlag();

    protected:

        EG_Rect m_sliderQuadRect;
        EG_QuadModelABS m_sliderQuadModel;

        float m_defaultValue;
        float m_minValue;
        float m_maxValue;
        float*  m_current;
        bool m_dragging;


};

#endif

