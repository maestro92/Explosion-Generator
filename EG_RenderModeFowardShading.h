




/*

void ExplosionGenerator::RenderReflectiveObjects()
{
    r_Technique = &r_Reflection_Render;
    m_pipeline.pushMatrix();
    r_Technique->enableShader(RENDER_PASS1);
        m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);
      //  glUniform3f( r_Reflection_Render.CameraPosition_UniformLocation, cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);



        if(isFirstPersonCamera)
            glUniform3f( r_Reflection_Render.CameraPosition_UniformLocation, firstPersonPovCamera.getEyePoint().x,
                                                                             firstPersonPovCamera.getEyePoint().y,
                                                                             firstPersonPovCamera.getEyePoint().z);
        else
        {
            glUniform3f( r_Reflection_Render.CameraPosition_UniformLocation, thirdPersonPovCamera.m_eye.x,
                                                                             thirdPersonPovCamera.m_eye.y,
                                                                             thirdPersonPovCamera.m_eye.z);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.Dynamic_CubeMap_ColorTextureID);
        glUniform1i(r_Reflection_Render.CubeMap_UniformLocation,0);

     //   m_pipeline.updateMatrices(ReflectionShader->getProgramId());
        r_Reflection_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);
        smoothSphere->draw();

    r_Technique->disableShader(RENDER_PASS1);
    m_pipeline.popMatrix();

}

void ExplosionGenerator::RenderScene()
{
    m_pipeline.matrixMode(MODEL_MATRIX);
    GetLightPos_ModelView_Matrix();

    /// 2nd Render pass of shadowMapping: camera's point of view
    m_pipeline.matrixMode(MODEL_MATRIX);
 //   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    m_pipeline.pushMatrix();

#if USING_INVERSE_MATRIX
    g_meshOrientation = myThirdPOV_camera.m_orientation;
    g_meshOrientation = glm::inverse(g_meshOrientation);
    glm::mat4 m = glm::toMat4(g_meshOrientation);
    m_pipeline.LoadMatrix(m);
#endif

    r_Shadow_Render.enableShader(RENDER_PASS2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);

		glUniformMatrix4fv(r_Shadow_Render.LightMVPmatrix_UniLoc,1,GL_FALSE,&shadowMatrix[0][0]);
		glUniform1i(r_Shadow_Render.shadowMap_UniLoc,0);

        glUniformMatrix4fv(r_Shadow_Render.LightPosition_MVmatrix_UniLoc,1,GL_FALSE,&LightPos_modelViewMatrix[0][0]);
		glUniform3f(r_Shadow_Render.LightPosition_UniLoc,
                        lightPosition.x,
                        lightPosition.y,
                        lightPosition.z);
		glUniform3f(r_Shadow_Render.CameraPosition_UniLoc,firstPersonPovCamera.getLocation().x,firstPersonPovCamera.getLocation().y,firstPersonPovCamera.getLocation().z);

//        r_Shadow_Render.loadSpecialUniformLocation(m_pipeline, RENDER_PASS2);

    //    m_pipeline.pushMatrix();
      //  glm::vec3 EulerAngles(45, 45, 0);
      //  glm::quat rotation_quaternion = glm::quat(EulerAngles);
      //  m_pipeline.Rotate(rotation_quaternion);

//        m_pipeline.Rotate(22.5, 1, 1, 0);

        r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS2);
        ground->draw();
      //  m_pipeline.popMatrix();
//        m_pipeline.pushMatrix();
//            m_pipeline.rotateX(45);
//            m_pipeline.rotateY(45);
//            r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS2);
//            ground->draw();
//        m_pipeline.popMatrix();

#if SPHERE_EFFECT
        r_Technique = &r_Shadow_Render;
        l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS2, sphere);
#endif
#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.show(m_pipeline, shadow_SecondRender->getProgramId(), cube);
#endif

       // if(!isFirstPersonCamera)
        {
            m_pipeline.pushMatrix();
        //        m_pipeline.translate(glm::vec3(5.0f, 2.0f, 0.0f));
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
            //    m_pipeline.Rotate(myThirdPOV_camera);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_Shadow_Render.loadUniformLocations(m_pipeline, RENDER_PASS2);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

#if CUBE_SPHERE_EFFECT
        l_Cube_SphereEffect.DrawMyHgridFrames();
#endif

        r_Shadow_Render.disableShader(RENDER_PASS2);
    m_pipeline.popMatrix();
}





void ExplosionGenerator::Render_to_CubeMapTexture2()
{
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, 512, 512);

    /// for some reason if I start at i=0, the positive X face doesn't work
    for(int i=-1; i<6; i++)
    {
        Render_to_CubeMapFace2(i);
        RenderScene();
  //      glDisable(GL_DEPTH_TEST);

    #if SMOKE_EFFECT
   //     if(i==NEGATIVE_Z)
   //         RenderSmoke(false, true, ReflectionSmoke, m_skybox.Dynamic_CubeMap_DepthTextureID);
    #endif

   //     glEnable(GL_DEPTH_TEST);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
}

void ExplosionGenerator::Render_to_CubeMapFace2(int face)
{


    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();

    m_pipeline.perspective(90,       // the camera angle
                            1,              // width to height ratio
                            0.5,            // the near z clippFing coordinate
                            1000.0);


    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadIdentity();


    switch (face)
    {
        case POSITIVE_X:
            m_pipeline.rotateZ(180);
            m_pipeline.rotateY(-90);
            break;

        case NEGATIVE_X:
            m_pipeline.rotateZ(180);
            m_pipeline.rotateY(90);
            break;

        case POSITIVE_Y:
            m_pipeline.rotateX(90);
            break;

        case NEGATIVE_Y:
            m_pipeline.rotateX(-90);
            break;

        case POSITIVE_Z:
            m_pipeline.rotateZ(180);
            m_pipeline.rotateY(180);
            break;

        case NEGATIVE_Z:
            m_pipeline.rotateZ(180);
            break;
        default:
            break;
    };


    glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
///*********************************
    m_pipeline.pushMatrix();
        m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);
        ReflectionSmoke.View = m_pipeline.getViewMatrix();
    m_pipeline.popMatrix();

    m_pipeline.matrixMode(MODEL_MATRIX);
    m_pipeline.pushMatrix();
        m_pipeline.translate(0,5,0);
        m_pipeline.rotateZ(180);
        m_pipeline.scale(5);
        ReflectionSmoke.Model = m_pipeline.getModelMatrix();
        ReflectionSmoke.Modelview = ReflectionSmoke.View * ReflectionSmoke.Model;
    m_pipeline.popMatrix();

    ReflectionSmoke.Projection = m_pipeline.getProjectionMatrix();
    ReflectionSmoke.ModelviewProjection = ReflectionSmoke.Projection * ReflectionSmoke.Modelview;

    glEnable(GL_CULL_FACE);

    glBindBuffer(GL_ARRAY_BUFFER, smoke.myVbos.CubeCenter);
    glEnableVertexAttribArray(SlotPosition);
    glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    /// getting the Front and Back of the cube
    r_TwoPass_Render.Render_TwoPass_RayCasting_1(ReflectionSmoke);
//    r_TwoPass_Render.Render_TwoPass_RayCasting_1_draft(ReflectionSmoke);
    glDisable(GL_CULL_FACE);
///*********************************



  //  glClearColor(0.0,0.0,0.5,1.0);
  //  glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0,0,512,512);



    glBindFramebuffer(GL_FRAMEBUFFER, m_skybox.CubeMapFBO);



    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)face, m_skybox.Dynamic_CubeMap_ColorTextureID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_skybox.Dynamic_CubeMap_DepthTextureID, 0);

  //  if(face != POSITIVE_X)
  //  glClearColor(0.0,0.0,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    m_pipeline.matrixMode(MODEL_MATRIX);
    m_skybox.RenderSkyBox(skyboxShader, m_pipeline);

    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);

    m_pipeline.matrixMode(MODEL_MATRIX);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

}




void ExplosionGenerator::forwardRender()
{
#if REFLECTION_EFFECT
    Render_to_CubeMapTexture2();
#endif

//    getDepthTexture_FromLightPosion();
#if 1

    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();
    m_pipeline.perspective(45,SCREEN_WIDTH/SCREEN_HEIGHT, 0.5, 1000.0);

///camera motion
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	m_pipeline.matrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();




    if(isFirstPersonCamera)
    {
        firstPersonPovCamera.Control(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        firstPersonPovCamera.UpdateCamera();
        firstPersonPovCamera.UpdateCamera_Rotation(m_pipeline);
    #if SKY_BOX
        m_skybox.UpdateRotationOnly_View_Pipeline(m_pipeline);
    #endif
        firstPersonPovCamera.UpdateCamera_Translation(m_pipeline);
       // thirdPersonPovCamera.c_position = firstPersonPovCamera.getEyePoint();
        thirdPersonPovCamera.setCharacterPosition(firstPersonPovCamera.getEyePoint().x,
                                                  firstPersonPovCamera.getEyePoint().y-5,
                                                  firstPersonPovCamera.getEyePoint().z);

        thirdPersonPovCamera.setPitch(firstPersonPovCamera.getPitch());
        thirdPersonPovCamera.setYaw(firstPersonPovCamera.getYaw());
    }

    else
    {
//        thirdPersonPovCamera.Control(m_pipeline);
     //   cout << "Character Position is " << thirdPersonPovCamera.c_position.x << " "
     //                                << thirdPersonPovCamera.c_position.y << " "
     //                                << thirdPersonPovCamera.c_position.z << endl;


        thirdPersonPovCamera.Control(m_pipeline, m_skybox);

        firstPersonPovCamera.setEyePoint(thirdPersonPovCamera.c_position.x,
                                         thirdPersonPovCamera.c_position.y + 5,
                                         thirdPersonPovCamera.c_position.z);

        firstPersonPovCamera.setPitch(thirdPersonPovCamera.m_pitchDegrees);
        firstPersonPovCamera.setYaw(thirdPersonPovCamera.m_yawDegrees);
    }


    pipeline temp_pipeline;
    temp_pipeline.perspective(45, SCREEN_WIDTH/SCREEN_HEIGHT, 0.5,1000.0);
    temp_pipeline.matrixMode(MODEL_MATRIX);

    ///First render pass: light's point of view
    getDepthTexture_FromLightPosion(temp_pipeline);


#if SMOKE_EFFECT
    r_Technique = &r_DepthTexture_Render;
    /// getting the depth of the scene
    m_pipeline.matrixMode(MODEL_MATRIX);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        m_pipeline.pushMatrix();
        r_Technique->enableShader(RENDER_PASS1);

            r_DepthTexture_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);
            ground->draw();



#if SPHERE_EFFECT
            l_SphereEffect.show(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif
#if CUBE_SPHERE_EFFECT
            l_Cube_SphereEffect.show(m_pipeline, Depth_CameraRender->getProgramId(), cube);
#endif

            m_pipeline.pushMatrix();
                m_pipeline.translate(ReflectiveSphere_Pos.x, ReflectiveSphere_Pos.y, ReflectiveSphere_Pos.z);
                r_DepthTexture_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);

                smoothSphere->draw();
            m_pipeline.popMatrix();


        if(!isFirstPersonCamera)
        {
            m_pipeline.pushMatrix();
                m_pipeline.LoadMatrix(thirdPersonPovCamera.c_worldMatrix);
                m_pipeline.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
                r_DepthTexture_Render.loadUniformLocations(m_pipeline, RENDER_PASS1);
                thirdPersonPovCamera.m_character->draw();
            m_pipeline.popMatrix();
        }

        r_Technique->disableShader(RENDER_PASS1);
        m_pipeline.popMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    /// sets the color
//    glClearColor(0,0,0.5,1);
    glClearColor(0,0,0,0);      glClear(GL_COLOR_BUFFER_BIT); // clears the buffer
    glDisable(GL_DEPTH_TEST);   glDisable(GL_CULL_FACE);

#if SKY_BOX
    m_skybox.RenderSkyBox(skyboxShader);
#endif
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);    glClear(GL_DEPTH_BUFFER_BIT);





    RenderScene();





#if REFLECTION_EFFECT
    RenderReflectiveObjects();
#endif

 //   drawAxis(20, glm::vec3(0,0,0));

#if SMOKE_EFFECT
//    RenderSmoke(true, true, Matrices, gbuffer.get_depth_texture());
    RenderSmoke(true, true, Matrices, depthTexture);
#endif


#endif
}

*/


