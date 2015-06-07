#include "EG_Utility.h"

glm::vec3 EG_Utility::toGlmVec(aiVector3D& v2)
{
    glm::vec3 v1;
    v1.x = v2.x;
    v1.y = v2.y;
    v1.z = v2.z;
    return v1;
}

glm::mat4 EG_Utility::toGlmMat(aiMatrix3x3 m2)
{
    glm::mat4 m1;
    m1[0][0] = m2.a1; m1[0][1] = m2.a2; m1[0][2] = m2.a3; m1[0][3] = 0.0f;
    m1[1][0] = m2.b1; m1[1][1] = m2.b2; m1[1][2] = m2.b3; m1[1][3] = 0.0f;
    m1[2][0] = m2.c1; m1[2][1] = m2.c2; m1[2][2] = m2.c3; m1[2][3] = 0.0f;
    m1[3][0] = 0.0f;  m1[3][1] = 0.0f;  m1[3][2] = 0.0f;  m1[3][3] = 1.0f;
    return m1;
}

glm::mat4 EG_Utility::toGlmMat(aiMatrix4x4& m2)
{
    glm::mat4 m1;
    m1[0][0] = m2.a1; m1[0][1] = m2.a2; m1[0][2] = m2.a3; m1[0][3] = m2.a4;
    m1[1][0] = m2.b1; m1[1][1] = m2.b2; m1[1][2] = m2.b3; m1[1][3] = m2.b4;
    m1[2][0] = m2.c1; m1[2][1] = m2.c2; m1[2][2] = m2.c3; m1[2][3] = m2.c4;
    m1[3][0] = m2.d1; m1[3][1] = m2.d2; m1[3][2] = m2.d3; m1[3][3] = m2.d4;
    return m1;
}


glm::mat4 EG_Utility::toGlmMat(const aiMatrix4x4& m2)
{
    glm::mat4 m1;
    m1[0][0] = m2.a1; m1[0][1] = m2.a2; m1[0][2] = m2.a3; m1[0][3] = m2.a4;
    m1[1][0] = m2.b1; m1[1][1] = m2.b2; m1[1][2] = m2.b3; m1[1][3] = m2.b4;
    m1[2][0] = m2.c1; m1[2][1] = m2.c2; m1[2][2] = m2.c3; m1[2][3] = m2.c4;
    m1[3][0] = m2.d1; m1[3][1] = m2.d2; m1[3][2] = m2.d3; m1[3][3] = m2.d4;
    return m1;
}
