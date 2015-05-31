#ifndef EG_SLIDER_H_
#define EG_SLIDER_H_

#include "EG_Control.h"


using namespace std;

class EG_Slider : public EG_Control
{


    public:
        enum SLIDER_VALUE_TYPE
        {
            INT_TYPE = 0,
            FLOAT_TYPE
        };


        EG_Slider();
        EG_Slider(string label, float min, float max,
               int x, int y, int width, int height);

        void setDefaultValue(float value);
        void setValue(float *value);

        void setSliderColor(glm::vec3 c);

        void setMaxValue(float max);
        void setMinValue(float min);
        float getValue();

        virtual void initColoredQuad();

        virtual bool update(MouseState &state);
        virtual bool update(MouseState &state, unsigned int& groupFlag);
        virtual bool update1(MouseState & state, unsigned int& groupFlag);

        virtual void render(pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID);

        virtual int getType();
        void setValueType(int t);
        bool getDraggingFlag();

    protected:

        EG_Rect m_sliderQuadRect;
        EG_QuadModelABS m_sliderQuadModel;
        glm::vec3 m_sliderColor;
        float m_defaultValue;
        float m_minValue;
        float m_maxValue;
        float*  m_current;
        bool m_dragging;
        int m_valueType;


};

#endif

