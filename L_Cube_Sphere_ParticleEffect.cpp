

#include "L_Cube_Sphere_ParticleEffect.h"

#define START_POS_X 0
#define START_POS_Y 0
#define START_POS_Z 0

#define ARRAY_SIZE_X 1
#define ARRAY_SIZE_Y 1
#define ARRAY_SIZE_Z 1


float Radius = 1;
float start_x, start_y, start_z;
float pos_x, pos_y, pos_z;
float vel_x, vel_y, vel_z;




// this is a declared Quadrics, used to draw any bizzare shapesz


L_Cube_Sphere_ParticleEffect::L_Cube_Sphere_ParticleEffect()
{
    testRadius = 2.0;
    CollisionMode = false;

    MaxX = 1; MinX = 1;
    MaxY = 1; MinY = 1;
    MaxZ = 1; MinZ = 1;

    int size;
    if (CollisionMode)
        size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z + 1;
    else
        size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z;

    e_ParticleBuffer.resize(size);
}




void L_Cube_Sphere_ParticleEffect::InitParticle(bool reset)
{
    // creates and returns a pointer to a new quadric object
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

                if (!reset)
                    e_ParticleBuffer[Index] = new WorldBox();


                InitParticlePos(i, k, j, Index);
                InitParticleVel(i, k, j, Index);
                InitParticleAttribute(i, k, j, Index);

                if (!reset)
                    myHgrid.AddParticleToHGrid( e_ParticleBuffer[Index]);
                else
                    myHgrid.rehash(e_ParticleBuffer[Index]);

       //         cout << "hash value is for index " << Index << " " << e_ParticleBuffer[Index]->w_hash.value << endl;
            }
        }
    }
}





void L_Cube_Sphere_ParticleEffect::InitParticle(int flag, float angle, bool reset)
{
    // creates and returns a pointer to a new quadric object
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

                if (!reset)
                    e_ParticleBuffer[Index] = new WorldBox();


                InitParticlePos(i, k, j, Index);
                InitParticleVel(i, k, j, Index);
                InitParticleAttribute(i, k, j, Index, flag, angle);

                if (!reset)
                    myHgrid.AddParticleToHGrid( e_ParticleBuffer[Index]);
                else
                    myHgrid.rehash(e_ParticleBuffer[Index]);
       //         cout << "hash value is for index " << Index << " " << e_ParticleBuffer[Index]->w_hash.value << endl;
            }
        }
    }
}





void L_Cube_Sphere_ParticleEffect::ExamineParticleAttribute()
{

    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {

   //     cout << "level is " << e_ParticleBuffer[i].m_level << endl;
    }

}



void L_Cube_Sphere_ParticleEffect::InitParticlePos(int i, int k, int j, int Index)
{
    int offset_y = 3;
    int Gap = 5;

    int offset_x = 0;
    int offset_z = 0;

    pos_x = start_x + Radius*Gap * i + offset_x;
    pos_y = start_y + Radius*Gap * k + offset_y;
    pos_z = start_z + Radius*Gap * j + offset_z;


    glm::vec3 pos( pos_x, pos_y, pos_z );
    e_ParticleBuffer[Index]->setPosition(pos);
  //  cout << "position is " << pos_x << " " << pos_y << " " << pos_z << endl;

}




void L_Cube_Sphere_ParticleEffect::InitParticleVel(int i, int k, int j, int Index)
{
        float VelMag = 5;
        float scale = 1.0;
        float y_scale = 2.0f;
#if 1
    //    vel_x = RandRange(-scale*VelMag,scale*VelMag);
        vel_y = RandRange(0,y_scale*VelMag);
   //     vel_z = RandRange(-scale*VelMag,scale*VelMag);
#else
        vel_x = 0;
        vel_y = 0;
        vel_z = 0;
#endif
        e_ParticleBuffer[Index]->setVelocity(glm::vec3(vel_x, vel_y, vel_z));
}



void L_Cube_Sphere_ParticleEffect::InitParticleAttribute(int i, int k, int j, int Index)
{
    e_ParticleBuffer[Index]->setSize(0.5f,0.5f,0.5f);
    e_ParticleBuffer[Index]->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    int temp = RandRange(0,3);
    float R_angle = RandRange(0,180);
  //  int temp = 0;
  //  float R_angle = 65.60f;
    cout << "temp is " << temp << ", angle is " << R_angle << endl;
    if(temp>=0 && temp<1)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(R_angle, 0.0f, 0.0f));
    if(temp>=1 && temp<2)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(0.0f, R_angle, 0.0f));
    if(temp>=2 && temp<3)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(0.0f, 0.0f, R_angle));
}


void L_Cube_Sphere_ParticleEffect::InitParticleAttribute(int i, int k, int j, int Index, int flag, float angle)
{
 //   e_ParticleBuffer[Index]->setSize(0.5f,0.5f,0.5f);
    e_ParticleBuffer[Index]->setSize(0.5f,0.5f,0.5f);

    int temp = RandRange(0,3);
    float R_angle = RandRange(0,45);
//    float R_angle = 47.8497;
    cout << "temp is " << temp << ", angle is " << R_angle << endl;
/*
    if(temp>=0 && temp<1)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(R_angle, 0.0f, 0.0f));
    if(temp>=1 && temp<2)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(0.0f, R_angle, 0.0f));
    if(temp>=2 && temp<3)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(0.0f, 0.0f, R_angle));
*/

    if(flag == 1)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(R_angle, 0.0f, 0.0f));
    if(flag == 2)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(0.0f, R_angle, 0.0f));
    if(flag == 3)
        e_ParticleBuffer[Index]->setRotation(glm::vec3(0.0f, 0.0f, R_angle));

}


// resets to initial position
void L_Cube_Sphere_ParticleEffect::Reset()
{
    InitParticle(true);
}


// resets to initial position
void L_Cube_Sphere_ParticleEffect::Reset(int flag, float angle)
{
    InitParticle(flag, angle, true);
}



/* updating the particle */
void L_Cube_Sphere_ParticleEffect::UpdateParticleEffect(float dt)
{
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        myHgrid.RemoveParticleFromHGrid(e_ParticleBuffer[i]);



        myHgrid.CheckCollision( e_ParticleBuffer[i], dt );
        e_ParticleBuffer[i]->w_Velocity += ExternalForce_neg*dt;
        e_ParticleBuffer[i]->w_Position += (e_ParticleBuffer[i]->w_Velocity * dt);

        cout << "AngularVelocity " << e_ParticleBuffer[i]->w_AngularVelocity.x << endl;
        e_ParticleBuffer[i]->w_Rotation += (e_ParticleBuffer[i]->w_AngularVelocity * dt);
        e_ParticleBuffer[i]->setRotation(e_ParticleBuffer[i]->w_Rotation);


        //       e_ParticleBuffer[i].ReCalculateBoundingVolume();
        myHgrid.AddParticleToHGrid(e_ParticleBuffer[i]);

    }
}



/*

// http://stackoverflow.com/questions/8494942/why-does-my-color-go-away-when-i-enable-lighting-in-opengl
void L_Cube_Sphere_ParticleEffect::DrawParticles(pipeline &m_pipeline, unsigned int shaderID, meshLoader* mymesh)
{
    m_pipeline.matrixMode(MODEL_MATRIX);
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        m_pipeline.pushMatrix();

            m_pipeline.translate(e_ParticleBuffer[i]->w_Position.x, e_ParticleBuffer[i]->w_Position.y, e_ParticleBuffer[i]->w_Position.z);

            m_pipeline.rotateX(e_ParticleBuffer[i]->w_Rotation.x);
            m_pipeline.rotateY(e_ParticleBuffer[i]->w_Rotation.y);
            m_pipeline.rotateZ(e_ParticleBuffer[i]->w_Rotation.z);

            m_pipeline.scale(((WorldBox*)e_ParticleBuffer[i])->w_rx);
            m_pipeline.updateMatrices(shaderID);
            m_pipeline.updateShadowMatrix(shaderID);
            mymesh->draw(shaderID);
        m_pipeline.popMatrix();
    }
}
*/


void L_Cube_Sphere_ParticleEffect::DrawMyHgridFrames()
{
    myHgrid.Draw();
}

void L_Cube_Sphere_ParticleEffect::update(bool toggle)
{
    UpdateParticleEffect(0.031);
}

void L_Cube_Sphere_ParticleEffect::show(pipeline &m_pipeline,  unsigned int shaderID , meshLoader* mymesh)
{
//    DrawParticles(m_pipeline, shaderID, mymesh);
}

void L_Cube_Sphere_ParticleEffect::show(bool toggle)
{

}

