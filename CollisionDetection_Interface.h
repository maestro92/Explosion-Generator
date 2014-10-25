#ifndef COLLISION_DETECTION_INTERFACE_H
#define COLLISION_DETECTION_INTERFACE_H


#include "glm/glm.hpp"
#include <cstdlib>
#include "WorldBox.h"
#include "WorldSphere.h"
#include "WorldObject.h"


using namespace std;


const float MIN_CELL_SIZE = 2;
const float MAX_CELL_SIZE = 20;       // the length of the side of a square
const unsigned int HGRID_MAX_LEVELS = 31;
const int NUM_BUCKETS = 1024;




typedef union {

 //   unsigned long int value;
	uint32_t value;
	struct {
		short int id1;
		short int id2;
	} data;
	unsigned char c[4];
} Col_hash_t;

struct ParticleCollisionPair
{
     h_Particle* h_par1;
     h_Particle* h_par2;

     ParticleCollisionPair()
     {
         h_par1 = new h_Particle();
         h_par2 = new h_Particle();
     }

};

struct ParticleCollisionPair_NEW
{
     WorldObject* h_obj1;
     WorldObject* h_obj2;

     ParticleCollisionPair_NEW()
     {
         h_obj1 = new WorldObject();
         h_obj2 = new WorldObject();
     }
};

class CollisionDetection_Interface
{
public:

};




#endif
