
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

    m_maxVelocity = 5.0f;
    m_maxRadius = 1.0;
    m_minRadius = 0.5;
    cout << "nice" << endl;
    int size;
    e_ParticleBuffer.reserve(1000);
    m_particleWVPMatrices.reserve(1000);
    m_particleWorldMatrices.reserve(1000);

    if (Ball2Ball_CollisionMode)
        size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z + 1;
    else
        size = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z;

	quad = gluNewQuadric();
	m_particlesCount = ARRAY_SIZE_X * ARRAY_SIZE_Y * ARRAY_SIZE_Z;
    e_ParticleBuffer.resize(size, h_Particle());
    m_particleWVPMatrices.resize(size);
    m_particleWorldMatrices.resize(size);


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
                    myHgrid.addParticleToHGrid( &e_ParticleBuffer[Index]);
                else
                    myHgrid.rehash(&e_ParticleBuffer[Index]);
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

     //   e_ParticleBuffer[Index].m_Pre_Velocity = glm::vec3(1000,1000,1000);
        e_ParticleBuffer[Index].m_fRadius = testRadius; // dRadius/2;
        float lifetime;
        lifetime = RandRange(3,5);
        e_ParticleBuffer[Index].m_fLifeTime = lifetime;
        e_ParticleBuffer[Index].m_fAge = 0;
        e_ParticleBuffer[Index].m_id = Index;

        if (!reset)
        {
            myHgrid.addParticleToHGrid( &e_ParticleBuffer[Index]);
            myHgrid.ParticleCollisionPairList.clear();
        }

        else
            myHgrid.rehash(&e_ParticleBuffer[Index]);
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

#endif





void EG_SphereParticleEffect::InitParticles(int count)
{
    e_ParticleBuffer.clear();
    m_particleWVPMatrices.clear();
    m_particleWorldMatrices.clear();

    m_particlesCount = count;
    int size;
    if (Ball2Ball_CollisionMode)
        m_particlesCount = count + 1;
    else
        m_particlesCount = count;

    e_ParticleBuffer.resize(m_particlesCount, h_Particle());
    m_particleWVPMatrices.resize(m_particlesCount);
    m_particleWorldMatrices.resize(m_particlesCount);




    int side = pow(count, 1.0/3.0);
    if(count > (side * side * side))
        side = side + 1;



    start_x = -(m_maxRadius * 2 * side)/2 + m_maxRadius;
    start_y = (START_POS_Y)*Radius*2;
    start_z = -(m_maxRadius * 2 * side)/2 + m_maxRadius;
    int Index = 0;

    cout << "start_x is " << start_x << endl;
    cout << "start_y is " << start_y << endl;
    cout << "start_z is " << start_z << endl;

    int currentCount = 0;

    for (int k=0; currentCount<count; k++)
    {
        for (int i=0;i<side;i++)
        {
            for(int j = 0;j<side;j++)
            {
                Index = k* side * side + i*side + j;
                InitParticleAttribute(i, k, j, Index);
                InitParticlePos(i, k, j, Index);
                InitParticleVel(i, k, j, Index);

                    myHgrid.addParticleToHGrid( &e_ParticleBuffer[Index]);


                currentCount++;
                if(currentCount >= count)
                    break;
            }
            if(currentCount >= count)
                break;
        }
        if(currentCount >= count)
            break;
    }



    cout << "Checking" << endl;
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {

        float x = e_ParticleBuffer[i].m_Position.x;
        float y = e_ParticleBuffer[i].m_Position.y;
        float z = e_ParticleBuffer[i].m_Position.z;
        cout << x << " " << y << " " << z << endl;

        myHgrid.Test_AddParticle(&e_ParticleBuffer[i]);

    }



}





void EG_SphereParticleEffect::ExamineParticleAttribute()
{

    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        cout << "id is " << e_ParticleBuffer[i].m_id << endl;
   //     cout << "level is " << e_ParticleBuffer[i].m_level << endl;
    }

}



void EG_SphereParticleEffect::InitParticlePos(int i, int k, int j, int Index)
{

/*
    pos_x = start_x + Radius*2 * i;
    pos_y = start_y + Radius*2 * k;
    pos_z = start_z + Radius*2 * j;
*/
    int offset_y = m_maxRadius + 0.5;
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
    e_ParticleBuffer[Index].m_Position = pos;
  //  cout << "position is " << pos_x << " " << pos_y << " " << pos_z << endl;

}




void EG_SphereParticleEffect::InitParticleVel(int i, int k, int j, int Index)
{

//    float vel_x, vel_y, vel_z;
    if (!Ball2Ball_CollisionMode)
    {
        /*
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

        */

        vel_x = RandRange(-m_maxVelocity, m_maxVelocity);
        vel_y = RandRange(0, m_maxVelocity*m_maxVelocity);
        vel_z = RandRange(-m_maxVelocity, m_maxVelocity);


#if 0
        vel_x = 0;
        vel_y = 0;
        vel_z = 0;
#endif
        e_ParticleBuffer[Index].m_Velocity = glm::vec3(vel_x, vel_y, vel_z);
    }
    else
    {

        vel_x = 0;
        vel_y = 0;
        vel_z = 0;

        e_ParticleBuffer[Index].m_Velocity = glm::vec3(vel_x, vel_y, vel_z);
    }


//    e_ParticleBuffer[Index].m_Pre_Velocity = glm::vec3(1000,1000,1000);

}

void EG_SphereParticleEffect::InitParticleAttribute(int i, int k, int j, int Index)
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
  //  e_ParticleBuffer[Index].m_fRadius = 0.5;
//    e_ParticleBuffer[Index].m_fRadius = testRadius;

    float lifetime;
    lifetime = RandRange(3,5);
    e_ParticleBuffer[Index].m_fRadius = RandRange(m_minRadius, m_maxRadius); // dRadius/2;
    e_ParticleBuffer[Index].m_fLifeTime = lifetime;
    e_ParticleBuffer[Index].m_fAge = 0;
    e_ParticleBuffer[Index].m_id = Index;
    // Index
}


// resets to initial position
void EG_SphereParticleEffect::Reset()
{
    myHgrid.clear();
//    InitParticles(m_count, true);
    InitParticles(m_particlesCount);
}





void EG_SphereParticleEffect::resetParticleVelocity()
{
    // cout << "Checking" << endl;
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        vel_x = RandRange(-m_maxVelocity, m_maxVelocity);
        vel_y = RandRange(0, m_maxVelocity*m_maxVelocity);
        vel_z = RandRange(-m_maxVelocity, m_maxVelocity);

        e_ParticleBuffer[i].m_Velocity = glm::vec3(vel_x, vel_y, vel_z);
    }

}



/* updating the particle */
void EG_SphereParticleEffect::UpdateParticleEffect(float dt)
{
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        e_ParticleBuffer[i].m_fAge += dt;


        myHgrid.removeParticleFromHGrid(&e_ParticleBuffer[i]);

        // check collision here?
        // check collision with the plane

        cout << "   ## id is " << e_ParticleBuffer[i].m_id << endl;
     //   cout << "   ## real time vel x is " << e_ParticleBuffer[i].m_Velocity.x << endl;
        cout << "   ## real time vel y is " << e_ParticleBuffer[i].m_Velocity.y << endl;
     //   cout << "   ## real time vel z is " << e_ParticleBuffer[i].m_Velocity.z << endl;
        cout << "   ## real time pos y is " << e_ParticleBuffer[i].m_Position.y - e_ParticleBuffer[i].m_fRadius << endl;


        // check collision
        // if there's no collision just let it update regularly
#if 0
        if ( ! myHgrid.CheckCollision( &e_ParticleBuffer[i], dt ) )
        {
            e_ParticleBuffer[i].m_Position += (e_ParticleBuffer[i].m_Velocity + dt * ExternalForce_half_neg )* dt;
            e_ParticleBuffer[i].m_Velocity += ExternalForce_neg*dt;
        }
#endif


#if 1
        myHgrid.CheckCollision( &e_ParticleBuffer[i], dt );
        e_ParticleBuffer[i].m_Velocity += ExternalForce_neg*dt;
        e_ParticleBuffer[i].m_Position += (e_ParticleBuffer[i].m_Velocity * dt);

#endif

/*
        myHgrid.CheckCollision( &e_ParticleBuffer[i], dt );
        e_ParticleBuffer[i].m_Position += (e_ParticleBuffer[i].m_Velocity + dt * ExternalForce_half_neg )* dt;
        e_ParticleBuffer[i].m_Velocity += ExternalForce_neg*dt;
*/

        myHgrid.addParticleToHGrid(&e_ParticleBuffer[i]);



    }
    // clear the collision pairs
    myHgrid.ParticleCollisionPairList.clear();

    cout << endl;
}




/*
// http://stackoverflow.com/questions/8494942/why-does-my-color-go-away-when-i-enable-lighting-in-opengl
void EG_SphereParticleEffect::DrawParticleCube(pipeline &m_pipeline,  unsigned int shaderID , meshLoader* mymesh)
{
    m_pipeline.matrixMode(MODEL_MATRIX);
    // static GLuint spheresList=0, torusList=0, baseList=0;
    // glEnable(GL_COLOR_MATERIAL);
  //  myshader->useShader();
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {

        m_pipeline.pushMatrix();

            m_pipeline.translate(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            m_pipeline.scale(e_ParticleBuffer[i].m_fRadius);
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
*/




void EG_SphereParticleEffect::update(bool toggle)
{
    UpdateParticleEffect(0.031);
 //   UpdateParticleCube(0.015);
 //   UpdateParticleCube(0.015);
  //   UpdateParticleCube(0.01);
}

/*
void EG_SphereParticleEffect::show(pipeline &m_pipeline, Technique* Renderer, int RenderTypeID,
                                int RenderPassID, meshLoader* mymesh)
{
    Technique* mytech;
    switch(RenderTypeID)
    {
        case SHADOW_RENDER:
            mytech = (Technique_Shadow_Render*) Renderer;
            break;
        case TWOPASS_RAYCASTING_RENDER:
            mytech = (Technique_TwoPass_Raycasting*) Renderer;
            break;
    }


    m_pipeline.matrixMode(MODEL_MATRIX);

    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            m_pipeline.scale(e_ParticleBuffer[i].m_fRadius);
            mytech->Setup_Matrix_forRender(m_pipeline, RenderPassID);
            ((Technique_Shadow_Render*)mytech)->Setup_ShadowMatrix_forRender(m_pipeline, RenderPassID);
      //      m_pipeline.updateMatrices(shaderID);
      //      m_pipeline.updateShadowMatrix(shaderID);

            mymesh->draw();
        m_pipeline.popMatrix();
    }

}
*/


void EG_SphereParticleEffect::show(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID, meshLoader* mymesh)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            m_pipeline.scale(e_ParticleBuffer[i].m_fRadius);

      //      Renderer->Setup_Matrix_forRender(m_pipeline, RenderPassID);
//            Renderer->Load_glUniform(m_pipeline, RenderPassID);

            Renderer->loadUniformLocations(m_pipeline, RenderPassID);
/*
            if(RenderTypeID == SHADOW_RENDER)
                ((Technique_Shadow_Render*)Renderer)->loadSpecialUniformLocation(m_pipeline, RenderPassID);
*/
            mymesh->draw();
//          mymesh->draw(Renderer->progShaders[RenderPassID]->getProgramId());
        m_pipeline.popMatrix();
    }
}

void EG_SphereParticleEffect::render(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID, meshLoader* mymesh)
{
    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            m_pipeline.scale(e_ParticleBuffer[i].m_fRadius);
            Renderer->loadUniformLocations(m_pipeline, RenderPassID);
            mymesh->draw();
        m_pipeline.popMatrix();
    }

}



void EG_SphereParticleEffect::render(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID, EG_Model& model)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            m_pipeline.scale(e_ParticleBuffer[i].m_fRadius);
            Renderer->loadUniformLocations(m_pipeline, RenderPassID);
            model.render();
        m_pipeline.popMatrix();
    }
}





void EG_SphereParticleEffect::updateMatrices(pipeline &m_pipeline)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            m_pipeline.scale(e_ParticleBuffer[i].m_fRadius);

            m_particleWVPMatrices[i] = m_pipeline.getModelViewProjectionMatrixForInstancedRendering();
            m_particleWorldMatrices[i] = m_pipeline.getModelMatrix();

        m_pipeline.popMatrix();
    }
}


void EG_SphereParticleEffect::instancedRender(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID, EG_InstancedModel& model)
{
    Renderer->enableShader(RenderPassID);
        Renderer->loadUniformLocations(m_pipeline, RenderPassID);
        // convert vector to pointer, http://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array-c
        model.render(getCount(), &m_particleWVPMatrices[0], &m_particleWorldMatrices[0]);
    Renderer->disableShader(RenderPassID);
}

/*
void EG_SphereParticleEffect::instancedRender(pipeline &m_pipeline, EG_Renderer* Renderer, int RenderPassID, EG_InstancedModel& model)
{
    m_pipeline.matrixMode(MODEL_MATRIX);

    for(int i = 0; i < e_ParticleBuffer.size(); i++)
    {
        m_pipeline.pushMatrix();
            m_pipeline.translate(e_ParticleBuffer[i].m_Position.x, e_ParticleBuffer[i].m_Position.y, e_ParticleBuffer[i].m_Position.z);
            m_pipeline.scale(e_ParticleBuffer[i].m_fRadius);
            Renderer->loadUniformLocations(m_pipeline, RenderPassID);
            model.render();
        m_pipeline.popMatrix();
    }
}
*/



int EG_SphereParticleEffect::getCount()
{
    return e_ParticleBuffer.size();
}




