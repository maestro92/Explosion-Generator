#include "CollisionDetection_HGrid.h"



CollisionDetection_HGrid::CollisionDetection_HGrid()
{
    occupied_Levels_Mask = 0;
    ExternalForce_neg = vec3(0,-9.8,0);
    ExternalForce_half_pos = vec3(0,4.9f,0);
    EnergyRetainRate_V = 0.1f;
    EnergyRetainRate_H = 0.9f;
}


CollisionDetection_HGrid::~CollisionDetection_HGrid()
{

}


// velocity of bouncing ball
bool CollisionDetection_HGrid::CheckCollision(h_Particle * h_par, float dt)
{
    vector<h_Particle*> neighbors;

    bool SphereSphere = CheckObjAgainstObjects(h_par, neighbors);
    bool SphereGround = CheckSphereGroundCollision(h_par, dt);

    return SphereGround | SphereSphere;
 //   return SphereGround;
}






bool CollisionDetection_HGrid::CheckObjAgainstObjects(h_Particle * h_par, vector<h_Particle*>& neighbors)
{
    float cell_size;
    int startLevel = 0;  // smallest cell size
/*
    cout << "id is " << h_par->m_id << endl;
    cout << "speed x " << h_par->m_Velocity.x << endl;
    cout << "speed y " << h_par->m_Velocity.y << endl;
    cout << "speed z " << h_par->m_Velocity.z << endl;
*/
    uint32 Mask = occupied_Levels_Mask;
    glm::vec3 pos = h_par->m_Position;

    hash_t hash;

    Col_hash_t C_hash;
    ParticleCollisionPair* C_Pair = new ParticleCollisionPair();

    bool Collision = false;
   // occupiedLevelsMask >>=1 same as occupiedLevelsMask = occupiedLevelsMask >> 1
    for (int level = startLevel; level <= HGRID_MAX_LEVELS; Mask >>= 1, level++)
    {

        // if no objects in rest of grid, stop now
        if (Mask == 0) break;

        // if no objects at this level, go on to the next level
        if ((Mask & 1) == 0) continue;



        cell_size = MAX_CELL_SIZE / (1 << level);
        // compute ranges
        float delta = h_par->m_fRadius + cell_size/2.0;

        float cellScale = 1.0f / cell_size;

        hash.data.level = level;
        int xmin = (int)floorf((pos.x - delta) * cellScale);
        int ymin = (int)floorf((pos.y - delta) * cellScale);
        int zmin = (int)floorf((pos.z - delta) * cellScale);

        int xmax = (int)ceilf ((pos.x + delta) * cellScale);
        int ymax = (int)ceilf ((pos.y + delta) * cellScale);
        int zmax = (int)ceilf ((pos.z + delta) * cellScale);

/*
        cout << "id is " << h_par->m_id << endl;
        cout << "cell_size " << cell_size << endl;
        cout << "delta " << delta << endl;
        cout << "x " << h_par->m_Position.x << endl;
        cout << "y " << h_par->m_Position.y << endl;
        cout << "z " << h_par->m_Position.z << endl;


        cout << "hash.x " << h_par->m_hash.data.x << endl;
        cout << "hash.y " << h_par->m_hash.data.y << endl;
        cout << "hahs.z " << h_par->m_hash.data.z << endl;

        cout << "xmin " << xmin << endl;
        cout << "ymin " << ymin << endl;
        cout << "zmin " << zmin << endl;
        cout << "xmax " << xmax << endl;
        cout << "ymax " << ymax << endl;
        cout << "zmax " << zmax << endl;
*/


        for(int x = xmin; x <= xmax; x++)
        {
            hash.data.x = x;
            for(int y = ymin; y <= ymax; y++)
            {
                hash.data.y = y;
                for(int z = zmin; z <= zmax; z++)
                {
                    hash.data.z = z;
                    if (myMap.find(hash.value) != myMap.end() )
                    {
                        h_Particle *neighbor = myMap[hash.value];

                        while(neighbor != NULL)
                        {
                            if (neighbor != h_par)
                            {
                                neighbors.push_back(neighbor);
                                // add it to the ParticleCollisionPairList
                                cout << "I'm Over here" << endl;


                                if (CheckSphereSphereCollision(h_par, neighbor))
                                {
                                    cout << "Ball Ball Collision" << endl;
                                    Collision = true;
                                    if (h_par->m_id < neighbor->m_id)
                                    {
                                         C_hash.data.id1 = h_par->m_id;
#ifdef DEBUG_HGRID_BALLBALL_COLLISION
                                        std::bitset<32> y(C_hash.value);
                                        std::cout << y << endl;
#endif


                                         C_hash.data.id2 = neighbor->m_id;
#ifdef DEBUG_HGRID_BALLBALL_COLLISION
                                        std::bitset<32> z(C_hash.value);
                                        std::cout << z << endl;
#endif
                                         C_Pair->h_par1 = h_par;
                                         C_Pair->h_par2 = neighbor;
                                    }
                                    else
                                    {
                                         C_hash.data.id1 = neighbor->m_id;
                                         C_hash.data.id2 = h_par->m_id;
                                         C_Pair->h_par1 = neighbor;
                                         C_Pair->h_par2 = h_par;
                                    }

                                    std::bitset<32> x(C_hash.value);
                                    std::cout << x << endl;
                                    cout << "C_hash value is " << C_hash.value << endl;
                                    if (ParticleCollisionPairList.find(C_hash.value) == ParticleCollisionPairList.end())
                                    {
                                        cout << "Before " << endl;
                                        cout << "first ball is " << h_par->m_id << endl;
                                        cout << "vx " << h_par->m_Velocity.x << endl;
                                        cout << "vy " << h_par->m_Velocity.y << endl;
                                        cout << "vz " << h_par->m_Velocity.z << endl;

                                        cout << "2nd ball is " << neighbor->m_id << endl;
                                        cout << "vx " << neighbor->m_Velocity.x << endl;
                                        cout << "vy " << neighbor->m_Velocity.y << endl;
                                        cout << "vz " << neighbor->m_Velocity.z << endl << endl;


                                        ParticleCollisionPairList[C_hash.value] = C_Pair;
                                        HandleSphereSphereCollision(h_par, neighbor);

                                        cout << "After " << endl;
                                        cout << "first ball is " << h_par->m_id << endl;
                                        cout << "vx " << h_par->m_Velocity.x << endl;
                                        cout << "vy " << h_par->m_Velocity.y << endl;
                                        cout << "vz " << h_par->m_Velocity.z << endl;

                                        cout << "2nd ball is " << neighbor->m_id << endl;
                                        cout << "vx " << neighbor->m_Velocity.x << endl;
                                        cout << "vy " << neighbor->m_Velocity.y << endl;
                                        cout << "vz " << neighbor->m_Velocity.z << endl;

                                        cout << "Next Velocity " << endl;
                                        cout << "first ball is " << h_par->m_id << endl;
                                        cout << "vx " << h_par->m_NextVelocity.x << endl;
                                        cout << "vy " << h_par->m_NextVelocity.y << endl;
                                        cout << "vz " << h_par->m_NextVelocity.z << endl;

                                        cout << "2nd ball is " << neighbor->m_id << endl;
                                        cout << "vx " << neighbor->m_NextVelocity.x << endl;
                                        cout << "vy " << neighbor->m_NextVelocity.y << endl;
                                        cout << "vz " << neighbor->m_NextVelocity.z << endl;

                                    }
                                }
                            }
                            neighbor = neighbor->pNextParticle;
                        }
                    }
                }
            }
        }
    }
    return Collision;


}









bool CollisionDetection_HGrid::CheckSphereSphereCollision(h_Particle * h_par1, h_Particle * h_par2)
{
    glm::vec3 pos1 = h_par1->m_Position;
    glm::vec3 pos2 = h_par2->m_Position;
    float dist = ((pos1.x - pos2.x) * (pos1.x - pos2.x) +
                    (pos1.y - pos2.y) * (pos1.y - pos2.y) +
                    (pos1.z - pos2.z) * (pos1.z - pos2.z));
    float radius_sum = h_par1->m_fRadius + h_par2->m_fRadius;
    radius_sum = radius_sum * radius_sum;

    if (dist <= radius_sum)
    {
        vec3 netVelocity = h_par1->m_Velocity - h_par2->m_Velocity;
        vec3 displacement = h_par1->m_Position - h_par2->m_Position;
        bool sep = (glm::dot(netVelocity, displacement) < 0);
        // if they have opposite directions, then we ignore the contact
        return sep;
    }
    else
        return false;
  //  return true;
}













bool CollisionDetection_HGrid::CheckSphereGroundCollision(h_Particle * h_par, float dt)
{
    if ( (h_par->m_Position.y - h_par->m_fRadius) <= 0.0f)
    {

#ifdef DEBUG_HGRID_BOARD_COLLISION
        cout << "id is " << h_par->m_id << endl;
        cout << "vel x is " << h_par->m_Velocity.x << endl;
        cout << "vel y is " << h_par->m_Velocity.y << endl;
        cout << "vel z is " << h_par->m_Velocity.z << endl;

        cout << "pre_vel x is " << h_par->m_Pre_Velocity.x << endl;
        cout << "pre_vel y is " << h_par->m_Pre_Velocity.y << endl;
        cout << "pre_vel z is " << h_par->m_Pre_Velocity.z << endl;
        cout << "boundary is" << h_par->m_Position.y - h_par->m_fRadius << endl;
#endif



/*
        if( fabs(h_par->m_Pre_Velocity.x) < 0.01 )
        {
            h_par->m_Velocity = vec3(0, h_par->m_Velocity.y, h_par->m_Velocity.z);
            h_par->m_Pre_Velocity = vec3(0, h_par->m_Pre_Velocity.y, h_par->m_Pre_Velocity.z);
        }

        if( fabs(h_par->m_Pre_Velocity.y) < 0.01)
        {
            h_par->m_Velocity = vec3(h_par->m_Velocity.x, 0, h_par->m_Velocity.z);
            h_par->m_Pre_Velocity = vec3(h_par->m_Pre_Velocity.x, 0, h_par->m_Pre_Velocity.z);
        }

        if( fabs(h_par->m_Pre_Velocity.z) < 0.01)
        {
            h_par->m_Velocity = vec3(h_par->m_Velocity.x, h_par->m_Velocity.y, 0);
            h_par->m_Pre_Velocity = vec3(h_par->m_Pre_Velocity.x, h_par->m_Pre_Velocity.y, 0);
        }
*/
/*
#ifdef DEBUG_HGRID_BOARD_COLLISION
        cout << "the updates is "<< endl;
        cout << "vel x is " << h_par->m_Velocity.x << endl;
        cout << "vel y is " << h_par->m_Velocity.y << endl;
        cout << "vel z is " << h_par->m_Velocity.z << endl;

        cout << "pre_vel x is " << h_par->m_Pre_Velocity.x << endl;
        cout << "pre_vel y is " << h_par->m_Pre_Velocity.y << endl;
        cout << "pre_vel z is " << h_par->m_Pre_Velocity.z << endl;

#endif
*/
   //     bool real_v = ( fabs(h_par->m_Velocity.y) < 0.01 && fabs(h_par->m_Velocity.x) < 0.01 && fabs(h_par->m_Velocity.z) < 0.01);
      /*
        if( fabs(h_par->m_Pre_Velocity.y) < 0.01)
        {
            h_par->m_Velocity = vec3(h_par->m_Velocity.x, 0, h_par->m_Velocity.z);
            h_par->m_Pre_Velocity = vec3(h_par->m_Pre_Velocity.x, 0, h_par->m_Pre_Velocity.z);
        }
*/

/*  ####
        if( fabs(h_par->m_Pre_Velocity.y) < 0.01 && fabs(h_par->m_Pre_Velocity.x) < 0.01 && fabs(h_par->m_Pre_Velocity.z) < 0.01)
        {

            cout << "I'm really here" << endl;
            h_par->m_Velocity = vec3(0,0,0);
            h_par->m_Pre_Velocity = h_par->m_Velocity;
            h_par->m_Position = vec3(h_par->m_Position.x ,h_par->m_fRadius,h_par->m_Position.z );
            return true;

        }
*/



//        if( fabs(h_par->m_Pre_Velocity.y) < 0.01 && fabs(h_par->m_Pre_Velocity.x) < 0.01 && fabs(h_par->m_Pre_Velocity.z) < 0.01)
        if( fabs(h_par->m_Pre_Velocity.y) < (9.81 * dt) && fabs(h_par->m_Pre_Velocity.x) < 0.01 && fabs(h_par->m_Pre_Velocity.z) < 0.01)
        {

            cout << "I'm really here" << endl;
            h_par->m_NextVelocity = vec3(0,0,0);
            h_par->m_Pre_Velocity = h_par->m_NextVelocity;
            h_par->m_NextPosition = vec3(h_par->m_Position.x ,h_par->m_fRadius,h_par->m_Position.z );
            return true;

        }

        if( fabs(h_par->m_Pre_Velocity.y) < (9.81 * dt))
        {

            cout << "Really here Checking y" << endl;

            h_par->m_NextVelocity.x = h_par->m_Velocity.x * EnergyRetainRate_H;
            h_par->m_NextVelocity.z = h_par->m_Velocity.z * EnergyRetainRate_H;
            h_par->m_NextVelocity = vec3(h_par->m_NextVelocity.x, 0, h_par->m_NextVelocity.z);

            h_par->m_NextPosition = h_par->m_Position + (h_par->m_Velocity + dt * ExternalForce_half_pos )* dt;

            h_par->m_Pre_Velocity = vec3(h_par->m_NextVelocity.x, 0, h_par->m_NextVelocity.z);
            return true;
        }

        // else it's just a regular collision
        else if ( h_par->m_Velocity.y < 0)
        {
            cout << "I'm here" << endl;

/// this is incorrect, should return the position based on the old velocity, not the new one
            h_par->m_NextVelocity.y = h_par->m_Velocity.y * -1.0f;
            h_par->m_NextPosition = h_par->m_Position + (h_par->m_NextVelocity + dt * ExternalForce_half_pos ) * dt;
         //   h_par->m_Position = h_par->m_prev_Position;

            cout << "next vel y " << h_par->m_NextVelocity.y << endl;
            cout << "next pos y boundary " << (h_par->m_NextPosition.y - h_par->m_fRadius) << endl;


//            glm::vec3 old_vel = h_par->m_Velocity.y + ExternalForce_half_pos*2.0f * dt;
  //          h_par->m_Position -= (h_par->m_Velocity + old_vel ) * 0.5f * dt;

    //        e_ParticleBuffer[i].m_Velocity += ExternalForce_neg*dt;
      //      e_ParticleBuffer[i].m_Position += (old_vel + e_ParticleBuffer[i].m_Velocity)* 0.5f * dt;

            // m_Vel and m_Pre_Vec, Which ever one is lower, we choose that as the new velocity

            if (h_par->m_Pre_Velocity.y < h_par->m_NextVelocity.y )
            {
                h_par->m_NextVelocity.y = h_par->m_Pre_Velocity.y;
            }

            h_par->m_NextVelocity.y = h_par->m_NextVelocity.y * EnergyRetainRate_V;
            h_par->m_NextVelocity.x = h_par->m_NextVelocity.x * EnergyRetainRate_H;
            h_par->m_NextVelocity.z = h_par->m_NextVelocity.z * EnergyRetainRate_H;

            h_par->m_Pre_Velocity = h_par->m_NextVelocity;
            cout << "after - vel y is " << h_par->m_NextVelocity.y << endl;

            return true;
        }

    }

    return false;
}



void CollisionDetection_HGrid::HandleSphereSphereCollision(h_Particle * h_par1, h_Particle * h_par2)
{
    const double e = 1.0f; // coefficient of restitution
    glm::vec3 x1 = h_par1->m_Position;
    glm::vec3 x2 = h_par2->m_Position;

    glm::vec3 v1 = h_par1->m_Velocity;
    glm::vec3 v2 = h_par2->m_Velocity;

    // calculate the normal
    glm::vec3 rel_x = x1-x2;

    glm::vec3 norm = glm::normalize(rel_x);
   // float mag_x = dot(rel_x, rel_x);
   // glm::vec3 norm = rel_x / mag_x;


/*
    cout << "norm x " << norm.x << endl;
    cout << "norm y " << norm.y << endl;
    cout << "norm z " << norm.z << endl;
*/



    // vel
    glm::vec3 rel_v = v1-v2;
    float rel_v_norm = glm::dot(rel_v, norm);           // separating velocity


    /// ### added parts
    float newSepVel = - rel_v_norm * 1.0f;
    float accCausedVel = -9.80;
    accCausedVel -= (-9.80);
    float accCausedSepVel = accCausedVel * 0.031;
    if ( accCausedSepVel < 0)
    {
        newSepVel += 0.9 * accCausedSepVel;
        if(newSepVel < 0)
            newSepVel = 0;
    }



    float deltaVel = newSepVel - rel_v_norm;
    float totalInverseMass = 2;
    float impulse = deltaVel / totalInverseMass;
    glm::vec3 impulsePerIMass = impulse * norm;

  //  h_par1->m_Velocity += (impulsePerIMass/h_par1->m_fmass);
  //  h_par2->m_Velocity -= (impulsePerIMass/h_par2->m_fmass);

    h_par1->m_NextVelocity = h_par1->m_Velocity + (impulsePerIMass/h_par1->m_fmass);
    h_par2->m_NextVelocity = h_par2->m_Velocity - (impulsePerIMass/h_par2->m_fmass);
    /// ### added parts


/*
    float fNumer = -(1+e) * rel_v_norm;
    float fDenom = ((1/h_par1->m_fmass)+ (1/h_par2->m_fmass));
    float f = fNumer / fDenom;
    glm::vec3 impulse = f * norm;

 //   h_par1->m_Velocity += (impulse/h_par1->m_fmass);
 //   h_par2->m_Velocity -= (impulse/h_par2->m_fmass);

    h_par1->m_NextVelocity = h_par1->m_Velocity + (impulse/h_par1->m_fmass);
    h_par2->m_NextVelocity = h_par2->m_Velocity - (impulse/h_par2->m_fmass);

*/
    h_par1->m_Pre_Velocity = glm::vec3(1000,1000,1000);
    h_par2->m_Pre_Velocity = glm::vec3(1000,1000,1000);
//    cout << "impulse f " << f << endl;
}


/*
void CollisionDetection_HGrid::CheckParticleAgainstGrid(h_Particle *h_par,
                                                        void (*pCallbackFunc)(h_Particle *pA, h_Particle *pB))
{
    float size = (float)MIN_CELL_SIZE;
    int startLevel = 0;
    uint32 occupiedLevelsMask = grid->occupied_Levels_Mask;
    vec3 pos = h_par->m_Position;


}
*/


/*
bool CollisionDetection_HGrid::CheckCollision(h_Particle * h_par, float dt)
{
    if ( (h_par->m_Position.y - h_par->m_fRadius) <= 0.0f)
    {
        cout << "id is " << h_par->m_id << endl;
        cout << "vel y is " << h_par->m_Velocity.y << endl;
        cout << "pre_vel y is " << h_par->m_Pre_Velocity.y << endl;
        cout << "boundary is" << h_par->m_Position.y - h_par->m_fRadius << endl;


        if( fabs(h_par->m_Pre_Velocity.y - h_par->m_Velocity.y) < 0.001)

        {

            cout << "I'm really here" << endl;
     //       return false;
            h_par->m_Velocity = vec3(0,0,0);
            h_par->m_Pre_Velocity = h_par->m_Velocity;
            h_par->m_Position = vec3(h_par->m_Position.x ,h_par->m_fRadius,h_par->m_Position.z );
            return true;
        }

        else if ( h_par->m_Velocity.y < 0)
        {
            cout << "I'm here" << endl;
            h_par->m_Pre_Velocity = h_par->m_Velocity;

            h_par->m_Velocity.y = h_par->m_Velocity.y * -1.0f;
            h_par->m_Position += h_par->m_Velocity * dt;

            if (h_par->m_Pre_Out_Velocity.y < h_par->m_Velocity.y )
            {
                h_par->m_Velocity.y = h_par->m_Pre_Out_Velocity.y;
            }

            h_par->m_Velocity.y = h_par->m_Velocity.y * EnergyRetainRate_V;
            h_par->m_Velocity.x = h_par->m_Velocity.x * EnergyRetainRate_H;
            h_par->m_Velocity.z = h_par->m_Velocity.z * EnergyRetainRate_H;


            h_par->m_Pre_Out_Velocity = h_par->m_Velocity;
            cout << "after - vel y is " << h_par->m_Velocity.y << endl;

            return true;
        }
    }

    return false;
}
*/





hash_t CollisionDetection_HGrid::ComputeHashBucketIndex(h_Particle * h_par)
{
    hash_t hash_key;

    // in a binary tree, level 0 is the root node, so we will use that notation
    unsigned short int level = HGRID_MAX_LEVELS;

    // getting the size of this particle
    float size = h_par->m_fRadius * 2;
    assert (size < MAX_CELL_SIZE);      // MAX_CELL_SIZE is 20

    // calculate which level does he fit in
    // we use left-bit-shift operation to calculate the cell size
    // level 0: biggest cell
    // level n: nth biggest cell
    float tmp = MAX_CELL_SIZE / ((1 << level));
    // cout << tmp << endl;

    while ( size > MAX_CELL_SIZE / (1 << level) )
    {
        level--;
    }

    float cell_size = MAX_CELL_SIZE / (1 << level);
    hash_key.data.x = (short int)floor(h_par->m_Position.x/ cell_size);
    hash_key.data.y = (short int)floor(h_par->m_Position.y/ cell_size);
    hash_key.data.z = (short int)floor(h_par->m_Position.z/ cell_size);

/*
    hash_key.data.x = (short int)h_par->m_Position.x;
    hash_key.data.y = (short int)h_par->m_Position.y;
    hash_key.data.z = (short int)h_par->m_Position.z;
  */
    hash_key.data.level = (unsigned short int)level;

    return hash_key;
}


hash_t CollisionDetection_HGrid::ComputeHashBucketIndex(int x, int y, int z, int level)
{
    hash_t hash_key;

    hash_key.data.x = (short int)x;
    hash_key.data.y = (short int)y;
    hash_key.data.z = (short int)z;
    hash_key.data.level = (unsigned short int)level;
    return hash_key;

}


void CollisionDetection_HGrid::AddParticleToHGrid( h_Particle * h_par )
{

    // Compute the hashvalue
    hash_t hash = ComputeHashBucketIndex(h_par);

#ifdef DEBUG_HGRID
    cout << "hash is " << hash.value << endl;
   // bitset<64> g(hash.value);
   // cout << g << endl;

    cout << "type is " << typeid(hash.value).name() << endl;

    cout << "hash.x " << hash.data.x << endl;
   // bitset<16> g1(hash.data.x);
   // cout << g1 << endl;


    cout << "hash.y " << hash.data.y << endl;
   // bitset<16> g2(hash.data.y);
   // cout << g2 << endl;


    cout << "hash.z " << hash.data.z << endl;
   // bitset<16> g3(hash.data.z);
   // cout << g3 << endl;


    cout << "hash level " << hash.data.level << endl << endl;
   // bitset<16> g4(hash.data.level);
   // cout << g4 << endl << endl;

/*
    uint64_t tmp = 0;

    bitset<64> g11(tmp);
    cout << "tmp init is " << g11 << endl;

    for (int i = 0; i<8; i++)
    {
        bitset<8> g5(hash.c[i]);
        cout << g5 << endl;

        tmp = tmp | (hash.c[i]);

        bitset<64> g6(tmp);
        cout << "tmp is " << g6 << endl;
        if (i != 7)
            tmp = (tmp << 8);
    }

    bitset<64> g6(tmp);
    cout << "tmp is " << g6 << endl;
    cout << "tmp final is " << tmp << endl;
*/
#endif

   // cout << "bucket_index level is " << hash.data.level << endl << endl;

    // assert if object is larger than largest grid cell
    assert(hash.data.level < HGRID_MAX_LEVELS);

    // update HGrid structures
    particle_At_Level[hash.data.level]++;
    occupied_Levels_Mask |= (1 << hash.data.level);


    // assign the hash_index and level to the particle
    h_par->m_hash.value = hash.value;
//    h_par->m_level = hash.data.level;
/*
    cout << "id is " << h_par->m_id << endl;
    cout << "hash.x " << h_par->m_hash.data.x << endl;
    cout << "hash.y " << h_par->m_hash.data.y << endl;
    cout << "hahs.z " << h_par->m_hash.data.z << endl;
*/
    // add to the unordered_map
    if ( myMap.find(hash.value) == myMap.end() )
    {
        h_par->pPrevParticle = NULL;
        h_par->pNextParticle = NULL;
        myMap[hash.value] = h_par;
    }

    // if it's not empty insert at the front
    else
    {
        h_Particle* head = myMap[hash.value];
        head->pPrevParticle = h_par;
        h_par->pNextParticle = head;

        h_par->pPrevParticle = NULL;
        myMap[hash.value] = h_par;
    }

}



void CollisionDetection_HGrid::Test_AddParticle( h_Particle * h_par )
{
    /*
    hash_t h;


    for ( unsigned i = 0; i < myMap.bucket_count(); ++i)
    {
        cout << "bucket #" << i << " contains:";
        for ( auto local_it = myMap.begin(i); local_it!= myMap.end(i); ++local_it )
        {
            h.value = local_it->first;
            cout << " " << local_it->first << ":-:" << local_it->second << endl;


            cout << "x is " << h.data.x << endl;
            cout << "y is " << h.data.y << endl;
            cout << "z is " << h.data.z << endl;
            cout << "level is " << h.data.level << endl;
            //cout << endl << endl;

        }
        cout << endl;
    }
    */
}


void CollisionDetection_HGrid::RemoveParticleFromHGrid( h_Particle *h_par )
{
    // Compute the hashvalue
    hash_t hash;
    hash.value = h_par->m_hash.value;

    // if particles at that level decreases down to zero,
    // we change the occupied_Levels_Mask to zero at that bit
    if(--particle_At_Level[hash.data.level] == 0)
        occupied_Levels_Mask &= ~(1 << hash.data.level);

    h_Particle* prev = h_par->pPrevParticle;
    h_Particle* next = h_par->pNextParticle;

    // we are not the head
    if (prev)
    {
        // and if there's something behind
        if (next)
        {
            prev->pNextParticle = next;
            next->pPrevParticle = prev;
        }
        else
            prev->pNextParticle = NULL;
    }

    // if we're the head
    else
    {
        if(next)
        {
            myMap[hash.value] = next;
            next->pPrevParticle = NULL;
        }
        else
            myMap.erase(hash.value);
    }

}



void CollisionDetection_HGrid::rehash( h_Particle *h_par )
{
    RemoveParticleFromHGrid(h_par);
    /*
    float dt = 0.031;

    if (h_par->m_Position.y >= 0)
//        if(e_ParticleBuffer[i].m_fAge < e_ParticleBuffer[i].m_fLifeTime)
    {
        vec3 v = h_par->m_Velocity;
        v = v * dt;
        h_par->m_Position += v;
        h_par->m_Velocity += ExternalForce1*dt;
    }
*/


    AddParticleToHGrid(h_par);

}




/*
void CollisionDetection_HGrid::CheckParticleAgainstGrid(h_Particle *h_par,
                                                        void (*pCallbackFunc)(h_Particle *pA, h_Particle *pB))
{
    float size = (float)MIN_CELL_SIZE;

    int startLevel = HGRID_MAX_LEVELS;  // smallest cell size

    uint32 occupiedLevelsMask = occupied_Levels_Mask;
    vec3 pos = h_par->m_Position;

    // occupiedLevelsMask >>=1 same as occupiedLevelsMask = occupiedLevelsMask >> 1
    for (int level = startLevel; level >=0; occupiedLevelsMask >>= level, level--)
    {
        // if no objects in rest of grid, stop now
        if (occupiedLevelsMask == 0) break;

        // if no objects at this level, go on to the next level
        if (occupiedLevelsMask & 1 == 0) continue;

        // compute ranges
        float delta
    }

}
*/



void CollisionDetection_HGrid::Draw()
{
    int i = 0;
    for (auto it = myMap.begin(); it != myMap.end(); it++)
    {
		i++;
		hash_t hash;
		hash.value = (*it).first;

   //     cout << " hash Value is " << it->first << ":" << it->second;
   //     cout << endl;

  //      cout << "hash Value level is " << hash.data.level << endl;

		float size = MAX_CELL_SIZE / (1 << hash.data.level);

        vec3 center;
        // the grid index + half of the cell size cuz we want the center, the 0.5 * size is for to offset it to the center
        center.x = (hash.data.x + 0.5) * size;
        center.y = (hash.data.y + 0.5) * size;
        center.z = (hash.data.z + 0.5) * size;
/*
        cout << "i is " << i << endl;
        cout << "center.x is" << center.x << endl;
        cout << "center.y is" << center.y << endl;
        cout << "center.z is" << center.z << endl;
*/
 //       center.x = hash.data.x;
   //     center.y = hash.data.y;
     //   center.z = hash.data.z;

        glPushMatrix();
            glTranslatef(center.x, center.y, center.z);
            drawCubeFrame(size);
            // glutWireCube(size);
        glPopMatrix();
    }
 //   cout << "i is " << i << endl << endl << endl;


}

/*
        glPushMatrix();
            glTranslatef(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            // draw a sphere
            // Glint slices, Glint stacks: resolution
            glColor3f(0,0,e_ParticleBuffer[i].m_id * 0.125);
            gluSphere(quad,e_ParticleBuffer[i].m_fRadius,20,20);
        glPopMatrix();
  */



void CollisionDetection_HGrid::drawCubeFrame(float size)
{
    glPushMatrix();
        glDisable( GL_LIGHTING );
        glBegin(GL_LINES);
            // front face
            glColor3f(1.0,0.0,1.0);
            glVertex3f(size/2, size/2  , size/2);
            glVertex3f(-size/2, size/2  , size/2);

            glVertex3f(-size/2, -size/2  , size/2);
            glVertex3f(size/2, -size/2  , size/2);

            glVertex3f(-size/2, size/2  , size/2);
            glVertex3f(-size/2, -size/2  , size/2);

            glVertex3f(size/2, size/2  , size/2);
            glVertex3f(size/2, -size/2  , size/2);


            // back face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, size/2  , -size/2);
            glVertex3f(-size/2, size/2 , -size/2);

            glVertex3f(-size/2, -size/2 , -size/2);
            glVertex3f(size/2, -size/2 , -size/2);

            glVertex3f(size/2, size/2  , -size/2);
            glVertex3f(size/2, -size/2 , -size/2);

            glVertex3f(-size/2, size/2 , -size/2);
            glVertex3f(-size/2, -size/2 , -size/2);

            // left face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(-size/2, size/2 , size/2);
            glVertex3f(-size/2, size/2 , -size/2);
            glVertex3f(-size/2, -size/2 , -size/2);
            glVertex3f(-size/2, -size/2 , size/2);

            // right face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, size/2 , -size/2);
            glVertex3f(size/2, size/2 , size/2);
            glVertex3f(size/2, -size/2 , size/2);
            glVertex3f(size/2, -size/2 , -size/2);
/*
            // top face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, size/2+offset, size/2);
            glVertex3f(-size/2, size/2+offset, size/2);
            glVertex3f(-size/2, size/2+offset, -size/2);
            glVertex3f(size/2, size/2+offset, -size/2);

            // bottom face
            glColor3f(1.0,1.0,1.0);
            glVertex3f(size/2, -size/2+offset, size/2);
            glVertex3f(-size/2, -size/2+offset, size/2);
            glVertex3f(-size/2, -size/2+offset, -size/2);
            glVertex3f(size/2, -size/2+offset, -size/2);
*/
        glEnd();
        glEnable( GL_LIGHTING );
    glPopMatrix();
}

