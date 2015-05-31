#ifndef EG_BUTTON_H
#define EG_BUTTON_H



#include "EG_Control.h"

class EG_Button : public EG_Control
{
  //  using EG_Control::update;

    public:
        EG_Button();
        EG_Button(string label, int x, int y, int width, int height,
                   glm::vec3 c);

        /// http://stackoverflow.com/questions/4271245/why-do-i-get-no-matching-function-when-i-inherit-this-function
        using EG_Control::update;

        virtual void initColoredQuad();

        virtual bool update(MouseState & state);
        virtual bool update(MouseState & state, unsigned int& groupFlag);
  //      virtual bool update1(MouseState & state, unsigned int& groupFlag);
    //    virtual bool update(string label);
    //    virtual void render(int x, int y);

  //      virtual bool update(glm::vec3 c);
  //      virtual bool update(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);

        virtual void render(pipeline& m_pipeline,
                            EG_Renderer* Renderer,
                            int RenderPassID);

        virtual int getType();

    protected:
        bool m_down;
    //    string m_label;
//        EG_Quad m_rectQuad;


        EG_QuadModelABS m_highlightQuadModel;
        EG_QuadModelABS m_pressedQuadModel;

        GLuint m_idleTexture;
        GLuint m_highlightTexture;
        GLuint m_pressedTexture;
        /// have three textures for it

};




#endif // EG_BUTTON_H
