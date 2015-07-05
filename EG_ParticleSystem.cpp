
//#include "engine_common.h"
//#include "ogldev_util.h"
#include "EG_ParticleSystem.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f




EG_ParticleSystem::EG_ParticleSystem()
{
    m_currVB = 0;   // current Vertex Buffer
    m_currTFB = 1;  // current transform vertex buffer
    m_isFirst = true;
    m_time = 0;
    PS_texture = NULL;

    ZERO_MEM(m_transformFeedback);
    ZERO_MEM(m_particleBuffer);
}


EG_ParticleSystem::~EG_ParticleSystem()
{
    if (m_transformFeedback[0] != 0) {
        glDeleteTransformFeedbacks(2, m_transformFeedback);
    }

    if (m_particleBuffer[0] != 0) {
        glDeleteBuffers(2, m_particleBuffer);
    }
}


bool EG_ParticleSystem::InitParticleSystem(glm::vec3 Pos)
{
    ParticlePS Particles[MAX_PARTICLES];
    ZERO_MEM(Particles);

    // we set the first particle as a Laucher
    Particles[0].Type = PARTICLE_TYPE_LAUNCHER;
    Particles[0].Pos = glm::vec3(0.0f, 0.0f, 0.0f);
    Particles[0].Vel = glm::vec3(0.0f, 0.0001f, 0.0f);
    Particles[0].LifetimeMillis = 0.0f;

    // making two transform feedback
    // we create two transformfeedback objects by calling glGenTransformFeedbacks
    glGenTransformFeedbacks(2, m_transformFeedback);

    // we create two buffers objects, one for each transform feedback objects
    glGenBuffers(2, m_particleBuffer);

    for (unsigned int i = 0; i < 2 ; i++)
    {
        // we start by binding GL_Transform_Feedback target to the transformfeedback object
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);

        // Next we bind the the corresponding buffer object to the GL_ARRAY_BUFFER
        // that makes it a regular vertex buffer and load the contents of the particle array into it.
        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), Particles, GL_DYNAMIC_DRAW);

        // This makes this buffer a transform feedback buffer and places it as index zero
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);
    }




    glm::vec3* pRandomData = new glm::vec3[1000];
    for (unsigned int i = 0 ; i < 1000 ; i++)
    {
        pRandomData[i].x = EG_Utility::randFloat();
        pRandomData[i].y = EG_Utility::randFloat();
        pRandomData[i].z = EG_Utility::randFloat();
    }

    glGenTextures(1, &m_randomTexture);
    glBindTexture(GL_TEXTURE_1D, m_randomTexture);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 1000, 0, GL_RGB, GL_FLOAT, pRandomData);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    delete [] pRandomData;


    // check OpenGL error
    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR)
    {
        cout << "random texture error: " << err << endl;
    }
    cout << "random texture very nice" << endl;
}







bool EG_ParticleSystem::Init_Texture(const std::string& TexFilename)
{

    glGenTextures(1,&PS_texture);
    SDL_Surface* img = IMG_Load(TexFilename.c_str());
    if(img == NULL)
    {
        cout << "Loadtexture img failed" << endl;
        exit(1);
        return -1;
    }

    SDL_PixelFormat pixel_format = {NULL,
                                    32,             // converting to 32 bit pixel
                                    4,              // number of bytes
                                    0,0,0,0,        // byte lost
                                    0,0,0,0,        // byte shift
                                    0xff000000,     // red mask
                                    0x00ff0000,     // green mask
                                    0x0000ff00,     // blue mask
                                    0x000000ff,     // alpha mask
                                    0,              // color code
                                    255};           // alpha code

    // SDL_SWSURFACE means it's a software surface so we don't store it
    // in the video card
    // we're converting im2 to into this new SDL_PixelFormat format
    SDL_Surface* img2 = SDL_ConvertSurface(img, &pixel_format ,SDL_SWSURFACE);
    if(img2 == NULL)
    {
        cout << "img2 was not loaded" << std :: endl;
        exit(1);
        return -1;
    }


    // tell OpenGL we want to use this texture and the texture type/target this texture is
    glBindTexture(GL_TEXTURE_2D, PS_texture);       //and use the texture, we have just generated

    // gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,img->w,img->h,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);
    // no else
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);        //we make the actual texture
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_BGR,GL_UNSIGNED_BYTE,img->pixels);        //we make the actual texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(img);   //we delete the image, we don't need it anymore
    SDL_FreeSurface(img2);

    return true;

}



void EG_ParticleSystem::UpdateParticles()
{
 //   m_randomTexture.Bind(RANDOM_TEXTURE_UNIT);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, m_randomTexture);

// We have another draw call later on that does that.
// Calling glEnable() with the GL_RASTERIZER_DISCARD flag
// tells the pipeline to discard all primitives before
// they reach the rasterizer (but after the optional transform feedback stage).
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(ParticlePS), 0);                          // type
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticlePS), (const GLvoid*)4);         // position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ParticlePS), (const GLvoid*)16);        // velocity
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticlePS), (const GLvoid*)28);          // lifetime

    glBeginTransformFeedback(GL_POINTS);

    // first drawing function, since we know we're drawing one point
    if (m_isFirst)
    {
        glDrawArrays(GL_POINTS, 0, 1);
        m_isFirst = false;
    }
    else
    {
        glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currVB]);
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glDisable(GL_RASTERIZER_DISCARD);
}






void EG_ParticleSystem::Render()
{
    RenderParticles();

    m_currVB = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;

}




void EG_ParticleSystem::RenderParticles()
{

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, PS_texture);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticlePS), (const GLvoid*)4);  // position

    glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

    glDisableVertexAttribArray(0);
}
