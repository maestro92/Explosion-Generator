#include "EG_Emitter.h"

EG_Emitter::EG_Emitter()
{
    ///
    /// 30 particles per second seems good
    m_prevTime = -1;
    m_particleSprite = -1;

    m_emissionRate = 50.0f;
    m_emissionRadius = 0.0f;

    m_life = 2.0f;
    m_lifeRange = 0.5f;

    m_size = 5.0f;
    m_sizeRange = 2.0f;

    m_saturation = 1.0f;
    m_alpha = 0.5;

    m_spread = 0.2f;
    m_gravity = 0.0f;
}

EG_Emitter::~EG_Emitter()
{
    for(list<EG_EmitterParticle*>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
        delete (*it);

 //   for(int i=0; i<m_particles.size(); i++)
 //       delete m_particles[i];

}

void EG_Emitter::init()
{
    m_particleSprite = EG_Utility::loadTexture("Assets/Images/fire sprite.jpg");
//    m_quad.init(-0.5, 0.5, 0.0, 1.0,
//                 0.0, 0.0, 1.0);

    m_quad.initCentered();
    // m_quad.init1(1, 1, 0, 0, 1);


    initRenderer();
}


void EG_Emitter::initRenderer()
{
    Shader* s = new Shader("EG_ParticleSystemShaders/EG_FireShader.vs",
                           "EG_ParticleSystemShaders/EG_FireShader.gs",
                           "EG_ParticleSystemShaders/EG_FireShader.fs");
    m_renderer.addShader(s);

    m_renderer.addDataPair(RENDER_PASS1, "u_modelViewProjMat",      DP_MAT4);
    m_renderer.addDataPair(RENDER_PASS1, "u_cameraPosition",        DP_VEC3);
    m_renderer.addDataPair(RENDER_PASS1, "u_cameraViewDir",         DP_VEC3);
    m_renderer.addDataPair(RENDER_PASS1, "u_texture",               DP_INT);


    s = new Shader("EG_ParticleSystemShaders/EG_FireShaderDraft.vs",
                   "EG_ParticleSystemShaders/EG_FireShaderDraft.fs");
    m_renderer.addShader(s);

    m_renderer.addDataPair(RENDER_PASS2, "u_texture",       DP_INT);
    m_renderer.addDataPair(RENDER_PASS2, "u_sceneDepthTexture", DP_INT);
    m_renderer.addDataPair(RENDER_PASS2, "u_mvp",           DP_MAT4);
    m_renderer.addDataPair(RENDER_PASS2, "u_alpha",         DP_FLOAT);
    m_renderer.addDataPair(RENDER_PASS2, "u_inverseSize",   DP_VEC2);
}


void EG_Emitter::update(long time)
{
    /// we'll receive the SDL_GetTicks(), which is the number of milliseconds since the initialization of SDL library

    /// without texture, we leave immediately
    if(m_particleSprite == -1)
        return;

    if(m_prevTime == -1)
        m_prevTime = time;

    /// i'm first converting (time-m_prevTime) from milliseconds to seconds
    /// then times emission rate, which is the number of seconds i'm emitting
    int numEmission = (int) ( (float) (time-m_prevTime) / 1000.0f * m_emissionRate );

    /// emit the particles
    for (int i=0; i<numEmission; i++)
        addParticle();

    /// this is to prevent cases if we're looping way to fast, and numEmission is simply giving us zero all the time
    /// we'll wait for a few loops, then change
    if(numEmission > 0)
        m_prevTime = time;

    m_curTime = time;




    for(list<EG_EmitterParticle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
    {
        EG_EmitterParticle* particle = (*it);
        particle->m_acceleration.y = -m_gravity;
        particle->m_acceleration += m_wind;
        particle->m_alpha = m_alpha;
        particle->m_rotation = m_rotation;

        particle->update(m_curTime);

        if(particle->m_active == false)
        {
            delete particle;
            list<EG_EmitterParticle*>::iterator pTemp = it--;
            m_particles.erase(pTemp);
        }
    }
}



void EG_Emitter::addParticle()
{
    EG_EmitterParticle* particle = new EG_EmitterParticle((int)m_particles.size());

    /// m_saturation is 1.0 by default
    /// if m_saturation == 1, the term 1-m_saturation == 0
    float r = EG_Utility::randFloat() * m_saturation + (1-m_saturation);
    float g = EG_Utility::randFloat() * m_saturation + (1-m_saturation);
    float b = EG_Utility::randFloat() * m_saturation + (1-m_saturation);

    particle->m_color = glm::vec3(r,g,b);

    particle->m_life = EG_Utility::randFloat(m_life - m_lifeRange, m_life + m_lifeRange);
    particle->m_totalLife = particle->m_life;

    float vx = EG_Utility::randFloat(-m_spread, m_spread);
    float vy = EG_Utility::randFloat(10,12);
    float vz = EG_Utility::randFloat(-m_spread, m_spread);
    particle->m_velocity = glm::vec3(vx, vy, vz);

    particle->m_acceleration = glm::vec3(0.0f, -m_gravity, 0.0f);

    particle->m_position.x = EG_Utility::randFloat(-m_emissionRadius, m_emissionRadius);
    particle->m_position.z = EG_Utility::randFloat(-m_emissionRadius, m_emissionRadius);

    m_particles.push_back(particle);
}

/// add
/// http://www.andersriggelsen.dk/glblendfunc.php
void EG_Emitter::render(pipeline& p, GLuint sceneDepthTexture)
{
    glEnable(GL_BLEND);
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_particleSprite);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sceneDepthTexture);

    m_renderer.enableShader(RENDER_PASS2);
    p.pushMatrix();
    for(list<EG_EmitterParticle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
    {

        EG_EmitterParticle* particle = (*it);
        p.pushMatrix();
            p.translate(particle->m_position);
            p.scale(5);

/*
            if(particle->m_id % 5 == 0)
                p.rotateZ(90.0f);
            else
                p.rotateZ(-90.0f);
*/


            glm::mat4 mvp = p.getModelViewMatrix();

            for(int i=0; i<3; i++)
            {
                for(int j=0; j<3; j++)
                {
                    if(i == j)
                        mvp[i][j] = 5.0f;
                    else
                        mvp[i][j] = 0.0f;
                }
            }


            if(particle->m_id % 5 == 0)
                mvp = mvp * glm::rotate(particle->m_life * 100.0f, 0.0f, 0.0f, 1.0f);
            else
                mvp = mvp * glm::rotate(particle->m_life * -100.0f, 0.0f, 0.0f, 1.0f);

          //  EG_Utility::debug("mat is", mvp);
/*
            if(particle->m_id % 5 == 0)
                mvp = mvp * glm::rotate(particle->m_life * 100.0f, 0.0f, 0.0f, 1.0f);
            else
                mvp = mvp * glm::rotate(particle->m_life * -100.0f, 0.0f, 0.0f, 1.0f);
*/
            mvp = p.getProjectionMatrix() * mvp;

            m_renderer.setData(RENDER_PASS2, "u_texture", 0);
            m_renderer.setData(RENDER_PASS2, "u_sceneDepthTexture", 1);
            m_renderer.setData(RENDER_PASS2, "u_mvp", mvp);
            m_renderer.setData(RENDER_PASS2, "u_alpha", particle->m_alpha);
            m_renderer.setData(RENDER_PASS2, "u_inverseSize", glm::vec2(glm::vec2(1.0f/800.0f, 1.0f/600.0f)));

            m_renderer.loadUniformLocations(p, RENDER_PASS2);
            m_quad.render();
        p.popMatrix();

    }
    p.popMatrix();
    m_renderer.disableShader(RENDER_PASS2);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);


    /*
    for(int i=0; i<m_particles.size(); i++)
    {
        EG_EmitterParticle* particle = m_particles[i];

        particle->m_acceleration.y = -m_gravity;
        particle->m_acceleration += m_wind;
        particle->m_alpha = m_alpha;
        particle->m_rotation = m_rotation;

        particle->update(m_curTime);

        if(m_particles[i])
    }
    */
}











