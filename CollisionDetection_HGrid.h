#ifndef COLLISION_DETECTION_HGRID_H
#define COLLISION_DETECTION_HGRID_H

#include "CollisionDetection_Interface.h"
#include "Particle.h"
#include "glm/glm.hpp"
#include <cstdlib>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <bitset>
#include <math.h>
#include "Random.h"

#include "gl/gl.h"
#include "gl/glu.h"

// const float SPHERE_TO_CELL_RATIO =

//#define DEBUG_HGRID
#define DEBUG_HGRID_BOARD_COLLISION
#define DEBUG_HGRID_BALLBALL_COLLISION



using namespace std;

typedef int(*pCallbackFunc)(h_Particle, h_Particle);

// boost unordered map

// 1. bounce
// 2. shadow



/*
typedef union
{
    unsigned long int value;        // 64 bit
    struct
    {
        short int x, y, z;          // short int is 8 bits/2 bytes
        unsigned short int level;   //
    } data;
} hash_t;
*/

/*
typedef union {

 // for some reason unsigned long int doesn't work

 //   unsigned long int value;
	uint64_t value;
	struct {
		short int x;
		short int y;
		short int z;
		unsigned short int level;
	} data;
	unsigned char c[8];
} hash_t;
*/


/*
typedef union {

 //   unsigned long int value;
	uint32_t value;
	struct {
		short int id1;
		short int id2;
	} data;
	unsigned char c[4];
} Col_hash_t;




*/



struct ParticleGroundPair
{
    h_Particle* h_par1;
};


class CollisionDetection_HGrid : public CollisionDetection_Interface
{

    // unions allow one portion of memory to be accessed as different data types
    // http://www.cplusplus.com/doc/tutorial/other_data_types/



    public:
        // my grid
        unordered_map<uint64_t, h_Particle*> myMap;
       // unordered_map<string, h_Particle*> myMap;

        unordered_map<uint32_t, ParticleCollisionPair*> ParticleCollisionPairList;


        glm::vec3 ExternalForce_neg;
        glm::vec3 ExternalForce_half_neg;
        glm::vec3 ExternalForce_pos;
        glm::vec3 ExternalForce_half_pos;

        float EnergyRetainRate_V;
        float EnergyRetainRate_H;

        // bit mask to see if it's occupied at each level
        uint32_t occupied_Levels_Mask;

        // how many particles are there on each level
        int particle_At_Level[HGRID_MAX_LEVELS];

        hash_t ComputeHashBucketIndex(h_Particle * h_par);
        hash_t ComputeHashBucketIndex(int x, int y, int z, int level);


    public:


        CollisionDetection_HGrid();
        ~CollisionDetection_HGrid();


        void addParticleToHGrid(  h_Particle * h_par  );

        void removeParticleFromHGrid( h_Particle *h_par );

        void checkObjAgainstObjects(h_Particle * h_par, vector<h_Particle*>& neighbors);

        void rehash( h_Particle *h_par );

        void draw();
        void drawCubeFrame(float size);


        void Test_AddParticle( h_Particle * h_par );

        bool CheckCollision( h_Particle * h_par, float dt );
        bool UpdateSphereGroundCollision(h_Particle * h_par, float dt);
        bool UpdateSphereSphereCollision(h_Particle * h_par, vector<h_Particle*>& neighbors, float dt);
        bool CheckSphereSphereCollision(h_Particle * h_par1, h_Particle * h_par2, bool & collision, float dt);

        bool CheckRestingContactOnGround(h_Particle * h_par, float dt);
        bool SphereAtRest(h_Particle * h_par);
        void HandleSphereSphereCollision(h_Particle * h_par1, h_Particle * h_par2, float dt);
};



#endif
