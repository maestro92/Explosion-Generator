
#include "CubeEmitter.h"
#include "gl/gl.h"
#include "gl/glu.h"

#define START_POS_X 0
#define START_POS_Y 0
#define START_POS_Z 0

#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_Z 5

float Radius = 0.1;
float start_x, start_y, start_z;
float pos_x, pos_y, pos_z;
float vel_x, vel_y, vel_z;


// this is a declared Quadrics, used to draw any bizzare shapes
GLUquadricObj* quad;

CubeEmitter::CubeEmitter()
{

    MaxRadius = 1;
    MinRadius = 0.1;
    cout << "nice" << endl;
    int size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z;

	quad = gluNewQuadric();
    ExternalForce = vec3(0,-9.81,0);

    e_ParticleBuffer.resize(size, Particle());

}


void CubeEmitter::EmitParticles( Particle& particle)
{



}



void CubeEmitter::InitParticleCube()
{
    // creates and returns a pointer to a new quadric object


   // float pos_x, pos_y, pos_z;
   // float vel_x, vel_y, vel_z;


    start_x = (START_POS_X - ARRAY_SIZE_X/2)*Radius*2;
    start_y = (START_POS_Y)*Radius*2;
    start_z = (START_POS_Z - ARRAY_SIZE_Z/2)*Radius*2;




    cout << "start_x is " << start_x << endl;
    cout << "start_y is " << start_y << endl;
    cout << "start_z is " << start_z << endl;

    for (int k=0;k<ARRAY_SIZE_Y;k++)
    {
        for (int i=0;i<ARRAY_SIZE_X;i++)
        {
            for(int j = 0;j<ARRAY_SIZE_Z;j++)
            {

                int Index = k* ARRAY_SIZE_X * ARRAY_SIZE_Z + i*ARRAY_SIZE_Z + j;

                InitParticlePos(i, k, j, Index);
                InitParticleVel(i, k, j, Index);
                InitParticleAttribute(i, k, j, Index);

            }
        }
    }

    cout << "Checking" << endl;
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        float x = e_ParticleBuffer[i].m_Position.x;
        float y = e_ParticleBuffer[i].m_Position.y;
        float z = e_ParticleBuffer[i].m_Position.z;
        cout << x << " " << y << " " << z << endl;
    }


}


void CubeEmitter::InitParticlePos(int i, int k, int j, int Index)
{

  //  float pos_x, pos_y, pos_z;
    e_ParticleBuffer[Index].m_fRadius = RandRange(MinRadius, MaxRadius); // dRadius/2;

    pos_x = start_x + Radius*2 * i;
    pos_y = start_y + Radius*2 * k;
    pos_z = start_z + Radius*2 * j;

    glm::vec3 pos( pos_x, pos_y, pos_z );
    e_ParticleBuffer[Index].m_InitPosition= pos;
    e_ParticleBuffer[Index].m_Position = pos;
    cout << "position is " << pos_x << " " << pos_y << " " << pos_z << endl;

}

void CubeEmitter::InitParticleVel(int i, int k, int j, int Index)
{
//    float vel_x, vel_y, vel_z;

    float VelMag = 10;
    // init velocity
    vel_x = RandRange(-1*VelMag,VelMag);
    vel_y = RandRange(0,VelMag*6);
    vel_z = RandRange(-1*VelMag,VelMag);

    e_ParticleBuffer[Index].m_Velocity = glm::vec3(vel_x, vel_y, vel_z);
}

void CubeEmitter::InitParticleAttribute(int i, int k, int j, int Index)
{
    float lifetime;
    lifetime = RandRange(3,5);
    e_ParticleBuffer[Index].m_fLifeTime = lifetime;
    e_ParticleBuffer[Index].m_fAge = 0;
}


// resets to initial position
void CubeEmitter::Reset()
{
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        e_ParticleBuffer[i].m_Position = e_ParticleBuffer[i].m_InitPosition;
    }
}

void CubeEmitter::UpdateParticleCube(float dt)
{
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        e_ParticleBuffer[i].m_fAge += dt;
        if (e_ParticleBuffer[i].m_Position.y >= 0)
//        if(e_ParticleBuffer[i].m_fAge < e_ParticleBuffer[i].m_fLifeTime)
        {
            e_ParticleBuffer[i].m_Position += e_ParticleBuffer[i].m_Velocity * dt;
            e_ParticleBuffer[i].m_Velocity += ExternalForce*dt;
        }


    }
}


void CubeEmitter::DrawParticleCube()
{
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {

        glPushMatrix();
            glTranslatef(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            // draw a sphere
            // Glint slices, Glint stacks: resolution
            gluSphere(quad,e_ParticleBuffer[i].m_fRadius,20,20);
        glPopMatrix();
    }

}




