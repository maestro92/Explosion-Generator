#include "particlesystem.h"
/*
particleSystem::particle::particle()
{
    lastUpdate = SDL_GetTicks();
}



particleSystem::particle::particle(const vector3d& pos,
                                    const vector3d& rot,
                                    const vector3d& col,
                                    const vector3d& sp,
                                    float s)
{
    position = pos;
    rotation = rot;
    color = col;

    size = s;
    speed = sp;
    lastUpdate = SDL_GetTicks();
    dead = false;
}


bool particleSystem::particle::update()
{
    // how much time has elapsed in seconds
    float deltaTime = (float)(SDL_GetTicks() - lastUpdate)/1000.0;


    position+=(speed*deltaTime);

    lifetime-=(SDL_GetTicks()-lastUpdate);
    lastUpdate = SDL_GetTicks();

    // ratio of time created and life
    float perc = (float)lifetime/created;
    color = startcolor*perc + endcolor*(1-perc);
    if(lifetime<=0)
    {
        dead = true;
        return true;
    }

    return false;
}


void particleSystem::particle::show(camera& cam)
{
    glColor4f(color.x/255.0, color.y/255.0, color.z/255.0, (float)lifetime/created);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(position.x, position.y, position.z);
        glTexCoord2f(1,0);
        glVertex3f(position.x, position.y + size, position.z);
        glTexCoord2f(1,1);
        glVertex3f(position.x + size, position.y + size, position.z);
        glTexCoord2f(0,1);
        glVertex3f(position.x + size, position.y, position.z);
    glEnd();

}


particleSystem::particleSystem(int partnum,vector3d p)
{
    particleNum = db;
    position = pos;
    minSize = ms;
    maxSize = mas;
    this->minSpeed=minSpeed;
    this->maxSpeed = maxSpeed;
    this->force = force;
    this->startColor = startColor;
    this->endColor = endcolor;
    minLifetime = minlt;
    isActive = true;
    if(tex == -1)
    {
        useTexture = false;
    }
    else
    {
        useTexture = true;
        texture = tex;
    }

    numparticles = nm;
    for(int i=0; i<db && numparticles > 0; i++)
    {
        particles.push_back(particle());
        generateParticle(particle[i]);
    }

}

particleSystem::~particleSystem()
{

}
void particleSystem::generateParticle(particle& p)
{
    p.position = position;
    p.size(float)(rand()%((int)((maxSize - minSize)*1000)))/

}
void particleSystem::setPosition(const vector3d& pos);
void particleSystem::setNumberOfParticles(int db);
void particleSystem::update();
void particleSystem::show(camera& cam)
{
    float f[16];
    glFloatv(GL_MODELVIEW_MATRIX,f);





}




bool particleSystem::isParticleSystemActive();
void setNumParticles(int n);
void setForce(const vector3d& f);
*/
