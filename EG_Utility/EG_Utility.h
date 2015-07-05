#ifndef EG_UTILITY_H
#define EG_UTILITY_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>


#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "EG_Shader.h"


#include "define.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <iomanip>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


using namespace std;

//typedef std::<EG_NoiseBasedParticle> EG_NoiseBasedParticleList;



struct EG_TexturePod
{
    GLuint id;
    int width;
    int height;
    int depth;
};

struct EG_SurfacePod
{
    GLuint FBO;
    GLuint colorTexture;
    GLuint depthTexture;
};



class EG_Utility
{
    public:
        EG_Utility();
        ~EG_Utility();


        template<typename T>
            static vector<T> reserveVector(int size);

        /// EG_Utility_Math.cpp
        static float DEGREE_TO_RADIAN;
        static float RADIAN_TO_DEGREE;
        static string floatToStr(float value);
        static float randFloat(float min=0, float max=1);


        /// EG_Utility_GL.cpp
        static void errorCheck();
        static GLuint createFBO();
        static void errorCheckFBO();
        static SDL_Surface* loadRawImage(string filename);
        static SDL_Surface* loadSDLImage(string filename);
        static GLuint loadTexture(string filename);
        static GLuint createTexture(int w, int h);
        static GLuint create3DTexture(int w, int h, int d);

        static GLuint createDepthTexture(int, int h);
        static GLuint createCubemapTexture();
        static GLuint createCubemapTexture(string* filenames);
        static void setTextureParameters(int w, int h, int internal_format, int format);
        static void setCubemapTextureParameters();
        static EG_SurfacePod createSurface(int width, int height);



        /// EG_Utility_AssimpMath.cpp
        static glm::vec3 toGlmVec(aiVector3D& v2);
        static glm::mat4 toGlmMat(aiMatrix3x3 m2);
        static glm::mat4 toGlmMat(aiMatrix4x4& m2);
        static glm::mat4 toGlmMat(const aiMatrix4x4& m2);

        static glm::vec2 scaleGlmVec(const glm::vec2 v, float s);
        static glm::vec2 scaleGlmVec(const glm::vec2 v, float s1, float s2);
        static glm::vec3 scaleGlmVec(const glm::vec3 v, float s);
        static glm::vec3 scaleGlmVec(const glm::vec3 v, float s1, float s2, float s3);


        /// EG_Utility_Debug.cpp
        static void debug(string s);
        static void debug(string s, string s2);
        static void debug(string s, unsigned int i);
        static void debug(string s, int i);
        static void debug(string s, float f);
        static void debug(string s, glm::vec2 v);
        static void debug(string s, glm::vec3 v);
        static void debug(string s, glm::vec4 v);
        static void debug(string s, glm::mat3 m);
        static void debug(string s, glm::mat4 m);
        static void checkGLError();


        /// EG_Utility_UniLoc.cpp
        static void setUniLoc(GLuint location, int value);
        static void setUniLoc(GLuint location, float value);
        static void setUniLoc(GLuint location, float x, float y);
        static void setUniLoc(GLuint location, glm::vec2 value);
        static void setUniLoc(GLuint location, glm::vec3 value);
        static void setUniLoc(GLuint location, glm::vec4 value);
        static void setUniLoc(GLuint location, glm::mat3 value);
        static void setUniLoc(GLuint location, glm::mat4 value);
        static void setUniLocTranspose(GLuint location, glm::mat4 value);


};



template<typename T>
vector<T> EG_Utility::reserveVector(int size)
{
    vector<T> v;
    v.reserve(size);
    return v;
}


#endif
