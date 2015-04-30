#ifndef EG_FULL_COLOR_RENDERER
#define EG_FULL_COLOR_RENDERER

#include "EG_RenderTechnique.h"

class EG_FullColorRenderer : public EG_RenderTechnique
{
    public:
        EG_FullColorRenderer();
        ~EG_FullColorRenderer();
        void init(int nShaders);

        void setColor(glm::vec3 color);

        GLuint m_colorUniformLocation;
};

#endif // EG_RENDER_TECHNIQUE_FULL_COLOR
