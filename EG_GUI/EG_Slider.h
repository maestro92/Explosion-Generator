#ifndef EG_SLIDER_H_
#define EG_SLIDER_H_

#include "EG_Control.h"


using namespace std;

class EG_Slider : public EG_Control
{
    public:
        EG_Slider(string label, float min, float max,
               int x, int y, int width, int height);

        void setValue(float *value);

        virtual bool update(MouseState &state);
        virtual void render(pipeline& m_pipeline,
                            EG_RenderTechnique* RenderTechnique,
                            int RenderPassID);

        virtual int getType();
    protected:

        float m_defaultValue;
        float m_minValue;
        float m_maxValue;
        float*  m_current;
        string m_label;
        bool m_dragging;


};

#endif

