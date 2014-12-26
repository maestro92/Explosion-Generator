#ifndef TECHNIQUE_H
#define TECHNIQUE_H
#include "define.h"
#include "EG_Shader.h"
#include "pipeline.h"
using namespace std;


const int RENDER_PASS1 = 0;
const int RENDER_PASS2 = 1;

const int SHADOW_RENDER = 0;
const int TWOPASS_RAYCASTING_RENDER = 1;
const int DEPTH_TEXTURE_RENDER = 2;

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


class Technique
{
    public:
        Technique();
        virtual ~Technique();

        virtual void init();

        void allocate_memberVariables(int Shader_Num);
        void init_memberVariables();

        void EnableShader(int RenderPassID);
        void DisableShader(int RenderPassID);


        GLuint GetUniformLocation(Shader* s, const char* UniformName);
        bool Init_Shader_GL_Location(Shader* s, Matrices_Location& Mat);

        void Load_glUniform(pipeline& p, int RenderPassID);

        void Setup_Special_glUniform(pipeline& p, int RenderPassID);
        void Setup_ShadowMatrix_forRender(pipeline& p, int RenderPassID);

    public:
        Matrices_Location* Matrices_UniLoc;
        Shader** ProgShaders;
        int shader_num;

};


#endif
