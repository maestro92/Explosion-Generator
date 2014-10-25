
#include "L_CubeEmitterEffect.h"
#include "gl/gl.h"
#include "gl/glu.h"

#define START_POS_X 0
#define START_POS_Y 0
#define START_POS_Z 0

#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_Z 5

float Radius = 1;
float start_x, start_y, start_z;
float pos_x, pos_y, pos_z;
float vel_x, vel_y, vel_z;




// this is a declared Quadrics, used to draw any bizzare shapesz
GLUquadricObj* quad;

L_CubeEmitterEffect::L_CubeEmitterEffect()
{
    Ball2Ball_CollisionMode = true;
    first = true;
    MaxRadius = 0.5;
    MinRadius = 2;
    cout << "nice" << endl;
    int size;
    if (Ball2Ball_CollisionMode)
        size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z + 1;
    else
        size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z;

	quad = gluNewQuadric();
//    ExternalForce = vec3(0,0,0);
    ExternalForce_neg = vec3(0,-9.8,0);
    ExternalForce_pos = vec3(0,9.8,0);
    ExternalForce_half_neg = vec3(0,-4.9,0);
    ExternalForce_half_pos = vec3(0,4.9,0);
    e_ParticleBuffer.resize(size, h_Particle());


/*
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        myHgrid.AddParticleToHGrid( &l_CubeEffect.e_ParticleBuffer[i]);
    }

    cout << "       #### end basic adding" << endl;

    int i = 0;
    for(i = 0; i < l_CubeEffect.e_ParticleBuffer.size(); i++)
    {
      //  myCD_Hgrid.Test_AddParticle(&l_CubeEffect.e_ParticleBuffer[i]);
    }
    cout << endl << "i is " << i << endl << endl;

       myHgrid.Test_AddParticle(&l_CubeEffect.e_ParticleBuffer[0]);
*/


}






void L_CubeEmitterEffect::InitParticleCube(bool reset)
{
    // creates and returns a pointer to a new quadric object


   // float pos_x, pos_y, pos_z;
   // float vel_x, vel_y, vel_z;


    start_x = (START_POS_X - ARRAY_SIZE_X/2)*Radius*2;
    start_y = (START_POS_Y)*Radius*2;
    start_z = (START_POS_Z - ARRAY_SIZE_Z/2)*Radius*2;
    int Index = 0;

    cout << "start_x is " << start_x << endl;
    cout << "start_y is " << start_y << endl;
    cout << "start_z is " << start_z << endl;

    for (int k=0;k<ARRAY_SIZE_Y;k++)
    {
        for (int i=0;i<ARRAY_SIZE_X;i++)
        {
            for(int j = 0;j<ARRAY_SIZE_Z;j++)
            {

                Index = k* ARRAY_SIZE_X * ARRAY_SIZE_Z + i*ARRAY_SIZE_Z + j;
                InitParticleAttribute(i, k, j, Index);
                InitParticlePos(i, k, j, Index);
                InitParticleVel(i, k, j, Index);


                if (!reset)
                    myHgrid.AddParticleToHGrid( &e_ParticleBuffer[Index]);
                else
                {
                    myHgrid.rehash(&e_ParticleBuffer[Index]);
                }
            }
        }
    }


    // init my attacker grid
    if (Ball2Ball_CollisionMode)
    {
        Index = e_ParticleBuffer.size()-1;
        // e_ParticleBuffer[Index].m_Position = glm::vec3(-30, 30, -30);
        // float scale = 20.0f;
        // e_ParticleBuffer[Index].m_Velocity = glm::vec3(scale, -scale, scale);

        e_ParticleBuffer[Index].m_Position = glm::vec3(-30, 3, -0);
        float scale = 30.0f;
        e_ParticleBuffer[Index].m_Velocity = glm::vec3(scale, 0, 0);
      //  e_ParticleBuffer[Index].m_Velocity = glm::vec3(scale, -scale+10, 0);
        //e_ParticleBuffer[Index].m_Velocity = glm::vec3(scale, 0, 0);

        e_ParticleBuffer[Index].m_Pre_Velocity = glm::vec3(1000,1000,1000);
        e_ParticleBuffer[Index].m_fRadius = 2; // dRadius/2;
        float lifetime;
        lifetime = RandRange(3,5);
        e_ParticleBuffer[Index].m_fLifeTime = lifetime;
        e_ParticleBuffer[Index].m_fAge = 0;
        e_ParticleBuffer[Index].m_id = Index;

        if (!reset)
        {
            myHgrid.AddParticleToHGrid( &e_ParticleBuffer[Index]);
            myHgrid.ParticleCollisionPairList.clear();
        }

        else
        {
            myHgrid.rehash(&e_ParticleBuffer[Index]);
        }
    }



    cout << "Checking" << endl;
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        float x = e_ParticleBuffer[i].m_Position.x;
        float y = e_ParticleBuffer[i].m_Position.y;
        float z = e_ParticleBuffer[i].m_Position.z;
  //      cout << x << " " << y << " " << z << endl;


        myHgrid.Test_AddParticle(&e_ParticleBuffer[i]);

    }



}




void L_CubeEmitterEffect::ExamineParticleAttribute()
{

    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        cout << "id is " << e_ParticleBuffer[i].m_id << endl;
   //     cout << "level is " << e_ParticleBuffer[i].m_level << endl;
    }

}



void L_CubeEmitterEffect::InitParticlePos(int i, int k, int j, int Index)
{

/*
    pos_x = start_x + Radius*2 * i;
    pos_y = start_y + Radius*2 * k;
    pos_z = start_z + Radius*2 * j;
*/
    int offset_y = 2;

    pos_x = start_x + Radius*5 * i;
    pos_y = start_y + Radius*5 * k + offset_y;
    pos_z = start_z + Radius*5 * j;

    if (!Ball2Ball_CollisionMode)
    {
        if (Index == 1)
            pos_y = 6;
    }

    glm::vec3 pos( pos_x, pos_y, pos_z );
    e_ParticleBuffer[Index].m_Position = pos;
  //  cout << "position is " << pos_x << " " << pos_y << " " << pos_z << endl;

}




void L_CubeEmitterEffect::InitParticleVel(int i, int k, int j, int Index)
{

//    float vel_x, vel_y, vel_z;
    if (!Ball2Ball_CollisionMode)
    {
        float VelMag = 10;
        float scale = 0.5;
        // init velocity
    //    vel_x = RandRange(-scale*VelMag,VelMag);
    //    vel_y = RandRange(0,VelMag*2);
    //    vel_z = RandRange(-scale*VelMag,scale*VelMag);

        vel_x = RandRange(-scale*VelMag,scale*VelMag);
        vel_y = RandRange(VelMag*2,VelMag*2);
        vel_z = RandRange(-scale*VelMag,scale*VelMag);

        vel_x = 0;
        vel_y = 0;
        vel_z = 0;

        e_ParticleBuffer[Index].m_Velocity = glm::vec3(vel_x, vel_y, vel_z);
    }
    else
    {

        vel_x = 0;
        vel_y = 0;
        vel_z = 0;

        e_ParticleBuffer[Index].m_Velocity = glm::vec3(vel_x, vel_y, vel_z);
    }


    e_ParticleBuffer[Index].m_Pre_Velocity = glm::vec3(1000,1000,1000);

}

void L_CubeEmitterEffect::InitParticleAttribute(int i, int k, int j, int Index)
{
    // Radius
    /*
    if (Index < 3)
        e_ParticleBuffer[Index].m_fRadius = 1; // dRadius/2;
    else if (Index >= 3 && Index < 6)
        e_ParticleBuffer[Index].m_fRadius = 2; // dRadius/2;
    else
        e_ParticleBuffer[Index].m_fRadius = 3; // dRadius/2;
*/
    e_ParticleBuffer[Index].m_fRadius = 2;


    float lifetime;
    lifetime = RandRange(3,5);
 //   e_ParticleBuffer[Index].m_fRadius = RandRange(MinRadius, MaxRadius); // dRadius/2;
    e_ParticleBuffer[Index].m_fLifeTime = lifetime;
    e_ParticleBuffer[Index].m_fAge = 0;
    e_ParticleBuffer[Index].m_id = Index;
    // Index
}


// resets to initial position
void L_CubeEmitterEffect::Reset()
{
    InitParticleCube(true);
}


/* updating the particle */
void L_CubeEmitterEffect::UpdateParticleCube(float dt)
{
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        e_ParticleBuffer[i].m_fAge += dt;


   //     myHgrid.RemoveParticleFromHGrid(&e_ParticleBuffer[i]);

/*
        if (!first)
        {
            e_ParticleBuffer[i].m_Position = e_ParticleBuffer[i].m_NextPosition;
            e_ParticleBuffer[i].m_Velocity = e_ParticleBuffer[i].m_NextVelocity;
        }
*/

        cout << "   ## id is " << e_ParticleBuffer[i].m_id << endl;
        cout << "   ## real time vel x is " << e_ParticleBuffer[i].m_Velocity.x << endl;
        cout << "   ## real time vel y is " << e_ParticleBuffer[i].m_Velocity.y << endl;
        cout << "   ## real time vel z is " << e_ParticleBuffer[i].m_Velocity.z << endl;
        cout << "   ## real time pos y is " << e_ParticleBuffer[i].m_Position.y - e_ParticleBuffer[i].m_fRadius << endl;



        // check collision
        // if there's no collision just let it update regularly
        if ( ! myHgrid.CheckCollision( &e_ParticleBuffer[i], dt ) )
        {
//            e_ParticleBuffer[i].m_Position += e_ParticleBuffer[i].m_Velocity * dt;
      //      glm::vec3 old_vel = e_ParticleBuffer[i].m_Velocity;


            //      e_ParticleBuffer[i].m_prev_Position = e_ParticleBuffer[i].m_Position;
      //      e_ParticleBuffer[i].m_Position += (e_ParticleBuffer[i].m_Velocity + dt * ExternalForce_half_neg )* dt;
      //      e_ParticleBuffer[i].m_Velocity += ExternalForce_neg*dt;
            //      e_ParticleBuffer[i].m_Position += (old_vel + e_ParticleBuffer[i].m_Velocity)* 0.5f * dt;

            e_ParticleBuffer[i].m_NextPosition = e_ParticleBuffer[i].m_Position + (e_ParticleBuffer[i].m_Velocity + dt * ExternalForce_half_neg )* dt;
            e_ParticleBuffer[i].m_NextVelocity = e_ParticleBuffer[i].m_Velocity + ExternalForce_neg*dt;

        }

   //     myHgrid.AddParticleToHGrid(&e_ParticleBuffer[i]);

    }
    // clear the collision pairs
    myHgrid.ParticleCollisionPairList.clear();

    cout << endl;
}



void L_CubeEmitterEffect::UpdateNextTimeStamp(float dt)
{
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {

        myHgrid.RemoveParticleFromHGrid(&e_ParticleBuffer[i]);




        e_ParticleBuffer[i].m_Position = e_ParticleBuffer[i].m_NextPosition;
        e_ParticleBuffer[i].m_Velocity = e_ParticleBuffer[i].m_NextVelocity;


        myHgrid.AddParticleToHGrid(&e_ParticleBuffer[i]);

    }
    // clear the collision pairs
    myHgrid.ParticleCollisionPairList.clear();

    cout << endl;

}




// http://stackoverflow.com/questions/8494942/why-does-my-color-go-away-when-i-enable-lighting-in-opengl
void L_CubeEmitterEffect::DrawParticleCube()
{

    glEnable(GL_COLOR_MATERIAL);
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {

        glPushMatrix();
            glTranslatef(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            // draw a sphere
            // Glint slices, Glint stacks: resolution
            glColor3f(0,0, 5 * 0.125);
         //   glColor3f(0,0,e_ParticleBuffer[i].m_id * 0.125);
            gluSphere(quad,e_ParticleBuffer[i].m_fRadius,20,20);
        glPopMatrix();
    }
    glDisable(GL_COLOR_MATERIAL);

    // drawing the cubes
    myHgrid.Draw();
}


void L_CubeEmitterEffect::update(bool toggle)
{
    UpdateParticleCube(0.031);
    UpdateNextTimeStamp(0.031);
    // UpdateParticleCube(0.01);
}

void L_CubeEmitterEffect::show(bool toggle)
{
    DrawParticleCube();
}


