#ifndef EG_Utility_H
#define EG_Utility_H

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

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "define.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>


#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

using namespace std;


class EG_Utility
{
    public:
        EG_Utility();
        ~EG_Utility();

        static void errorCheck();

        /// texture related
        static SDL_Surface* loadRawImage(string filename);
        static SDL_Surface* loadSDLImage(string filename);

        static GLuint loadTexture(string filename);
        static GLuint createTexture(int w, int h);
        static GLuint createDepthTexture(int, int h);
        static GLuint createCubemapTexture();
        static GLuint createCubemapTexture(string* filenames);
        static void setTextureParameters(int w, int h, int format);
        static void setCubemapTextureParameters();


        template<typename T>
            static vector<T> reserveVector(int size);

        static glm::vec3 toGlmVec(aiVector3D& v2);
//        static glm::mat4 toGlmMat(aiMatrix3x3& m2);
        static glm::mat4 toGlmMat(aiMatrix3x3 m2);
        static glm::mat4 toGlmMat(aiMatrix4x4& m2);
        static glm::mat4 toGlmMat(const aiMatrix4x4& m2);
//        static glm::vec3 scaleGlmVec3(glm::vec3 v, float s);
//        static glm::vec3 scaleGlmVec3(glm::vec3 v, float s1, float s2, float s3);
        static glm::vec3 scaleGlmVec3(const glm::vec3 v, float s);
        static glm::vec3 scaleGlmVec3(const glm::vec3 v, float s1, float s2, float s3);

/*
        static void printGlmVec3(glm::vec3 v);
        static void printGlmVec3(string s, glm::vec3 v);
        static void printGlmMat(glm::mat4 m);
        static void printGlmMat(string s, glm::mat4 m);
*/
        static float DEGREE_TO_RADIAN;
        static float RADIAN_TO_DEGREE;

        static void printGlm(string s, glm::vec2 v);
        static void printGlm(string s, glm::mat4 m);
        static void printGlm(string s, glm::vec3 v);
    //    static GLuint GetUniformLocation(shader* s, const char* UniformName);

};



template<typename T>
vector<T> EG_Utility::reserveVector(int size)
{
    vector<T> v;
    v.reserve(size);
    return v;
}


#endif
