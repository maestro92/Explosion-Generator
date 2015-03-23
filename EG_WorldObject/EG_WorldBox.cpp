#include "EG_WorldBox.h"



WorldBox::WorldBox()
{
    obj_type = BOX;
    w_rx = 1.0f;
    w_ry = 1.0f;
    w_rz = 1.0f;

    b_rx = 1.0f;
    b_ry = 1.0f;
    b_rz = 1.0f;
    ModelMatrix = glm::mat4(1.0);
    w_AngularVelocity = glm::vec3(0.0,0.0,0.0);
    w_Rotation = glm::vec3(0.0,0.0,0.0);
}



WorldBox::WorldBox(float x, float y, float z)
{
    obj_type = BOX;
    w_rx = x;
    w_ry = y;
    w_rz = z;

    b_rx = x;
    b_ry = y;
    b_rz = z;
    ModelMatrix = glm::mat4(1.0);
}


WorldBox::~WorldBox()
{

}



void WorldBox::setSize(float x, float y, float z)
{
    w_rx = x;
    w_ry = y;
    w_rz = z;

    b_rx = x;
    b_ry = y;
    b_rz = z;

    w_InertiaTensor = mat3(0.0);
    w_InertiaTensor[0][0] = (4*y*y + 4*z*z)/12.0f;
    w_InertiaTensor[1][1] = (4*x*x + 4*z*z)/12.0f;
    w_InertiaTensor[2][2] = (4*x*x + 4*y*y)/12.0f;

    w_InverseInertiaTensor = glm::inverse(w_InertiaTensor);

    V_xyz011 = vec4(1.0);
    V_xyz111 = vec4(1.0);
    V_xyz101 = vec4(1.0);

    V_xyz011.x = -w_rx;
    V_xyz011.y = w_ry;
    V_xyz011.z = w_rz;

    V_xyz111.x = w_rx;
    V_xyz111.y = w_ry;
    V_xyz111.z = w_rz;

    V_xyz101.x = w_rx;
    V_xyz101.y = -w_ry;
    V_xyz101.z = w_rz;

}



void WorldBox::setRotation(glm::vec3 rot)
{
 //   if(rot.x >= 360) rot.x = rot.x-360.0f;
  //  if(rot.y >= 360) rot.x = rot.y-360.0f;
  //  if(rot.z >= 360) rot.x = rot.z-360.0f;


    w_Rotation = rot;

//    glm::mat4 tra_mat = glm::translate(w_Position.x, w_Position.y, w_Position.z);
//    glm::mat4 tra_mat = glm::translate(0.0f, 10.0f, 0.0f);
/*
    cout << tra_mat[0][0] << " " << tra_mat[0][1] << " " << tra_mat[0][2] << " " << tra_mat[0][3] << endl;
    cout << tra_mat[1][0] << " " << tra_mat[1][1] << " " << tra_mat[1][2] << " " << tra_mat[1][3] << endl;
    cout << tra_mat[2][0] << " " << tra_mat[2][1] << " " << tra_mat[2][2] << " " << tra_mat[2][3] << endl;
    cout << tra_mat[3][0] << " " << tra_mat[3][1] << " " << tra_mat[3][2] << " " << tra_mat[3][3] << endl;
*/

/// for references
    glm::mat4 rot_mat = glm::mat4(1.0);
    if (rot.x != 0.0f)
        rot_mat *= glm::rotate(rot.x,1.0f,0.0f,0.0f);
    if (rot.y != 0.0f)
        rot_mat *= glm::rotate(rot.y,0.0f,1.0f,0.0f);
    if (rot.z != 0.0f)
        rot_mat *= glm::rotate(rot.z,0.0f,0.0f,1.0f);


    cout << rot_mat[0][0] << " " << rot_mat[0][1] << " " << rot_mat[0][2] << " " << rot_mat[0][3] << endl;
    cout << rot_mat[1][0] << " " << rot_mat[1][1] << " " << rot_mat[1][2] << " " << rot_mat[1][3] << endl;
    cout << rot_mat[2][0] << " " << rot_mat[2][1] << " " << rot_mat[2][2] << " " << rot_mat[2][3] << endl;
    cout << rot_mat[3][0] << " " << rot_mat[3][1] << " " << rot_mat[3][2] << " " << rot_mat[3][3] << endl;

    V_xyz011 = vec4(1.0);
    V_xyz111 = vec4(1.0);
    V_xyz101 = vec4(1.0);

    V_xyz011.x = -w_rx;
    V_xyz011.y = w_ry;
    V_xyz011.z = w_rz;

    V_xyz111.x = w_rx;
    V_xyz111.y = w_ry;
    V_xyz111.z = w_rz;

    V_xyz101.x = w_rx;
    V_xyz101.y = -w_ry;
    V_xyz101.z = w_rz;

    ModelMatrix = rot_mat;
 //   cout << "position is " << w_Position.x + w_Position.x<< " " << w_Position.y + w_Position.y << " " << w_Position.z + w_Position.z << endl;
 //   cout << "V_xyz111 is " << V_xyz111.x + w_Position.x << " " << V_xyz111.y + w_Position.y << " " << V_xyz111.z + w_Position.z << endl;

    V_xyz111 = ModelMatrix * V_xyz111;
    cout << "V_xyz111 is " << V_xyz111.x << " " << V_xyz111.y << " " << V_xyz111.z << endl;

    V_xyz011 = ModelMatrix * V_xyz011;
    cout << "V_xyz011 is " << V_xyz011.x << " " << V_xyz011.y << " " << V_xyz011.z << endl;

    V_xyz101 = ModelMatrix * V_xyz101;
    cout << "V_xyz101 is " << V_xyz101.x << " " << V_xyz101.y << " " << V_xyz101.z << endl;


    /// recalculate the bounding volume
    b_rx = std::max(fabs(V_xyz101.x), std::max(fabs(V_xyz111.x), fabs(V_xyz011.x)));
    b_rz = std::max(fabs(V_xyz101.z), std::max(fabs(V_xyz111.z), fabs(V_xyz011.z)));


    if(fabs(V_xyz111.y) > fabs(V_xyz011.y))
        GroundContactPoint = V_xyz111;
    else
        GroundContactPoint = V_xyz011;

    if(fabs(V_xyz101.y) > fabs(GroundContactPoint.y))
        GroundContactPoint = V_xyz101;

    if(GroundContactPoint.y > 0)
        GroundContactPoint.y *= -1.0f;
//    cout << "GroundContactPoint " << GroundContactPoint.x << ", " << GroundContactPoint.y << ", " << GroundContactPoint.z << endl;

    b_ry = fabs(GroundContactPoint.y);

    GroundContactPoint.x += w_Position.x;
    GroundContactPoint.y += w_Position.y;
    GroundContactPoint.z += w_Position.z;

    GroundContactPoint.x = 0;
 //   cout << "After GroundContactPoint " << GroundContactPoint.x << ", " << GroundContactPoint.y << ", " << GroundContactPoint.z << endl;
    cout << "b_rx " << b_rx << ", b_ry " << b_ry << ", b_rz " << b_rz <<endl<<endl;
 //   cout << std::max(b_rx,(std::max(b_ry, b_rz))) << endl;
}

// this is gonna be the axis of rotation
void WorldBox::setAngularVelocity(glm::vec3 ang_vel)
{
    w_AngularVelocity = ang_vel;
}

bool WorldBox::CheckObjGroundCollision(float dt)
{
    cout << "Checking: b_ry is " << b_ry << endl;

    if ( (w_Position.y + (w_Velocity.y + ExternalForce_neg1.y*dt)* dt - b_ry)<= 0.0f && w_Velocity.y <= 0.001)
    {
        return true;
    }
    else
        return false;
}



float WorldBox::getBoundingVolumeSize()
{
    /// in set Rotation, we already used fabs(), so we ensure b_rx is positive
    float max_dim = std::max(b_rx,(std::max(b_ry, b_rz))) * 2.0f;
    cout << "max_dim: " << max_dim << endl;
    return max_dim;
}

void WorldBox::ReCalculateBoundingVolume()
{


}
