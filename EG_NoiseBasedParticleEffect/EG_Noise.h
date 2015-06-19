#ifndef EG_NOISE_H_
#define EG_NOISE_H_
#include "define.h"
#include "EG_Utility.h"
struct Noise3
{
    Noise3(unsigned int seed = 171717);
    void reinit(unsigned int seed);
    float operator()(float x, float y, float z)const;
    float operator()(const glm::vec3& x)const;

    protected:
        static const unsigned int n=128;
        glm::vec3 basis[n];
        int perm[n];
        unsigned int hash_index(int i, int j, int k) const;
};


struct FlowNoise3: public Noise3
{
    FlowNoise3(unsigned int seed=171717, float spin_variation=0.2);
    void set_time(float t);

    protected:
        glm::vec3 original_basis[n];
        float spin_rate[n];
        glm::vec3 spin_axis[n];
};

// transforms even the sequence 0,1,2,3,... into reasonably good random numbers
// challenge: improve on this in speed and "randomness"!
inline unsigned int randhash(unsigned int seed)
{
   unsigned int i=(seed^12345391u)*2654435769u;
   i^=(i<<6)^(i>>26);
   i*=2654435769u;
   i+=(i<<5)^(i>>12);
   return i;
}

// returns repeatable stateless pseudo-random number in [0,1]
inline double randhashd(unsigned int seed)
{
    return randhash(seed)/(double)UINT_MAX;
}

inline float randhashf(unsigned int seed)
{
    return randhash(seed)/(float)UINT_MAX;
}

inline double randhashd(unsigned int seed, double a, double b)
{
    return (b-a)*randhash(seed)/(double)UINT_MAX + a;
}

inline float randhashf(unsigned int seed, float a, float b)
{
    return ( (b-a)*randhash(seed)/(float)UINT_MAX + a);
}


#endif // EG_NOISE_H_
