#include "EG_ParticleSystemEffect.h"

EG_ParticleSystemEffect::EG_ParticleSystemEffect()
{
    m_currVB = 0;   // current Vertex Buffer
    m_currTFB = 1;  // current transform vertex buffer
    m_isFirst = true;
    m_time = 0;

}


EG_ParticleSystemEffect::~EG_ParticleSystemEffect()
{
}


void EG_ParticleSystemEffect::init()
{
    int size = 1000;

    glm::vec3* randomData = new glm::vec3[size];
    for (unsigned int i = 0 ; i < size ; i++)
    {
        randomData[i].x = EG_Utility::randFloat();
        randomData[i].y = EG_Utility::randFloat();
        randomData[i].z = EG_Utility::randFloat();
    }

    glGenTextures(1, &m_randomTexture);
    glBindTexture(GL_TEXTURE_1D, m_randomTexture);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, randomData);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    delete [] randomData;


    // check OpenGL error
    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR)
    {
        cout << "random texture error: " << err << endl;
    }
    cout << "random texture very nice" << endl;



    m_particleTexture = EG_Utility::loadTexture("Assets/Images/fireworks_red.jpg");






    EG_ParticleSystemParticle particles[MAX_PARTICLES];
    ZERO_MEM(particles);

    particles[0].type = PARTICLE_TYPE_LAUNCHER;
    particles[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
    particles[0].velocity = glm::vec3(0.0, 0.0001f, 0.0f);
    particles[0].lifeTimeMillis = 0.0f;

    // making two transform feedback
    // we create two transformfeedback objects by calling glGenTransformFeedbacks
    glGenTransformFeedbacks(2, m_transformFeedbacks);

    // we create two buffers objects, one for each transform feedback objects
    glGenBuffers(2, m_particleBuffers);


    for (unsigned int i = 0; i < 2 ; i++)
    {
        // we start by binding GL_Transform_Feedback target to the transformfeedback object
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedbacks[i]);

        // Next we bind the the corresponding buffer object to the GL_ARRAY_BUFFER
        // that makes it a regular vertex buffer and load the contents of the particle array into it.
        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);

        // This makes this buffer a transform feedback buffer and places it as index zero
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffers[i]);
    }




}


void EG_ParticleSystemEffect::update()
{
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, m_randomTexture);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[m_currVB]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedbacks[m_currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), 0);                          // type
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)4);         // position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)16);        // velocity
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)28);          // lifetime

    glBeginTransformFeedback(GL_POINTS);

    // first drawing function, since we know we're drawing one point
    if (m_isFirst)
    {
        glDrawArrays(GL_POINTS, 0, 1);
        m_isFirst = false;
    }
    else
    {
        glDrawTransformFeedback(GL_POINTS, m_transformFeedbacks[m_currVB]);
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glDisable(GL_RASTERIZER_DISCARD);
}


void EG_ParticleSystemEffect::render(pipeline& p, EG_Renderer* Renderer, int pass)
{
    glDisable(GL_CULL_FACE);
    Renderer->enableShader(pass);
    p.pushMatrix();

        Renderer->loadUniformLocations(p, pass);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_particleTexture);

        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[m_currTFB]);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)4);  // position

        glDrawTransformFeedback(GL_POINTS, m_transformFeedbacks[m_currTFB]);

        glDisableVertexAttribArray(0);

    p.popMatrix();
    Renderer->disableShader(pass);

    m_currVB = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;
}




void EG_ParticleSystemEffect::render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_particleTexture);


    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[m_currTFB]);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)4);  // position

    glDrawTransformFeedback(GL_POINTS, m_transformFeedbacks[m_currTFB]);

    glDisableVertexAttribArray(0);


    m_currVB = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;
}

