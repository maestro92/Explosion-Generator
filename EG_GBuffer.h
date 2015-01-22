#ifndef _EG_GBUFFER_H_
#define _EG_GBUFFER_H_


#include "define.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>


#define GBUFFER_POSITION_TEXTURE_UNIT 0
#define GBUFFER_DIFFUSE_TEXTURE_UNIT  1
#define GBUFFER_NORMAL_TEXTURE_UNIT   2
#define GBUFFER_TEXCOORD_TEXTURE_UNIT 3



class EG_GBuffer
{
    public:
        enum GBUFFER_TEXTURE_TYPE {
            GBUFFER_TEXTURE_TYPE_POSITION,  // 0
            GBUFFER_TEXTURE_TYPE_DIFFUSE,   // 1
            GBUFFER_TEXTURE_TYPE_NORMAL,    // 2
            GBUFFER_TEXTURE_TYPE_TEXCOORD,  // 3
            GBUFFER_TEXTURE_TYPE_STENCIL,   // 4
            GBUFFER_TEXTURE_TYPE_DEPTH,     // 5
            GBUFFER_NUM_TEXTURES
        };


        EG_GBuffer();
        ~EG_GBuffer();

        void init(unsigned int width, unsigned int height);
        void bindForWriting();
        void bindForReading();
        void setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

        void bindForReadingMrtDemo();
        void bindForReadingLightPass();


        void bindForReading36();





        void init37(unsigned int width, unsigned int height);
        void StartFrame37();
        void BindForGeomPass37();
        void BindForStencilPass37();
        void BindForLightPass37();
        void BindForFinalPass37();




        GLuint get_depth_texture();


   // private:
        GLuint m_fbo;
        GLuint m_textures[GBUFFER_NUM_TEXTURES];
        GLuint m_depthTexture;
        GLuint m_finalTexture;


};


#endif
