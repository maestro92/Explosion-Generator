
#include "EG_Utility.h"



EG_Utility::EG_Utility()
{

}


EG_Utility::~EG_Utility()
{

}



void EG_Utility::checkGLError()
{
    GLenum err (glGetError());

    while( err != GL_NO_ERROR )
    {
        string error;

        switch(err)
        {
                case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
        }

   //     cerr << "GL_" << error.c_str() <<" - "":"<<line<<endl;
        cout << "GL_" << error.c_str() << endl;
        err=glGetError();
    }
}


glm::vec3 EG_Utility::scaleGlmVec3(const glm::vec3 v, float s)
{
    return scaleGlmVec3(v,s,s,s);
}

glm::vec3 EG_Utility::scaleGlmVec3(const glm::vec3 v, float s1, float s2, float s3)
{
    glm::vec3 newV(s1*v.x, s2*v.y, s3*v.z);
    return newV;
}
