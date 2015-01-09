#ifndef EG_TECHNIQUE_SHADOW_RENDER_H
#define EG_TECHNIQUE_SHADOW_RENDER_H

#include "pipeline.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "EG_RenderTechnique.h"



/// LightPOV    render1
/// CameraPOV   render2
class Technique_Shadow_Render : public EG_RenderTechnique
{

        /// first pass for depth in Light's point of View
        /// Second pass in Camera's point of View


    public:

/*
        /// Matrix Locations
        Matrices_Location Matrices_Loc1;        /// first pass for depth in Light's point of View
        Matrices_Location Matrices_Loc2;        /// Second pass in Camera's point of View
        GLuint egLoc_ShadowMatrix1;    /// This is for the ShadowMatrix
        GLuint egLoc_ShadowMatrix2;    /// Bias Matrix * LightModelViewProjectionMatrix
        /// Bias Matrix being NDC coordinates to Texture Coordinates
*/

    //    Matrices_Location Matrices_Loc1;
    //    Matrices_Location Matrices_Loc2;

/*
        /// first pass for depth in Light's point of View
        /// Second pass in Camera's point of View
        Matrices_Location Matrices_UniLoc[2];
*/

        /// This is for the ShadowMatrix
        /// Bias Matrix * LightModelViewProjectionMatrix
        /// Bias Matrix being NDC coordinates to Texture Coordinates
        GLuint ShadowMatrix_UniLoc[2];

        Technique_Shadow_Render();
        ~Technique_Shadow_Render();


        GLuint LightMVPmatrix_UniLoc;
        GLuint shadowMap_UniLoc;
        GLuint LightPosition_MVmatrix_UniLoc;
        GLuint LightPosition_UniLoc;
        GLuint CameraPosition_UniLoc;



      //  void init(int w, int h);
        void init(int w, int h, int Shader_Num);
        void loadUniformLocations(pipeline& p, int RenderPassID);
   //     void Setup_Matrix_forRender(pipeline& p, int RenderPassID);
   //     void Setup_Matrix_forRender1(pipeline& p);
   //     void Setup_Matrix_forRender2(pipeline& p);

  //      void Setup_Matrix_forRender(pipeline& p, int RenderPassID);
  //      void Setup_ShadowMatrix_forRender(pipeline& p, int RenderPassID);
  //      void loadSpecialUniformLocation(pipeline& p, int RenderPassID);

        GLuint ShadowMap;
        GLuint DepthTexture_LightPOV;

        int ShadowMapWidth;
        int ShadowMapHeight;



        // Shaders
        Shader* FirstPass_LightPOV;
        Shader* SecondPass_CameraPOV;



};

#endif
