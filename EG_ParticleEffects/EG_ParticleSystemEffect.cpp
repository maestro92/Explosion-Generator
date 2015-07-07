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

    /// making two transform feedback
    /// we create two transformfeedback objects by calling glGenTransformFeedbacks
    /// the transformfeedback objects encapsulates all the state that is attached to the transform feedback object
    glGenTransformFeedbacks(2, m_transformFeedbackObjects);

    /// we create two buffers objects, one for each transform feedback objects
    glGenBuffers(2, m_particleBuffers);


    for (unsigned int i = 0; i < 2 ; i++)
    {
        /// http://www.twodee.org/blog/?p=7451

        /// we first setup our buffers and pass in data
        /// GL_STREAM_DRAW vs GL_DYNAMIC_DRAW

/*
https://www.opengl.org/discussion_boards/showthread.php/151643-STREAM_DRAW-vs-DYNAMIC_DRAW
Stream means you are going to create it once, set it once, and use it once.
Dynamic means you are going to create it once, change it a lot, and use it a lot.
Static means you are going to create it once, set it once, and use it a lot.
But, these are just usage hints to the driver to tell it how you are going to be using the buffer. The driver is free to implement them the same way.
*/
        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);


        /// we start by binding GL_Transform_Feedback target to the transformfeedback object
        /// this transformfeedback object will have all the following operation (releavant to transform feedback) bind to it
        /// meaning the m_particleBuffer[i] below is attached to this m_transformfeedback object
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedbackObjects[i]);


        /// We also bind the buffer object to the GL_TRANSFORM_FEEDBACK_BUFFER target
        /// This makes this buffer a transform feedback buffer and places it as index zero
        /// so now the m_particleBuffers[i], which is initially a regular buffer
        /// is now attacked the current transformFeedback object

        /// also that these regular buffer objects are also TransfromFeedBack buffers
        /// so the m_particleBuffers are both vertex input buffers and transform feedback buffers
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffers[i]);

        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    }
}


void EG_ParticleSystemEffect::update()
{
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, m_randomTexture);

    glEnable(GL_RASTERIZER_DISCARD);

    /// input vertexBuffer in the update stage
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[m_currVB]);
    /// we're binding transformfeed back that we want to act on, (since we want to draw into it)
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedbackObjects[m_currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), 0);                          // type
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)4);         // position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)16);        // velocity
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)28);          // lifetime

    /// this makes the transform feedback active
    /// so we're writing it to the feedback buffer
    glBeginTransformFeedback(GL_POINTS);

    /// first drawing function, (we're drawing into m_transfromFeedbackObjects[m_currTFB]
    /// since we know we're drawing one point
    if (m_isFirst)
    {
        glDrawArrays(GL_POINTS, 0, 1);
        m_isFirst = false;
    }
    else
    {
        /// Specifies the name of a transform feedback object from which to retrieve a primitive count
        glDrawTransformFeedback(GL_POINTS, m_transformFeedbackObjects[m_currVB]);
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glDisable(GL_RASTERIZER_DISCARD);
}


void EG_ParticleSystemEffect::render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_particleTexture);

    /// we first write our stuff into the m_transformFeedbackObject[m_currTFB] transform feedback object
    /// that object has m_particleBuffer[m_currTFB] as the attached vertex buffer.
    /// we now bind this buffer to provide the input vertices for rendering
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[m_currTFB]);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)4);  /// "Type" was first, so 4 offset is position

    glDrawTransformFeedback(GL_POINTS, m_transformFeedbackObjects[m_currTFB]);

    glDisableVertexAttribArray(0);

    m_currVB = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;
}










void EG_ParticleSystemEffect::initCurlNoiseStyle()
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

    /// we create two buffers objects, one for each transform feedback objects
    glGenBuffers(2, m_particleBuffers);


    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void EG_ParticleSystemEffect::updateCurlNoiseStyle()
{
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, m_randomTexture);

    glEnable(GL_RASTERIZER_DISCARD);

    /// input vertexBuffer in the update stage
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[0]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), 0);                          // type
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)4);         // position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)16);        // velocity
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)28);          // lifetime

    /// this sets up the output TransformFeed Buffer
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffers[1]);

    /// this makes the transform feedback active
    /// so we're writing it to the feedback buffer
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glDisable(GL_RASTERIZER_DISCARD);
    std::swap(m_particleBuffers[0], m_particleBuffers[1]);
}



void EG_ParticleSystemEffect::renderCurlNoiseStyle()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_particleTexture);

    /// we first write our stuff into the m_transformFeedbackObject[m_currTFB] transform feedback object
    /// that object has m_particleBuffer[m_currTFB] as the attached vertex buffer.
    /// we now bind this buffer to provide the input vertices for rendering
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[0]);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)4);  /// "Type" was first, so 4 offset is position

 //   glDrawTransformFeedback(GL_POINTS, m_transformFeedbackObjects[m_currTFB]);
    glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

    glDisableVertexAttribArray(0);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EG_ParticleSystemParticle), (const GLvoid*)4);  /// "Type" was first, so 4 offset is position

        glDrawTransformFeedback(GL_POINTS, m_transformFeedbackObjects[m_currTFB]);

        glDisableVertexAttribArray(0);

    p.popMatrix();
    Renderer->disableShader(pass);

    m_currVB = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;
}




