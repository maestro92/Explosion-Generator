#include "EG_NoiseBasedParticleEffect.h"



static const glm::vec3 SphereCenter(0,0,0);
static const float NoiseLengthScale[] = {0.4f, 0.23f, 0.11f};
static const float NoiseGain[] = {1.0f, 0.5f, 0.25f};
//static const float PlumeCeiling(8);
static const float PlumeCeiling(4);
static const float PlumeBase(0);
static const float PlumeHeight(16);
static const float RingRadius(0.1f);
static const float RingSpeed(0.1f);
static const float RingsPerSecond(0.1f);
static const float RingMagnitude(7);
static const float RingFalloff(0.7f);
static const float ParticlesPerSecond(4000);
static const float SeedRadius(0.5f);
static const float InitialBand(0.1f);


// static Particle Particles[ParticleCount] = {0};
static EG_NoiseBasedParticle m_GPUParticles[ParticleCount] = {0};
static EG_VelocityCache VelocityCache;

// static float Time = 0;
static unsigned int Seed(0);



EG_NoiseBasedParticleEffect::EG_NoiseBasedParticleEffect()
{
    m_sphereCenter = glm::vec3(0.0,0.0,0.0);
    m_sphereRadius = 1.0f;
    Time = 0;

    m_GPUParticles[ParticleCount] = {0};
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
        glm::vec3 v = computeCurl(p);

        if(i==0)
        {
            // EG_Utility::debug("position is ", p);
            // v = computeCurl2(p);
        //    v = computeCurl(p);
            EG_Utility::debug("position is ", p);
            EG_Utility::debug("velocity is ", v);
        }
        else
        {
       //     v = computeCurl(p);

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

    seedParticles(dt);
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



void EG_NoiseBasedParticleEffect::seedParticles(float dt)
{
 //   static float time = 0;
 //   time += dt;
    unsigned int num_new = (unsigned int) (dt * ParticlesPerSecond);
    if(num_new == 0)
        return;


    EG_Utility::debug("num_new ", num_new);
 //   time = 0;
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

/*
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
*/


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

    float ring_y = RingSpeed * 0.013;

    while(ring_y > 0)
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

/*

glm::vec3 EG_NoiseBasedParticleEffect::samplePotential2(glm::vec3 p)
{
    glm::vec3 psi(0,0,0);
    glm::vec3 gradient = computeGradient2(p);

    EG_Utility::debug(" ComputeGradient", gradient);

    glm::vec3 risingForce = glm::vec3(p.z,0,-p.x);

    float ring_y = RingSpeed * 0.013;

    while(ring_y > 0)
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
*/



void EG_NoiseBasedParticleEffect::initGPU(int width, int height)
{
    m_backgroundSurface = createSurface(width, height);
    m_particleSurface = createSurface(width, height);

    m_backgroundTexture = EG_Utility::loadTexture("Assets/Images/Scroll.png");
    m_spriteTexture = EG_Utility::loadTexture("Assets/Images/Sprite.png");

    m_screenQuad.init();

    glGenBuffers(1, &m_particleBuffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_GPUParticles), m_GPUParticles, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_particleBuffers[1]);
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_GPUParticles), 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_velocityTexture = createVelocityTexture(32, 64, 32);
}

void EG_NoiseBasedParticleEffect::updateGPU()
{
    float dt = 0.0030;
    float timeStep = 0.016;

    Time += dt;

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[0]);
    glEnableVertexAttribArray(SlotPosition);
    glEnableVertexAttribArray(SlotBirthTime);
    glEnableVertexAttribArray(SlotVelocity);
    unsigned char* pData = 0;
    glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), pData);
    glVertexAttribPointer(SlotBirthTime, 1, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), 12 + pData);
    glVertexAttribPointer(SlotVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(EG_NoiseBasedParticle), 16 + pData);

    /// specify the target buffer;
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffers[1]);

    /// draw it
    glBeginTransformFeedback(GL_POINTS);
    glBindTexture(GL_TEXTURE_3D, m_velocityTexture.id);
    glDrawArrays(GL_POINTS, 0, ParticleCount);

    /// restore
    glEndTransformFeedback();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(SlotPosition);
    glDisableVertexAttribArray(SlotBirthTime);
    glDisableVertexAttribArray(SlotVelocity);
    glDisable(GL_RASTERIZER_DISCARD);
    std::swap(m_particleBuffers[0], m_particleBuffers[1]);
}



// EG_TexturePod EG_NoiseBasedParticleEffect::createVelocityTexture(int width, int height, int depth, void(*progress)(int))
EG_TexturePod EG_NoiseBasedParticleEffect::createVelocityTexture(int width, int height, int depth)
{
    VelocityCache.data.resize(width * height * depth * 3);

    const float w = 2.0f;
    const float h = w * height / width;
    const float d = w;

    size_t requiredBytes = VelocityCache.data.size() * sizeof(float);
    FILE* voxelsFile = fopen("Velocity.dat", "rb");
    if(voxelsFile)
    {
        size_t bytesRead = fread(&VelocityCache.data[0], 1, requiredBytes, voxelsFile);
        if(bytesRead == requiredBytes)
        {
            EG_Utility::debug("Error in createVelocityTexture");
        }
    }
    else
    {
        vector<float>::iterator pData = VelocityCache.data.begin();
        for(int slice=0; slice<depth; slice++)
        {
            for(int row=0; row < height; row++)
            {
                for(int col=0; col < width; col++)
                {
                    glm::vec3 p;
                    p.x = -w + 2 * w * col / width;
                    p.y = -h + 2 * h * row / height;
                    p.z = -d + 2 * d * slice / depth;
                    glm::vec3 v = computeCurl(p);
                    *pData++ = v.x;
                    *pData++ = v.y;
                    *pData++ = v.z;
                }
            }
     //       progress(depth-1-slice);
        }

        voxelsFile = fopen("Velocity.dat", "wb");
        int bytesWritten = fwrite(&VelocityCache.data[0], 1, requiredBytes, voxelsFile);
    }
    fclose(voxelsFile);

    GLuint textureID;
    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F, width, height, depth, 0, GL_RGB, GL_FLOAT, &VelocityCache.data[0]);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    VelocityCache.description.id = textureID;
    VelocityCache.description.width = width;
    VelocityCache.description.height = height;
    VelocityCache.description.depth = depth;

    return VelocityCache.description;
}


