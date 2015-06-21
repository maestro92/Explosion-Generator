#include "EG_NoiseBasedParticleEffect.h"



static const glm::vec3 SphereCenter(0,0,0);
static const float NoiseLengthScale[] = {0.4f, 0.23f, 0.11f};
static const float NoiseGain[] = {1.0f, 0.5f, 0.25f};
static const float PlumeCeiling(8);
static const float PlumeBase(-3);
static const float PlumeHeight(8);
static const float RingRadius(0.6f);
static const float RingSpeed(0.3f);
static const float RingsPerSecond(0.1f);
static const float RingMagnitude(7);
static const float RingFalloff(0.7f);
static const float ParticlesPerSecond(4000);
static const float SeedRadius(0.5f);
static const float InitialBand(0.1f);

static float Time = 0;
static unsigned int Seed(0);


EG_NoiseBasedParticleEffect::EG_NoiseBasedParticleEffect()
{
    m_sphereCenter = glm::vec3(0.0,0.0,0.0);
    m_sphereRadius = 1.0f;
}

EG_NoiseBasedParticleEffect::~EG_NoiseBasedParticleEffect()
{

}



void EG_NoiseBasedParticleEffect::init(int width, int height)
{
    m_backgroundSurface = createSurface(width, height);
    m_particleSurface = createSurface(width, height);

    m_backgroundTexture = EG_Utility::loadTexture("Assets/Images/Scroll.png");
    m_spriteTexture = EG_Utility::loadTexture("Assets/Images/Sprite.png");

    m_screenQuad.init();
}




EG_SurfacePod EG_NoiseBasedParticleEffect::createSurface(int width, int height)
{
    EG_SurfacePod pod;

    pod.depthTexture = EG_Utility::createDepthTexture(width, height);

    // Create a FBO and attach the depth texture:
    glGenFramebuffers(1, &pod.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pod.FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pod.depthTexture, 0);

    pod.colorTexture = EG_Utility::createTexture(width, height);

    // Attach the color buffer:
    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pod.colorTexture, 0);


    return pod;




    /*
    EG_SurfacePod pod;

    /// Create a FBO
    // pod.FBO = EG_Utility::createFBO();
    glGenFramebuffers(1, &pod.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pod.FBO);

    /// Create a color texture
    pod.colorTexture = EG_Utility::createTexture(width, height);
    /// Attach the color buffer:
    glBindTexture(GL_TEXTURE_2D, pod.colorTexture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pod.colorTexture, 0);

    /// Create a depth texture:
    pod.depthTexture = EG_Utility::createDepthTexture(width, height);
    /// Attach the depth texture
    glBindTexture(GL_TEXTURE_2D, pod.depthTexture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, pod.depthTexture, 0);

    EG_Utility::errorCheckFBO();
    EG_Utility::errorCheck();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return pod;
*/
    /*
    EG_SurfacePod pod;

    /// Create a FBO
    pod.FBO = EG_Utility::createFBO();

    /// Create a depth texture:
    pod.depthTexture = EG_Utility::createDepthTexture(width, height);
    /// Attach the depth texture
    glBindFramebuffer(GL_FRAMEBUFFER, pod.FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, pod.depthTexture, 0);


    /// Create a color texture
    pod.colorTexture = EG_Utility::createTexture(width, height);

    /// Attach the color buffer:
    GLuint colorBuffer;
    glGenRenderbuffers(1, &colorBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pod.colorTexture, 0);

    EG_Utility::errorCheckFBO();
    EG_Utility::errorCheck();

    return pod;
    */
}


void EG_NoiseBasedParticleEffect::update(float dt, float timeStep)
{
    //float timestep = 0.0013f;

    EG_Utility::debug("New Iteration: ", m_particles.size());

    dt = 0.0030;
    timeStep = 0.016;

    Time += dt;

    for(int i=0; i<m_particles.size(); i++)
    {
        glm::vec3 p(m_particles[i].px, m_particles[i].py, m_particles[i].pz);
        glm::vec3 v;
//        glm::vec3 v = computeCurl(p);



        if(i==0)
        {
            // EG_Utility::debug("position is ", p);
            // v = computeCurl2(p);
            v = computeCurl(p);
            EG_Utility::debug("position is ", p);
            EG_Utility::debug("velocity is ", v);
        }
        else
        {
            v = computeCurl(p);

        }


        glm::vec3 midx = p + 0.5f * timeStep * v;
        p += timeStep * computeCurl(midx);
        m_particles[i].px = p.x;
        m_particles[i].py = p.y;
        m_particles[i].pz = p.z;
        m_particles[i].vx = v.x;
        m_particles[i].vy = v.y;
        m_particles[i].vz = v.z;


    }

    for(EG_NoiseBasedParticleList::iterator i=m_particles.begin(); i!=m_particles.end();)
    {
        if(i->py > PlumeCeiling)
        {
            i = m_particles.erase(i);
            cout << "   erasing" << endl;
        }
        else
            i++;
    }

    if(Time < 0.1f)
        seedParticles(dt);
}

void EG_NoiseBasedParticleEffect::seedParticles(float dt)
{
    static float time = 0;
    time += dt;
    unsigned int num_new = (unsigned int) (time * ParticlesPerSecond);
    if(num_new == 0)
        return;


    EG_Utility::debug("num_new ", num_new);
    time = 0;
    m_particles.reserve(m_particles.size() + num_new);
    for(unsigned int i=0; i<num_new; i++)
    {
        float theta = randhashf(Seed++, 0, TWO_PI);
        float r = randhashf(Seed++, 0, SeedRadius);
        float y = randhashf(Seed++, 0, SeedRadius);
        EG_NoiseBasedParticle p;

        p.px = r * cos(theta);
        p.py = PlumeBase + y;
        p.pz = r * sin(theta) + 0.125f;
        p.timeOfBirth = Time;
        if(i==0)
        {
            EG_Utility::debug("new seed p is ", glm::vec3(p.px, p.py, p.pz));
        }

        m_particles.push_back(p);
    }

}

bool EG_NoiseBasedParticleEffect::ZOrderPredicate(const EG_NoiseBasedParticle& p0, const EG_NoiseBasedParticle& p1)
{
//    float z0 = (m_modelViewProjection * Point3(p0.Px, p0.Py, p0.Pz)).getZ();
 //   float z1 = (m_modelViewProjection * Point3(p1.Px, p1.Py, p1.Pz)).getZ();
    float z0 = 1;
    float z1 = 2;
    return z0 > z1;
}



glm::vec3 EG_NoiseBasedParticleEffect::computeGradient(glm::vec3 p)
{
    float e = 0.01f;
    glm::vec3 dx(e,0,0);
    glm::vec3 dy(0,e,0);
    glm::vec3 dz(0,0,e);

    float d =    sampleDistance(p);
    float dfdx = sampleDistance(p + dx) - d;
    float dfdy = sampleDistance(p + dy) - d;
    float dfdz = sampleDistance(p + dz) - d;

    glm::vec3 g = glm::normalize(glm::vec3(dfdx, dfdy, dfdz));
    return g;
}

glm::vec3 EG_NoiseBasedParticleEffect::computeGradient2(glm::vec3 p)
{
    float e = 0.01f;
    glm::vec3 dx(e,0,0);
    glm::vec3 dy(0,e,0);
    glm::vec3 dz(0,0,e);

    float d =    sampleDistance(p);
    float dfdx = sampleDistance(p + dx) - d;
    float dfdy = sampleDistance(p + dy) - d;
    float dfdz = sampleDistance2(p + dz) - d;

    EG_Utility::debug(" dfdx ", dfdx);
    EG_Utility::debug(" dfdy ", dfdy);
    EG_Utility::debug(" dfdz ", dfdz);

    glm::vec3 g = glm::normalize(glm::vec3(dfdx, dfdy, dfdz));
    return g;
}



glm::vec3 EG_NoiseBasedParticleEffect::computeCurl(glm::vec3 p)
{
    float e = 1e-4f;
    glm::vec3 dx(e,0,0);
    glm::vec3 dy(0,e,0);
    glm::vec3 dz(0,0,e);

    float x = samplePotential(p + dy)[2] - samplePotential(p - dy)[2]
            - samplePotential(p + dz)[1] + samplePotential(p - dz)[1];

    float y = samplePotential(p + dz)[0] - samplePotential(p - dz)[0]
            - samplePotential(p + dx)[2] + samplePotential(p - dx)[2];

    float z = samplePotential(p + dx)[1] - samplePotential(p - dx)[1]
            - samplePotential(p + dy)[0] + samplePotential(p - dy)[0];

    float numer = 1/(2*e);
    return EG_Utility::scaleGlmVec(glm::vec3(x,y,z), numer);

}


glm::vec3 EG_NoiseBasedParticleEffect::computeCurl2(glm::vec3 p)
{
    float e = 1e-4f;
    glm::vec3 dx(e,0,0);
    glm::vec3 dy(0,e,0);
    glm::vec3 dz(0,0,e);

    float x = samplePotential(p + dy)[2] - samplePotential(p - dy)[2]
            - samplePotential(p + dz)[1] + samplePotential(p - dz)[1];

    float y = samplePotential(p + dz)[0] - samplePotential(p - dz)[0]
            - samplePotential(p + dx)[2] + samplePotential(p - dx)[2];

//    float z = samplePotential2(p + dx)[1] - samplePotential2(p - dx)[1]
//            - samplePotential2(p + dy)[0] + samplePotential2(p - dy)[0];

    float z1 = samplePotential2(p + dx)[1];
    float z2 = samplePotential2(p - dx)[1];
    float z3 = samplePotential2(p + dy)[0];
    float z4 = samplePotential2(p - dy)[0];

    EG_Utility::debug(" z1 ", z1);
    EG_Utility::debug(" z2 ", z2);
    EG_Utility::debug(" z3 ", z3);
    EG_Utility::debug(" z4 ", z4);

    float z = z1-z2-z3+z4;



    EG_Utility::debug(" Potential is", glm::vec3(x,y,z));

    float numer = 1/(2*e);
    return EG_Utility::scaleGlmVec(glm::vec3(x,y,z), numer);

}

float EG_NoiseBasedParticleEffect::sampleDistance(glm::vec3 p)
{
    float phi = p.y;
    glm::vec3 u = p - m_sphereCenter;
    float d = glm::length(u);
    return d - m_sphereRadius;
}


float EG_NoiseBasedParticleEffect::sampleDistance2(glm::vec3 p)
{
    float phi = p.y;
    glm::vec3 u = p - m_sphereCenter;
    float d = glm::length(u);
    float output = d - m_sphereRadius;


    EG_Utility::debug(" phi", phi);
    EG_Utility::debug(" u", u);
    EG_Utility::debug(" d", d);
    EG_Utility::debug(" dist", output);

    return output;
}



glm::vec3 EG_NoiseBasedParticleEffect::samplePotential(glm::vec3 p)
{
    glm::vec3 psi(0,0,0);
    glm::vec3 gradient = computeGradient(p);

    glm::vec3 risingForce = glm::vec3(p.z,0,-p.x);

    float ring_y = PlumeCeiling;

    while(ring_y > PlumeBase)
    {

        float ry = p.y - ring_y;
        float rr = sqrt(p.x*p.x + p.z*p.z);

        float denom = sqr(rr-RingRadius) + sqr(rr+RingRadius) + sqr(ry) + RingFalloff;
        float rmag = RingMagnitude / denom;

        glm::vec3 rpsi = rmag * risingForce;
        psi+=rpsi;
        ring_y -= RingSpeed / RingsPerSecond;
    }
    return psi;
}



glm::vec3 EG_NoiseBasedParticleEffect::samplePotential2(glm::vec3 p)
{
    glm::vec3 psi(0,0,0);
    glm::vec3 gradient = computeGradient2(p);

    EG_Utility::debug(" ComputeGradient", gradient);

    glm::vec3 risingForce = glm::vec3(p.z,0,-p.x);

    float ring_y = PlumeCeiling;

    while(ring_y > PlumeBase)
    {

        float ry = p.y - ring_y;
        float rr = sqrt(p.x*p.x + p.z*p.z);

        float denom = sqr(rr-RingRadius) + sqr(rr+RingRadius) + sqr(ry) + RingFalloff;
        float rmag = RingMagnitude / denom;

        glm::vec3 rpsi = rmag * risingForce;
        psi+=rpsi;
        ring_y -= RingSpeed / RingsPerSecond;
    }

    return psi;
}
