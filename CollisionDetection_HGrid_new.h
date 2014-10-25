#ifndef COLLISION_DETECTION_HGRID_NEW_H
#define COLLISION_DETECTION_HGRID_NEW_H

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
#define DEBUG_HGRID_BOARD_COLLISION 0
#define DEBUG_HGRID_BALLBALL_COLLISION


using namespace std;




class CollisionDetection_HGrid_NEW : public CollisionDetection_Interface
{

    // unions allow one portion of memory to be accessed as different data types
    // http://www.cplusplus.com/doc/tutorial/other_data_types/



    public:
        // my grid
        unordered_map<uint64_t, WorldObject*> myMap;

        unordered_map<uint32_t, ParticleCollisionPair_NEW*> ParticleCollisionPairList;


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

        hash_t ComputeHashBucketIndex(WorldObject* h_obj);
        hash_t ComputeHashBucketIndex(int x, int y, int z, int level);


    public:


        CollisionDetection_HGrid_NEW();
        ~CollisionDetection_HGrid_NEW();


        void AddParticleToHGrid(  WorldObject* h_obj  );

        void RemoveParticleFromHGrid( WorldObject* h_obj );

        void CheckObjAgainstObjects(WorldObject* h_obj, vector<WorldObject*>& neighbors);

        void rehash( WorldObject* h_obj );

        void Draw();
        void drawCubeFrame(float size);

        void Test_AddParticle( WorldObject* h_obj );

        bool CheckCollision( WorldObject* h_obj, float dt );


        bool UpdateObjGroundCollision(WorldObject* h_obj, float dt);
        bool CheckObjGroundCollision(WorldObject* h_obj, float dt);
        void ProcessObjGroundCollisionContact(WorldObject* h_obj);

        bool UpdateObjObjCollision(WorldObject* h_obj, vector<WorldObject*>& neighbors, float dt);


        bool CheckSphereSphereCollision(WorldSphere* h_obj1, WorldSphere* h_obj2, bool & collision, float dt);

        bool CheckRestingContactOnGround(WorldObject* h_obj, float dt);
        void HandleSphereSphereCollision(WorldObject* h_obj1, WorldObject* h_obj2, float dt);

        float GetBoundingVolumeSize(WorldObject* h_obj);

};



#endif

