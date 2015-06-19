#include "EG_Noise.h"

template<class S,class T>
inline S lerp(const S &value0, const S &value1, T f)
{
    return (1-f)*value0 + f*value1;
}

template<class S,class T>
inline S bilerp(const S &v00, const S &v10,
                const S &v01, const S &v11,
                T fx, T fy)
{
   return lerp(
               lerp(v00, v10, fx),
               lerp(v01, v11, fx),
               fy);
}


template<class S,class T>
inline S trilerp(
   const S& v000, const S& v100,
   const S& v010, const S& v110,
   const S& v001, const S& v101,
   const S& v011, const S& v111,
   T fx, T fy, T fz)
{
   return lerp(
            bilerp(v000, v100, v010, v110, fx, fy),
            bilerp(v001, v101, v011, v111, fx, fy),
            fz);
}


static glm::vec3 sample_sphere(unsigned int& seed)
{
    glm::vec3 v;
    float m2;
    while(m2>1 || m2==0)
    {
        v.x = randhashf(seed++,-1,1);
        v.y = randhashf(seed++,-1,1);
        v.z = randhashf(seed++,-1,1);

        float l = glm::length(v);
        m2 = l*l;
    }

    v = EG_Utility::scaleGlmVec(v, sqrt(m2));
    return v;
}


Noise3::Noise3(unsigned int seed)
{
    for(unsigned int i=0; i<n; i++)
    {
        basis[i] = sample_sphere(seed);
        perm[i] = i;
    }
    reinit(seed);
}


void Noise3::reinit(unsigned int seed)
{
    for(unsigned int i=1; i<n; i++)
    {
        int j = randhash(seed++)%(i+1);
        swap(perm[i], perm[j]);
    }
}


float Noise3::operator()(const glm::vec3& x) const
{
    return (*this)(x[0], x[1], x[2]);
}


float Noise3::operator()(float x, float y, float z) const
{
   float floorx=std::floor(x), floory=std::floor(y), floorz=std::floor(z);
   int i=(int)floorx, j=(int)floory, k=(int)floorz;
   const glm::vec3 &n000=basis[hash_index(i,j,k)];
   const glm::vec3 &n100=basis[hash_index(i+1,j,k)];
   const glm::vec3 &n010=basis[hash_index(i,j+1,k)];
   const glm::vec3 &n110=basis[hash_index(i+1,j+1,k)];
   const glm::vec3 &n001=basis[hash_index(i,j,k+1)];
   const glm::vec3 &n101=basis[hash_index(i+1,j,k+1)];
   const glm::vec3 &n011=basis[hash_index(i,j+1,k+1)];
   const glm::vec3 &n111=basis[hash_index(i+1,j+1,k+1)];

   float fx=x-floorx, fy=y-floory, fz=z-floorz;
   float sx=fx*fx*fx*(10-fx*(15-fx*6)),
         sy=fy*fy*fy*(10-fy*(15-fy*6)),
         sz=fz*fz*fz*(10-fz*(15-fz*6));
   return trilerp(    fx*n000[0] +     fy*n000[1] +     fz*n000[2],
                  (fx-1)*n100[0] +     fy*n100[1] +     fz*n100[2],
                      fx*n010[0] + (fy-1)*n010[1] +     fz*n010[2],
                  (fx-1)*n110[0] + (fy-1)*n110[1] +     fz*n110[2],
                      fx*n001[0] +     fy*n001[1] + (fz-1)*n001[2],
                  (fx-1)*n101[0] +     fy*n101[1] + (fz-1)*n101[2],
                      fx*n011[0] + (fy-1)*n011[1] + (fz-1)*n011[2],
                  (fx-1)*n111[0] + (fy-1)*n111[1] + (fz-1)*n111[2],
                  sx, sy, sz);
}


unsigned int Noise3::hash_index(int i, int j, int k) const
{
    int hi = perm[i%n];
    int hj = perm[(hi+j)%n];
    return perm[(hj+k)%n];
}
