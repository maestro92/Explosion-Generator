#ifndef EG_NOISE_BASED_PARTICLE_H
#define EG_NOISE_BASED_PARTICLE_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include "define.h"


using namespace std;


struct EG_NoiseBasedParticle
{
    float px;
    float py;
    float pz;
    float timeOfBirth;
    float vx;
    float vy;
    float vz;
};

typedef std::vector<EG_NoiseBasedParticle> EG_NoiseBasedParticleList;


#endif
