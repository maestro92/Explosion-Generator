#ifndef EG_RENDERER_H
#define EG_RENDERER_H
#include "define.h"
#include "EG_Shader.h"
#include "pipeline.h"
#include "EG_Utility.h"
#include <unordered_map>


using namespace std;

/*
const int RENDER_PASS1 = 0;
const int RENDER_PASS2 = 1;
*/

enum RenderPass
{
    RENDER_PASS1=0,
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



// http://blogs.msdn.com/b/oldnewthing/archive/2004/05/07/127826.aspx
// http://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
/*
always make the base classes' destructors virtual when they're meant to be manipulated polymorphically
*/
struct DataPair
{
    GLuint uniLoc;

    virtual ~DataPair(){};

    virtual void setUniLoc(){};
    virtual void printValue(){};

    virtual void setValue(int value){};
    virtual void setValue(float value){};
    virtual void setValue(glm::vec2 value){};
    virtual void setValue(glm::vec3 value){};
    virtual void setValue(glm::vec4 value){};
    virtual void setValue(glm::mat3 value){};
    virtual void setValue(glm::mat4 value){};
};

struct IntDataPair : public DataPair
{
    int value;
    ~IntDataPair(){};

    virtual void setValue(int value)
    {
        this->value = value;
    }

    virtual void setUniLoc()
    {
        EG_Utility::setUniLoc(uniLoc, value);
    }

    virtual void printValue()
    {
        EG_Utility::debug("value is", value);
    }
};

struct FloatDataPair : public DataPair
{
    float value;
    ~FloatDataPair(){};

    void setValue(float value)
    {
        this->value = value;
    }

    void setUniLoc()
    {
        EG_Utility::setUniLoc(uniLoc, value);
    }

    void printValue()
    {
        EG_Utility::debug("value is", value);
    }
};

struct Vec2DataPair : public DataPair
{
    glm::vec2 value;
    ~Vec2DataPair(){};

    void setValue(glm::vec2 value)
    {
        this->value = value;
    }

    void setUniLoc()
    {
        EG_Utility::setUniLoc(uniLoc, value);
    }

    void printValue()
    {
        EG_Utility::debug("value is", value);
    }
};

struct Vec3DataPair : public DataPair
{
    glm::vec3 value;
    ~Vec3DataPair(){};

    void setValue(glm::vec3 value)
    {
        this->value = value;
    }

    void setUniLoc()
    {
        EG_Utility::setUniLoc(uniLoc, value);
    }

    void printValue()
    {
        EG_Utility::debug("value is", value);
    }
};

struct Vec4DataPair : public DataPair
{
    glm::vec4 value;
    ~Vec4DataPair(){};


    void setValue(glm::vec4 value)
    {
        this->value = value;
    }

    void setUniLoc()
    {
        EG_Utility::setUniLoc(uniLoc, value);
    }

    void printValue()
    {
        EG_Utility::debug("value is", value);
    }
};

struct Mat3DataPair : public DataPair
{
    glm::mat3 value;
    ~Mat3DataPair(){};

    void setValue(glm::mat3 value)
    {
        this->value = value;
    }

    void setUniLoc()
    {
        EG_Utility::setUniLoc(uniLoc, value);
    }

    void printValue()
    {
        EG_Utility::debug("value is", value);
    }
};

struct Mat4DataPair : public DataPair
{
    glm::mat4 value;
    ~Mat4DataPair(){};

    void setValue(glm::mat4 value)
    {
        this->value = value;
    }

    void setUniLoc()
    {
        EG_Utility::setUniLoc(uniLoc, value);
    }

    void printValue()
    {
        EG_Utility::debug("value is", value);
    }
};




enum DATA_PAIR_TYPE
{
    DP_INT = 0,
    DP_FLOAT,
    DP_VEC2,
    DP_VEC3,
    DP_VEC4,
    DP_MAT3,
    DP_MAT4,
};


class EG_Renderer
{
    public:
        EG_Renderer();
        virtual ~EG_Renderer();

        virtual void init();
        virtual void init(int nShaders) = 0;

        void addShader(Shader* s);
        void allocateMemberVariables(int nShaders);
        void initMemberVariables();
        void enableShader(int RenderPassID);
        void disableShader(int RenderPassID);


        GLuint GetUniformLocation(Shader* s, const char* UniformName);


        bool Init_Shader_GL_Location(Shader* s, Matrices_Location& Mat);

        virtual void loadUniformLocations(pipeline& p, int RenderPassID);
        virtual void render();

        void initDataPairUniLoc(DataPair* p, int pass, const char* name);
        void addDataPair(int pass, const char* name, int dataType);

        GLuint getUniLoc(Shader* s, const char* name);


        void setUniformLocation(GLuint location, int value);
        void setUniformLocation(GLuint location, float value);
        void setUniformLocation(GLuint location, float x, float y);
        void setUniformLocation(GLuint location, glm::vec2 value);
        void setUniformLocation(GLuint location, glm::vec3 value);
        void setUniformLocation(GLuint location, glm::vec4 value);
        void setUniformLocation(GLuint location, glm::mat3 value);
        void setUniformLocation(GLuint location, glm::mat4 value);
        void setUniformLocationTranspose(GLuint location, glm::mat4 value);

        void setAllDataPairUniLocs(int pass);

        void setDataPairUniLoc(IntDataPair& dp);
        void setDataPairUniLoc(FloatDataPair& dp);
        void setDataPairUniLoc(Vec2DataPair& dp);
        void setDataPairUniLoc(Vec3DataPair& dp);
        void setDataPairUniLoc(Vec4DataPair& dp);
        void setDataPairUniLoc(Mat3DataPair& dp);
        void setDataPairUniLoc(Mat4DataPair& dp);


        void printTables();
        void setData(int pass, const char* name, int value);
        void setData(int pass, const char* name, float value);
        void setData(int pass, const char* name, glm::vec2 value);
        void setData(int pass, const char* name, glm::vec3 value);
        void setData(int pass, const char* name, glm::vec4 value);
        void setData(int pass, const char* name, glm::mat3 value);
        void setData(int pass, const char* name, glm::mat4 value);


        void getAllMatrixUniLocs();
        bool getMatrixUniLocs(Shader* s, Matrices_Location& Mat);

    public:
//        Matrices_Location* Matrices_UniLoc;
//        Shader** m_shaders;
//        vector< vector<DataPair*> > m_allDataPairs;


        vector<Matrices_Location> m_matricesUniLocs;
        vector<Shader*> m_shaders;
        vector< vector<DataPair*> > m_allDataPairs;

        vector< unordered_map<string, DataPair*> > tables;

    private:
        int m_numShaders;
        int m_curShader;

};


#endif
