#ifndef EG_FULL_COLOR_RENDERER
#define EG_FULL_COLOR_RENDERER

#include "EG_Renderer.h"

class EG_FullColorRenderer : public EG_Renderer
{
    public:
        EG_FullColorRenderer();
        ~EG_FullColorRenderer();
        void init(int nShaders);

        void setColor(glm::vec3 color);

        GLuint m_colorUniformLocation;
};

#endif // EG_RENDER_TECHNIQUE_FULL_COLOR
