
#include "EG_SphereParticleEffect.h"
#include "gl/gl.h"
#include "gl/glu.h"

#define START_POS_X 0
#define START_POS_Y 0
#define START_POS_Z 0

#define ARRAY_SIZE_X 4
#define ARRAY_SIZE_Y 4
#define ARRAY_SIZE_Z 4


// this is a declared Quadrics, used to draw any bizzare shapesz


const float PARTICLE_SPACE = 2.0f;

EG_SphereParticleEffect::EG_SphereParticleEffect()
{
    Radius = 1;
    testRadius = 2.0;
    Ball2Ball_CollisionMode = false;

    m_maxRadius = 1.0;
    m_minRadius = 0.5;

    cout << "nice" << endl;
    int size;
    if (Ball2Ball_CollisionMode)
        size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z + 1;
    else
        size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z;

    m_particles.resize(size, h_Particle());


/*
    for(int i = 0; i < m_particles.size(); i++)
    {
        myHgrid.AddParticleToHGrid( &l_CubeEffect.m_particles[i]);
    }

    cout << "       #### end basic adding" << endl;

    int i = 0;
    for(i = 0; i < l_CubeEffect.m_particles.size(); i++)
    {
      //  myCD_Hgrid.Test_AddParticle(&l_CubeEffect.m_particles[i]);
    }
    cout << endl << "i is " << i << endl << endl;

       myHgrid.Test_AddParticle(&l_CubeEffect.m_particles[0]);
*/


}





#if 0
void EG_SphereParticleEffect::InitParticles(bool reset)
{
    // creates and returns a pointer to a new quadric object


   // float pos_x, pos_y, pos_z;
   // float vel_x, vel_y, vel_z;

/*
    start_x = (START_POS_X - ARRAY_SIZE_X/2)*Radius*2;
    start_y = (START_POS_Y)*Radius*2;
    start_z = (START_POS_Z - ARRAY_SIZE_Z/2)*Radius*2;
  */

    start_x = -(m_maxRadius * 2 * ARRAY_SIZE_X)/2 + m_maxRadius;
    start_y = (START_POS_Y)*Radius*2;
    start_z = -(m_maxRadius * 2 * ARRAY_SIZE_Z)/2 + m_maxRadius;



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
                    myHgrid.addParticleToHGrid( &m_particles[Index]);
                else
                    myHgrid.rehash(&m_particles[Index]);
            }
        }
    }


    // init my attacker grid
    if (Ball2Ball_CollisionMode)
    {
        Index = m_particles.size()-1;
        // m_particles[Index].m_Position = glm::vec3(-30, 30, -30);
        // float scale = 20.0f;
        // m_particles[Index].m_Velocity = glm::vec3(scale, -scale, scale);

        m_particles[Index].m_Position = glm::vec3(-30, 3, -0);
        float scale = 30.0f;
        m_particles[Index].m_Velocity = glm::vec3(scale, 0, 0);
      //  m_particles[Index].m_Velocity = glm::vec3(scale, -scale+10, 0);
        //m_particles[Index].m_Velocity = glm::vec3(scale, 0, 0);

     //   m_particles[Index].m_Pre_Velocity = glm::vec3(1000,1000,1000);
        m_particles[Index].m_fRadius = testRadius; // dRadius/2;
        float lifetime;
        lifetime = RandRange(3,5);
        m_particles[Index].m_fLifeTime = lifetime;
        m_particles[Index].m_fAge = 0;
        m_particles[Index].m_id = Index;

        if (!reset)
        {
            myHgrid.addParticleToHGrid( &m_particles[Index]);
            myHgrid.ParticleCollisionPairList.clear();
        }

        else
            myHgrid.rehash(&m_particles[Index]);
    }



    cout << "Checking" << endl;
    for(int i = 0; i < m_particles.size(); i++)
    {
        float x = m_particles[i].m_Position.x;
        float y = m_particles[i].m_Position.y;
        float z = m_particles[i].m_Position.z;
  //      cout << x << " " << y << " " << z << endl;


        myHgrid.Test_AddParticle(&m_particles[i]);
    }
}

#endif





void EG_SphereParticleEffect::InitParticles(int count, bool reset)
{
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
                    myHgrid.addParticleToHGrid( &m_particles[Index]);
                else
                    myHgrid.rehash(&m_particles[Index]);
            }
        }
    }


    // init my attacker grid
    if (Ball2Ball_CollisionMode)
    {
        Index = m_particles.size()-1;
        // m_particles[Index].m_Position = glm::vec3(-30, 30, -30);
        // float scale = 20.0f;
        // m_particles[Index].m_Velocity = glm::vec3(scale, -scale, scale);

        m_particles[Index].m_Position = glm::vec3(-30, 3, -0);
        float scale = 30.0f;
        m_particles[Index].m_Velocity = glm::vec3(scale, 0, 0);
      //  m_particles[Index].m_Velocity = glm::vec3(scale, -scale+10, 0);
        //m_particles[Index].m_Velocity = glm::vec3(scale, 0, 0);

     //   m_particles[Index].m_Pre_Velocity = glm::vec3(1000,1000,1000);
        m_particles[Index].m_fRadius = testRadius; // dRadius/2;
        float lifetime;
        lifetime = RandRange(3,5);
        m_particles[Index].m_fLifeTime = lifetime;
        m_particles[Index].m_fAge = 0;
        m_particles[Index].m_id = Index;

        if (!reset)
        {
            myHgrid.addParticleToHGrid( &m_particles[Index]);
            myHgrid.ParticleCollisionPairList.clear();
        }

        else
            myHgrid.rehash(&m_particles[Index]);
    }



    cout << "Checking" << endl;
    for(int i = 0; i < m_particles.size(); i++)
    {
        float x = m_particles[i].m_Position.x;
        float y = m_particles[i].m_Position.y;
        float z = m_particles[i].m_Position.z;
  //      cout << x << " " << y << " " << z << endl;


        myHgrid.Test_AddParticle(&m_particles[i]);

    }


}




void EG_SphereParticleEffect::ExamineParticleAttribute()
{

    for(int i = 0; i < m_particles.size(); i++)
    {
        cout << "id is " << m_particles[i].m_id << endl;
   //     cout << "level is " << m_particles[i].m_level << endl;
    }

}



void EG_SphereParticleEffect::InitParticlePos(int i, int k, int j, int Index)
{

/*
    pos_x = start_x + Radius*2 * i;
    pos_y = start_y + Radius*2 * k;
    pos_z = start_z + Radius*2 * j;
*/
    int offset_y = 1.5;
    int Gap = 2;

    int offset_x = 0;
    int offset_z = 0;
//    int offset_x = RandRange(-2, 2);
//    int offset_z = RandRange(-2, 2);
/*
    pos_x = start_x + Radius*Gap * i + offset_x;
    pos_y = start_y + Radius*Gap * k + offset_y;
    pos_z = start_z + Radius*Gap * j + offset_z;
*/

    pos_x = start_x + m_maxRadius * 2 * i + offset_x;
    pos_y = start_y + m_maxRadius * 2 * k + offset_y;
    pos_z = start_z + m_maxRadius * 2 * j + offset_z;




/*
    if (!Ball2Ball_CollisionMode)
    {
        if (Index == 1)
            pos_y = 6;
        if (Index == 2)
            pos_y = 20;
    }
*/


    glm::vec3 pos( pos_x, pos_y, pos_z );
    m_particles[Index].m_Position = pos;
  //  cout << "position is " << pos_x << " " << pos_y << " " << pos_z << endl;

}




void EG_SphereParticleEffect::InitParticleVel(int i, int k, int j, int Index)
{

//    float vel_x, vel_y, vel_z;
    if (!Ball2Ball_CollisionMode)
    {
        float VelMag = 5;
        float scale = 1.0;
        float y_scale = 5.0f;
        // init velocity
    //    vel_x = RandRange(-scale*VelMag,VelMag);
    //    vel_y = RandRange(0,VelMag*2);
    //    vel_z = RandRange(-scale*VelMag,scale*VelMag);

        vel_x = RandRange(-scale*VelMag,scale*VelMag);
        vel_y = RandRange(0,y_scale*VelMag);
        vel_z = RandRange(-scale*VelMag,scale*VelMag);
#if 0
        vel_x = 0;
        vel_y = 0;
        vel_z = 0;
#endif
        m_particles[Index].m_Velocity = glm::vec3(vel_x, vel_y, vel_z);
    }
    else
    {

        vel_x = 0;
        vel_y = 0;
        vel_z = 0;

        m_particles[Index].m_Velocity = glm::vec3(vel_x, vel_y, vel_z);
    }


//    m_particles[Index].m_Pre_Velocity = glm::vec3(1000,1000,1000);

}

void EG_SphereParticleEffect::InitParticleAttribute(int i, int k, int j, int Index)
{
    // Radius
    /*
    if (Index < 3)
        m_particles[Index].m_fRadius = 1; // dRadius/2;
    else if (Index >= 3 && Index < 6)
        m_particles[Index].m_fRadius = 2; // dRadius/2;
    else
        m_particles[Index].m_fRadius = 3; // dRadius/2;
*/
  //  m_particles[Index].m_fRadius = 0.5;
//    m_particles[Index].m_fRadius = testRadius;

    float lifetime;
    lifetime = RandRange(3,5);
    m_particles[Index].m_fRadius = RandRange(m_minRadius, m_maxRadius); // dRadius/2;
    m_particles[Index].m_fLifeTime = lifetime;
    m_particles[Index].m_fAge = 0;
    m_particles[Index].m_id = Index;
    // Index
}


// resets to initial position
void EG_SphereParticleEffect::Reset()
{
    InitParticles(true);
}


/* updating the particle */
void EG_SphereParticleEffect::UpdateParticleEffect(float dt)
{
    for(int i = 0; i < m_particles.size(); i++)
    {
        m_particles[i].m_fAge += dt;


        myHgrid.removeParticleFromHGrid(&m_particles[i]);

        // check collision here?
        // check collision with the plane

        cout << "   ## id is " << m_particles[i].m_id << endl;
     //   cout << "   ## real time vel x is " << m_particles[i].m_Velocity.x << endl;
        cout << "   ## real time vel y is " << m_particles[i].m_Velocity.y << endl;
     //   cout << "   ## real time vel z is " << m_particles[i].m_Velocity.z << endl;
        cout << "   ## real time pos y is " << m_particles[i].m_Position.y - m_particles[i].m_fRadius << endl;


        // check collision
        // if there's no collision just let it update regularly
#if 0
        if ( ! myHgrid.CheckCollision( &m_particles[i], dt ) )
        {
            m_particles[i].m_Position += (m_particles[i].m_Velocity + dt * ExternalForce_half_neg )* dt;
            m_particles[i].m_Velocity += ExternalForce_neg*dt;
        }
#endif


#if 1
        myHgrid.CheckCollision( &m_particles[i], dt );
        m_particles[i].m_Velocity += ExternalForce_neg*dt;
        m_particles[i].m_Position += (m_particles[i].m_Velocity * dt);

#endif

/*
        myHgrid.CheckCollision( &m_particles[i], dt );
        m_particles[i].m_Position += (m_particles[i].m_Velocity + dt * ExternalForce_half_neg )* dt;
        m_particles[i].m_Velocity += ExternalForce_neg*dt;
*/

        myHgrid.addParticleToHGrid(&m_particles[i]);



    }
    // clear the collision pairs
    myHgrid.ParticleCollisionPairList.clear();

    cout << endl;
}





// http://stackoverflow.com/questions/8494942/why-does-my-color-go-away-when-i-enable-lighting-in-opengl
void EG_SphereParticleEffect::DrawParticleCube(pipeline &m_pipeline,  unsigned int shaderID , meshLoader* mymesh)
{
    m_pipeline.matrixMode(MODEL_MATRIX);
    // static GLuint spheresList=0, torusList=0, baseList=0;
    // glEnable(GL_COLOR_MATERIAL);
  //  myshader->useShader();
    for(int i = 0; i < m_particles.size(); i++)
    {

        m_pipeline.pushMatrix();

            m_pipeline.translate(m_particles[i].m_Position.x, m_particles[i].m_Position.y, m_particles[i].m_Position.z);
            m_pipeline.scale(m_particles[i].m_fRadius);
            m_pipeline.updateMatrices(shaderID);
            m_pipeline.updateShadowMatrix(shaderID);
            mymesh->draw(shaderID);
        m_pipeline.popMatrix();
    }
 //   glDisable(GL_COLOR_MATERIAL);

   // myshader->delShader();
    // drawing the cubes
 //   myHgrid.Draw();
}





void EG_SphereParticleEffect::update(bool toggle)
{
    UpdateParticleEffect(0.031);
 //   UpdateParticleCube(0.015);
 //   UpdateParticleCube(0.015);
  //   UpdateParticleCube(0.01);
}

/*
void EG_SphereParticleEffect::show(pipeline &m_pipeline, Technique* RenderTechnique, int RenderTypeID,
                                int RenderPassID, meshLoader* mymesh)
{
    Technique* mytech;
    switch(RenderTypeID)
    {
        case SHADOW_RENDER:
            mytech = (Technique_Shadow_Render*) RenderTechnique;
            break;
        case TWOPASS_RAYCASTING_RENDER:
            mytech = (Technique_TwoPass_Raycasting*) RenderTechnique;
            break;
    }


    m_pipeline.matrixMode(MODEL_MATRIX);

    for(int i = 0; i < m_particles.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(m_particles[i].m_Position.x, m_particles[i].m_Position.y, m_particles[i].m_Position.z);
            m_pipeline.scale(m_particles[i].m_fRadius);
            mytech->Setup_Matrix_forRender(m_pipeline, RenderPassID);
            ((Technique_Shadow_Render*)mytech)->Setup_ShadowMatrix_forRender(m_pipeline, RenderPassID);
      //      m_pipeline.updateMatrices(shaderID);
      //      m_pipeline.updateShadowMatrix(shaderID);

            mymesh->draw();
        m_pipeline.popMatrix();
    }

}
*/


void EG_SphereParticleEffect::show(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID, meshLoader* mymesh)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    for(int i = 0; i < m_particles.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(m_particles[i].m_Position.x, m_particles[i].m_Position.y, m_particles[i].m_Position.z);
            m_pipeline.scale(m_particles[i].m_fRadius);

      //      m_pipeline.updateMatrices(shaderID);
      //      m_pipeline.updateShadowMatrix(shaderID);

      //      RenderTechnique->Setup_Matrix_forRender(m_pipeline, RenderPassID);
//            RenderTechnique->Load_glUniform(m_pipeline, RenderPassID);

            RenderTechnique->loadUniformLocations(m_pipeline, RenderPassID);
/*
            if(RenderTypeID == SHADOW_RENDER)
                ((Technique_Shadow_Render*)RenderTechnique)->loadSpecialUniformLocation(m_pipeline, RenderPassID);
*/
            mymesh->draw();
//          mymesh->draw(RenderTechnique->progShaders[RenderPassID]->getProgramId());
        m_pipeline.popMatrix();
    }
}

void EG_SphereParticleEffect::render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID, meshLoader* mymesh)
{
    for(int i = 0; i < m_particles.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(m_particles[i].m_Position.x, m_particles[i].m_Position.y, m_particles[i].m_Position.z);
            m_pipeline.scale(m_particles[i].m_fRadius);
            RenderTechnique->loadUniformLocations(m_pipeline, RenderPassID);
            mymesh->draw();
        m_pipeline.popMatrix();
    }

}



void EG_SphereParticleEffect::render(pipeline &m_pipeline, EG_RenderTechnique* RenderTechnique, int RenderPassID, EG_Model& model)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    for(int i = 0; i < m_particles.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(m_particles[i].m_Position.x, m_particles[i].m_Position.y, m_particles[i].m_Position.z);
            m_pipeline.scale(m_particles[i].m_fRadius);
            RenderTechnique->loadUniformLocations(m_pipeline, RenderPassID);
            model.render();
        m_pipeline.popMatrix();
    }
}






