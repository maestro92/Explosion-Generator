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

#include "EG_Technique.h"




class Technique_TwoPass_Raycasting : public Technique
{
    public:


        Technique_TwoPass_Raycasting();
        ~Technique_TwoPass_Raycasting();

        void init(int w, int h, int Shader_Num);

        void Load_glUniform(Matrices_Location& Mat_Loc, Matrices_t& Mat);

        void Render_TwoPass_RayCasting_1(Matrices_t& Mat);
        void Render_TwoPass_RayCasting_2(Matrices_t& Mat, GLuint depthTexture);
    //    void Render_TwoPass_RayCasting_CubeDepth(Matrices_t& Mat, GLuint fbo);

        unsigned int createTexture(int w, int h, bool isDepth);

        //  private:
        GLuint IntervalsFbo1;   // BackFace
        GLuint IntervalsFbo2;   // FrontFace
        GLuint TextureFbo1;   // BackFace
        GLuint TextureFbo2;   // FrontFace
        GLuint TwoPass_CubeDepthTexture_Front;
        GLuint TwoPass_CubeDepthTexture_Back;

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



};

#endif
