#include "CollisionDetection_HGrid_new.h"




CollisionDetection_HGrid_NEW::CollisionDetection_HGrid_NEW()
{
    occupied_Levels_Mask = 0;

    ExternalForce_neg = vec3(0,-9.8,0);
    ExternalForce_pos = vec3(0,9.8,0);
    ExternalForce_half_neg = vec3(0,-4.9,0);
    ExternalForce_half_pos = vec3(0,4.9,0);

    EnergyRetainRate_V = 0.5f;
    EnergyRetainRate_H = 0.9f;
}


CollisionDetection_HGrid_NEW::~CollisionDetection_HGrid_NEW()
{

}


// velocity of bouncing ball
bool CollisionDetection_HGrid_NEW::CheckCollision(WorldObject* h_obj, float dt)
{
    vector<WorldObject*> neighbors;

    bool ObjObj = UpdateObjObjCollision(h_obj, neighbors, dt);
    bool ObjGround = UpdateObjGroundCollision(h_obj, dt);

    bool sgss = ObjGround | ObjObj;
  //  bool sgss = ObjGround;
    return sgss;
}


/// Force * mass = linear acceleration
/// torque * Inertia = angular acceleration
/// we can turn a force into a torque,
/// torque = r * f (check page224)

/// contact normal SIMULATION
/// http://www.myphysicslab.com/beta/contact.html
/// http://www.myphysicslab.com/beta/contact.html
/// http://www.myphysicslab.com/beta/contact.html
void CollisionDetection_HGrid_NEW::ProcessObjGroundCollisionContact(WorldObject* h_obj)
{
    /// idea
    /// impulse: instantaneous linear velocity change
    /// impulsive torque: instantaneous angular velocity change
    float e = 0.9;
    glm::vec3 N = glm::vec3(0,1,0);
    glm::vec4 tempA = ((WorldBox*)h_obj)->GroundContactPoint;
    glm::vec3 rA = glm::vec3(tempA.x, tempA.y, tempA.z) - h_obj->w_Position;
    glm::vec3 kA = glm::cross(rA, N);
    glm::vec3 kB = glm::vec3(0);
    glm::vec3 uA = h_obj->w_InverseInertiaTensor * kA;
    glm::vec3 uB = glm::vec3(0);

    glm::vec3 rel_v = ((WorldBox*)h_obj)->w_Velocity;
//    glm::vec3 uA = kA;
//    glm::vec3 uB = glm::vec3(0);
    double fNumer = -(1 + e) * ( glm::dot(N, rel_v) + glm::dot(((WorldBox*)h_obj)->w_AngularVelocity, kA) );
    double fDenom = 1.0f + glm::dot(kA,uA);
 //     double fDenom = 1.0f;
    double f = fNumer / fDenom;
    glm::vec3 impulse = glm::vec3(f*N.x, f*N.y, f*N.z);

    ((WorldBox*)h_obj)->w_Velocity += impulse;
    ((WorldBox*)h_obj)->w_AngularVelocity += (((float)f) * uA);
    /// a.massinv = 1,    b.massinv = 0


}


bool CollisionDetection_HGrid_NEW::UpdateObjGroundCollision(WorldObject* h_obj, float dt)
{

    if ( CheckObjGroundCollision(h_obj, dt))
    {

#if DEBUG_HGRID_BOARD_COLLISION
//        cout << "id is " << h_obj->m_id << endl;
        cout << "vel x is " << h_obj->w_Velocity.x << endl;
        cout << "vel y is " << h_obj->w_Velocity.y << endl;
        cout << "vel z is " << h_obj->w_Velocity.z << endl;

#endif

        ProcessObjGroundCollisionContact(h_obj);


     //       cout << "I'm here" << endl;


/*
        /// for spheres, we're doing a simple case
            h_obj->w_Velocity.y = h_obj->w_Velocity.y * -1.0f;
            h_obj->w_Velocity.y = h_obj->w_Velocity.y * EnergyRetainRate_V;
            h_obj->w_Velocity.x = h_obj->w_Velocity.x * EnergyRetainRate_H;
            h_obj->w_Velocity.z = h_obj->w_Velocity.z * EnergyRetainRate_H;
*/
    //        cout << "after - vel y is " << h_obj->w_Velocity.y << endl;

            /// if the velocity has dropped below the amount that one timestep of acceleration would generate,
            /// we make it equal to dt * ExternalForce_pos.y to cancel out the effect
            if (h_obj->w_Velocity.y <= ( dt * ExternalForce_pos.y))
            {
                h_obj->w_Velocity.y = dt * ExternalForce_pos.y;
            }

            return true;

    }

    return false;
}






bool CollisionDetection_HGrid_NEW::CheckObjGroundCollision(WorldObject* h_obj, float dt)
{
    switch(h_obj->obj_type)
    {
        case SPHERE:
            return true;
        case BOX:
            return ((WorldBox*)h_obj)->CheckObjGroundCollision(dt);
    }
}


float CollisionDetection_HGrid_NEW::GetBoundingVolumeSize(WorldObject* h_obj)
{
    switch(h_obj->obj_type)
    {
        case SPHERE:
            return 20;
        case BOX:
            return ((WorldBox*)h_obj)->getBoundingVolumeSize();
    }
}





bool CollisionDetection_HGrid_NEW::UpdateObjObjCollision(WorldObject* h_obj, vector<WorldObject*>& neighbors, float dt)
{
#if 0
    float cell_size;
    int startLevel = 0;  // smallest cell size
/*
    cout << "id is " << h_obj->m_id << endl;
    cout << "speed x " << h_obj->w_Velocity.x << endl;
    cout << "speed y " << h_obj->w_Velocity.y << endl;
    cout << "speed z " << h_obj->w_Velocity.z << endl;
*/
    uint32_t Mask = occupied_Levels_Mask;
    glm::vec3 pos = h_obj->w_Position;

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
        float delta = h_obj->m_fRadius + cell_size/2.0;

        float cellScale = 1.0f / cell_size;

        hash.data.level = level;
        int xmin = (int)floorf((pos.x - delta) * cellScale);
        int ymin = (int)floorf((pos.y - delta) * cellScale);
        int zmin = (int)floorf((pos.z - delta) * cellScale);

    //    int xmax = (int)floorf ((pos.x + delta) * cellScale);
    //    int ymax = (int)floorf ((pos.y + delta) * cellScale);
    //    int zmax = (int)floorf ((pos.z + delta) * cellScale);

        int xmax = (int)ceilf ((pos.x + delta) * cellScale);
        int ymax = (int)ceilf ((pos.y + delta) * cellScale);
        int zmax = (int)ceilf ((pos.z + delta) * cellScale);
     //   float zmax_tmp = (pos.z + delta) * cellScale;
     //   int zmax = 0;
     //   if (zmax_tmp > 0)
     //       zmax = (int)ceilf (zmax_tmp);
     //   else
     //       zmax = (int)floorf (zmax_tmp);
/*
        cout << "id is " << h_obj->m_id << endl;
        cout << "cell_size " << cell_size << endl;
        cout << "delta " << delta << endl;
        cout << "x " << h_obj->w_Position.x << endl;
        cout << "y " << h_obj->w_Position.y << endl;
        cout << "z " << h_obj->w_Position.z << endl;


        cout << "hash.x " << h_obj->m_hash.data.x << endl;
        cout << "hash.y " << h_obj->m_hash.data.y << endl;
        cout << "hahs.z " << h_obj->m_hash.data.z << endl;

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
                        WorldObject*neighbor = myMap[hash.value];

                        while(neighbor != NULL)
                        {
                            if (neighbor != h_obj)
                            {
                                neighbors.push_back(neighbor);
                                // add it to the ParticleCollisionPairList
                                cout << "I'm Over here" << endl;


                                if (CheckSphereSphereCollision(h_obj, neighbor, Collision, dt))
                                {
                                    cout << "Ball Ball Collision" << endl;
                                    Collision = true;
                                    if (h_obj->m_id < neighbor->m_id)
                                    {
                                         C_hash.data.id1 = h_obj->m_id;
#ifdef DEBUG_HGRID_BALLBALL_COLLISION
                                        std::bitset<32> y(C_hash.value);
                                        std::cout << y << endl;
#endif


                                         C_hash.data.id2 = neighbor->m_id;
#ifdef DEBUG_HGRID_BALLBALL_COLLISION
                                        std::bitset<32> z(C_hash.value);
                                        std::cout << z << endl;
#endif
                                         C_Pair->h_obj1 = h_obj;
                                         C_Pair->h_obj2 = neighbor;
                                    }
                                    else
                                    {
                                         C_hash.data.id1 = neighbor->m_id;
                                         C_hash.data.id2 = h_obj->m_id;
                                         C_Pair->h_obj1 = neighbor;
                                         C_Pair->h_obj2 = h_obj;
                                    }

                                    std::bitset<32> x(C_hash.value);
                                    std::cout << x << endl;
                                    cout << "C_hash value is " << C_hash.value << endl;
                                    if (ParticleCollisionPairList.find(C_hash.value) == ParticleCollisionPairList.end())
                                    {
                                        cout << "Before " << endl;
                                        cout << "first ball is " << h_obj->m_id << endl;
                                        cout << "vx " << h_obj->w_Velocity.x << endl;
                                        cout << "vy " << h_obj->w_Velocity.y << endl;
                                        cout << "vz " << h_obj->w_Velocity.z << endl;

                                        cout << "2nd ball is " << neighbor->m_id << endl;
                                        cout << "vx " << neighbor->w_Velocity.x << endl;
                                        cout << "vy " << neighbor->w_Velocity.y << endl;
                                        cout << "vz " << neighbor->w_Velocity.z << endl << endl;


                                        ParticleCollisionPairList[C_hash.value] = C_Pair;
                                        HandleSphereSphereCollision(h_obj, neighbor, dt);

                                        cout << "After " << endl;
                                        cout << "first ball is " << h_obj->m_id << endl;
                                        cout << "vx " << h_obj->w_Velocity.x << endl;
                                        cout << "vy " << h_obj->w_Velocity.y << endl;
                                        cout << "vz " << h_obj->w_Velocity.z << endl;

                                        cout << "2nd ball is " << neighbor->m_id << endl;
                                        cout << "vx " << neighbor->w_Velocity.x << endl;
                                        cout << "vy " << neighbor->w_Velocity.y << endl;
                                        cout << "vz " << neighbor->w_Velocity.z << endl;
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
#endif // 0

    return false;
}


bool CollisionDetection_HGrid_NEW::CheckSphereSphereCollision(WorldSphere* h_obj1, WorldSphere* h_obj2, bool & collision, float dt)
{

    glm::vec3 v1 = h_obj1->w_Velocity + ExternalForce_neg*dt;
    glm::vec3 v2 = h_obj2->w_Velocity + ExternalForce_neg*dt;;

    glm::vec3 pos1 = h_obj1->w_Position + v1 * dt;
    glm::vec3 pos2 = h_obj2->w_Position + v2 * dt;


/*
    float dist = ((pos1.x - pos2.x) * (pos1.x - pos2.x) +
                    (pos1.y - pos2.y) * (pos1.y - pos2.y) +
                    (pos1.z - pos2.z) * (pos1.z - pos2.z));
*/

    float dist = ((pos1.x - pos2.x) * (pos1.x - pos2.x) +
                    (pos1.y - pos2.y) * (pos1.y - pos2.y) +
                    (pos1.z - pos2.z) * (pos1.z - pos2.z));

    float radius_sum = h_obj1->w_radius + h_obj2->w_radius;
    radius_sum = radius_sum * radius_sum;

    collision = (dist <= radius_sum);

    return collision;

#if 0
    if (dist <= radius_sum)
    {
        collision = true;
        vec3 netVelocity = h_obj1->w_Velocity - h_obj2->w_Velocity;
        vec3 displacement = h_obj1->w_Position - h_obj2->w_Position;
        bool sep = (glm::dot(netVelocity, displacement) <= 0);
        // if they have opposite directions, then we ignore the contact
        return sep;
    }
    else
        return false;
  //  return true;
#endif
}






void CollisionDetection_HGrid_NEW::HandleSphereSphereCollision(WorldObject* h_obj1, WorldObject* h_obj2, float dt)
{
#if 0
    bool straight_down = false;
    int upperBall = 1;

    float offset_x = 0;
    float offset_z = 0;
    cout << "HandleSphereSphereCollision" << endl;
    cout << "h_obj1 id" << h_obj1->m_id << endl;

    cout << "h_obj2 id" << h_obj2->m_id << endl;

    const double restitution = 0.1f; // coefficient of restitution
    glm::vec3 x1 = h_obj1->w_Position;
    glm::vec3 x2 = h_obj2->w_Position;

    glm::vec3 v1 = h_obj1->w_Velocity;
    glm::vec3 v2 = h_obj2->w_Velocity;

    // calculate the normal
    glm::vec3 rel_x = x1-x2;
    if (fabs(rel_x.x) <= 0.5 && fabs(rel_x.z) <= 0.5)
    {
        cout << "straight down" << endl;
        straight_down = true;
        offset_x = RandRange(-2, 2);
        offset_z = RandRange(-2, 2);
        if (h_obj2->w_Position.y > h_obj1->w_Position.y)
            upperBall = 2;
    }


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
    cout << "rel_v y" << rel_v.y << endl;
    cout << "rel_v_norm " << rel_v_norm << endl;

    // collision
   // if(rel_v_norm < 0)
    {

        /// ### added parts
        float newSepVel = - rel_v_norm * restitution;
        cout << "newSepVel " << newSepVel << endl;

        if (newSepVel < 10.0f * fabs(dt * ExternalForce_neg.y * norm.y) )
        {
            newSepVel = 10.0f * fabs(dt * ExternalForce_neg.y * norm.y);
        }

        float deltaVel = newSepVel - rel_v_norm;
        cout << "deltaVel " << deltaVel << endl;
        float totalInverseMass = 2;
        float impulse = deltaVel / totalInverseMass;
        glm::vec3 impulsePerIMass = impulse * norm;

        if (upperBall == 2)
        {

            h_obj2->w_Velocity.x += offset_x;
            h_obj2->w_Velocity.z += offset_z;
        }
        else
        {
            h_obj1->w_Velocity.x += offset_x;
            h_obj1->w_Velocity.z += offset_z;
        }
        h_obj1->w_Velocity += (impulsePerIMass/h_obj1->m_fmass);
        h_obj2->w_Velocity -= (impulsePerIMass/h_obj2->m_fmass);

        /// ### added parts
    }
  //  else if (rel_v_norm < 0.0001)
#endif
}





hash_t CollisionDetection_HGrid_NEW::ComputeHashBucketIndex(WorldObject* h_obj)
{

    hash_t hash_key;

    // in a binary tree, level 0 is the root node, so we will use that notation
    unsigned short int level = HGRID_MAX_LEVELS;

    // getting the size of the bounding volume of this particle
    /// for AABB, we get the longest size of the three
    float size = GetBoundingVolumeSize(h_obj);



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
    hash_key.data.x = (short int)floor(h_obj->w_Position.x/ cell_size);
    hash_key.data.y = (short int)floor(h_obj->w_Position.y/ cell_size);
    hash_key.data.z = (short int)floor(h_obj->w_Position.z/ cell_size);

/*
    hash_key.data.x = (short int)h_obj->w_Position.x;
    hash_key.data.y = (short int)h_obj->w_Position.y;
    hash_key.data.z = (short int)h_obj->w_Position.z;
  */
    hash_key.data.level = (unsigned short int)level;

    return hash_key;

}


hash_t CollisionDetection_HGrid_NEW::ComputeHashBucketIndex(int x, int y, int z, int level)
{
    hash_t hash_key;
    hash_key.data.x = (short int)x;
    hash_key.data.y = (short int)y;
    hash_key.data.z = (short int)z;
    hash_key.data.level = (unsigned short int)level;
    return hash_key;
}


void CollisionDetection_HGrid_NEW::AddParticleToHGrid( WorldObject* h_obj )
{
#if 1
    // Compute the hashvalue
    hash_t hash = ComputeHashBucketIndex(h_obj);

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
    h_obj->w_hash.value = hash.value;
//    h_obj->m_level = hash.data.level;
/*
    cout << "id is " << h_obj->m_id << endl;
    cout << "hash.x " << h_obj->m_hash.data.x << endl;
    cout << "hash.y " << h_obj->m_hash.data.y << endl;
    cout << "hahs.z " << h_obj->m_hash.data.z << endl;
*/
    // add to the unordered_map
    if ( myMap.find(hash.value) == myMap.end() )
    {
        h_obj->pPrevObj = NULL;
        h_obj->pNextObj = NULL;
        myMap[hash.value] = h_obj;
    }

    // if it's not empty insert at the front
    else
    {
        WorldObject* head = myMap[hash.value];
        head->pPrevObj = h_obj;
        h_obj->pNextObj = head;

        h_obj->pPrevObj = NULL;
        myMap[hash.value] = h_obj;
    }
#endif
}



void CollisionDetection_HGrid_NEW::Test_AddParticle( WorldObject* h_obj )
{

}


void CollisionDetection_HGrid_NEW::RemoveParticleFromHGrid( WorldObject*h_obj )
{
#if 1
    // Compute the hashvalue
    hash_t hash;
    hash.value = h_obj->w_hash.value;

    // if particles at that level decreases down to zero,
    // we change the occupied_Levels_Mask to zero at that bit
    if(--particle_At_Level[hash.data.level] == 0)
        occupied_Levels_Mask &= ~(1 << hash.data.level);

    WorldObject* prev = h_obj->pPrevObj;
    WorldObject* next = h_obj->pNextObj;

    // we are not the head
    if (prev)
    {
        // and if there's something behind
        if (next)
        {
            prev->pNextObj = next;
            next->pPrevObj = prev;
        }
        else
            prev->pNextObj = NULL;
    }

    // if we're the head
    else
    {
        if(next)
        {
            myMap[hash.value] = next;
            next->pPrevObj = NULL;
        }
        else
            myMap.erase(hash.value);
    }
#endif
}



void CollisionDetection_HGrid_NEW::rehash( WorldObject*h_obj )
{
    RemoveParticleFromHGrid(h_obj);
    AddParticleToHGrid(h_obj);
}




void CollisionDetection_HGrid_NEW::Draw()
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


void CollisionDetection_HGrid_NEW::drawCubeFrame(float size)
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

        glEnd();
        glEnable( GL_LIGHTING );
    glPopMatrix();
}

