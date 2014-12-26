#ifndef WORLD_CUBE
#define WORLD_CUBE


#include "EG_WorldObject.h"

#include <iostream>
#include <cstdlib>
using namespace std;

/// reason why i don't use cube is because I want a general 3D rectangles
class WorldBox : public WorldObject
{
    public:
        // Constructor
        WorldBox();
        WorldBox(float x, float y, float z);

        // for box
        virtual void setSize(float x, float y, float z);

        // set position
        virtual void setPosition(glm::vec3 pos);
        virtual void setRotation(glm::vec3 rot);
        virtual void setAngularVelocity(glm::vec3 ang_vel); // this is gonna be the axis of rotation

        virtual bool CheckObjGroundCollision(float dt);
        virtual float getBoundingVolumeSize();
        virtual void ReCalculateBoundingVolume();
        // Destructor
		virtual ~WorldBox();

        /// member variables
        /// radius or halfwidth extends(rx,ry,rz)

        glm::vec4 GroundContactPoint;

        glm::vec4 V_xyz011;   /// with just two points, we can use symmetry and derive all the other points
        glm::vec4 V_xyz111;   /// 1 means positive, 0 means negative
        glm::vec4 V_xyz101;   /// 1 means positive, 0 means negative
        glm::mat4 ModelMatrix;

        float w_rx;
        float w_ry;
        float w_rz;

        float w_scalex;
        float w_scaley;
        float w_scalez;

        /// http://stackoverflow.com/questions/6053522/how-to-recalculate-axis-aligned-bounding-box-after-translate-rotate
        /// we'll recompute the AABB of the transformed AABB
        float b_rx;
        float b_ry;
        float b_rz;
};

#endif
