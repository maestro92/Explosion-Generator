#ifndef EG_RENDERER_H
#define EG_RENDERER_H
#include "define.h"
#include "EG_Shader.h"
#include "pipeline.h"

using namespace std;

/*
const int RENDER_PASS1 = 0;
const int RENDER_PASS2 = 1;
*/

enum RenderPass
{
    RENDER_PASS1,
    RENDER_PASS2,
    RENDER_PASS3,
    RENDER_PASS4,
    RENDER_PASS5,
    RENDER_PASS6
};

const int SHADOW_RENDER = 0;
const int TWOPASS_RAYCASTING_RENDER = 1;
const int DEPTH_TEXTURE_RENDER = 2;
const int DEFERRED_SHADING_RENDER = 3;

static struct Matrices_t{
    glm::mat4 Projection;
    glm::mat4 Modelview;
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 ViewNoRotate;
    glm::mat4 ModelviewProjection;
    glm::mat4 Normal;
} Matrices;


struct Matrices_Location
{
    GLuint ProjectionMatrix;
    GLuint ModelviewMatrix;
    GLuint ViewMatrix;
    GLuint ModelviewProjection;
    GLuint ModelMatrix;
    GLuint ViewNoRotateMatrix;
    GLuint NormalMatrix;
};


class EG_Renderer
{
    public:
        EG_Renderer();
        virtual ~EG_Renderer();

        virtual void init();
        virtual void init(int nShaders) = 0;

        void allocateMemberVariables(int nShaders);
        void initMemberVariables();
        void enableShader(int RenderPassID);
        void disableShader(int RenderPassID);


        GLuint GetUniformLocation(Shader* s, const char* UniformName);
        GLuint getUniLoc(Shader* s, const char* UniformName);
        bool Init_Shader_GL_Location(Shader* s, Matrices_Location& Mat);

        virtual void loadUniformLocations(pipeline& p, int RenderPassID);
        virtual void render();

        void setUniLoc(GLuint location, int value);
        void setUniLoc(GLuint location, float value);
        void setUniLoc(GLuint location, float x, float y);
        void setUniLoc(GLuint location, glm::vec3 value);
        void setUniLoc(GLuint location, glm::vec4 value);
        void setUniLoc(GLuint location, glm::mat4 value);

        void setUniformLocation(GLuint location, int value);
        void setUniformLocation(GLuint location, float value);
        void setUniformLocation(GLuint location, float x, float y);
        void setUniformLocation(GLuint location, glm::vec3 value);
        void setUniformLocation(GLuint location, glm::vec4 value);
        void setUniformLocation(GLuint location, glm::mat4 value);
        void setUniformLocationTranspose(GLuint location, glm::mat4 value);
    public:
        Matrices_Location* Matrices_UniLoc;
        Shader** m_shaders;
    private:
        int m_numShaders;

};


#endif
