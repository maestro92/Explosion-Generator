#ifndef TWOPASS1_TECHNIQUE_H
#define TWOPASS1_TECHNIQUE_H

#include "Smoke_struct.h"
#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "Technique.h"




class TwoPass_RayCasting_Technique
{
    public:
         // Matrix Locations
        struct Matrices_Location
        {
            GLuint ProjectionMatrix;
            GLuint ModelviewMatrix;
            GLuint ViewMatrix;
            GLuint ModelviewProjection;
            GLuint ModelMatrix;
            GLuint ViewNoRotateMatrix;
        };



        TwoPass_RayCasting_Technique();
        ~TwoPass_RayCasting_Technique();

        void init(int w, int h);
        void Load_glUniform(Matrices_Location& Mat_Loc, Matrices_t& Mat);

        void Render_TwoPass_RayCasting_1(Matrices_t& Mat);
        void Render_TwoPass_RayCasting_2(Matrices_t& Mat, GLuint depthTexture);
        void Render_TwoPass_RayCasting_CubeDepth(Matrices_t& Mat, GLuint fbo);


        void Render_TwoPass_RayCasting_1_draft(Matrices_t& Mat);

        unsigned int createTexture(int w, int h, bool isDepth);

  //  private:
        GLuint IntervalsFbo1;   // BackFace
        GLuint IntervalsFbo2;   // FrontFace
        GLuint TextureFbo1;   // BackFace
        GLuint TextureFbo2;   // FrontFace
        GLuint TwoPass_CubeDepthTexture_Front;
        GLuint TwoPass_CubeDepthTexture_Back;


        Matrices_Location Matrices_Loc1;
        Matrices_Location Matrices_Loc2;
        Matrices_Location Matrices_CubeDepth;



        GLuint RayStartPoints_Location2;
        GLuint RayStopPoints_Location2;
        GLuint Depth_Location2;
        GLuint Depth_TwoPassLocation_Front2;
        GLuint Depth_TwoPassLocation_Back2;

        // Shaders
        shader* TwoPassIntervals;
        shader* TwoPassRaycast;
        shader* TwoPass_CubeDepth;

        void Init_Shader_GL_Location(unsigned int shaderID, Matrices_Location& Mat);

};

#endif
