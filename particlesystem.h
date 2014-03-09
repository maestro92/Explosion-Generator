#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "vector3d.h"
#include "camera.h"
#include "SOIL.h"

/
#include <GL/gl.h>
#include <GL/glu.h>
//glm::vec2 nice = glm::vec2(0);

class particleSystem

{


    struct particle
    {
        vector3d position;
        vector3d rotation;
        vector3d speed;
        vector3d startcolor;
        vector3d endcolor;
        vector3d velocity;

        float size;
        uint32_t lastUpdate;
        vector3d color;

        // number of frames it can be alive
        int lifetime;
        int created;

        bool dead;

        public:
            particle();
            particle(const vector3d& pos,
                        const vector3d& rot,
                        const vector3d& direction,
                        const vector3d& sp,
                        float size);
        bool update();
        void show(Camera& cam);
    };



    public:
        std::vector<particle*> particles;
        int particleNum;
        vector3d pos;
        bool useTexture;
        unsigned int texture;
        uint32_t active;
        bool isActive;
        vector3d startColor, endColor, force;
        float minSize, maxSize, minSpeed, maxSpeed;
        int minLifetime, max,Lifetime;
        int numparticles;


	public:
        particleSystem(int partnum,vector3d p);
        ~particleSystem();
        void generateParticle(particle& p);
        void setPosition(const vector3d& pos);
        void setNumberOfParticles(int db);
        void update();
        void show(Camera& cam);
        bool isParticleSystemActive();
        void setNumParticles(int n);
        void setForce(const vector3d& f);



};
#endif
