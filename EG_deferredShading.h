





#if DEFERRED_SHADING
    r_deferredShadingPointLightPass.enableShader(RENDER_PASS1);
        r_deferredShadingPointLightPass.setPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingPointLightPass.setColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingPointLightPass.setNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        r_deferredShadingPointLightPass.setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_deferredShadingPointLightPass.disableShader(RENDER_PASS1);

    r_deferredShadingDirectionalLightPass.enableShader(RENDER_PASS1);
        r_deferredShadingDirectionalLightPass.setPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingDirectionalLightPass.setColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingDirectionalLightPass.setNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        r_deferredShadingDirectionalLightPass.setStencilTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_STENCIL);

        r_deferredShadingDirectionalLightPass.setMatSpecularIntensity(1.0f);
        r_deferredShadingDirectionalLightPass.setMatSpecularPower(32.0f);
        r_deferredShadingDirectionalLightPass.setDirectionalLight(allLights.getDirectionalLight(0));
        r_deferredShadingDirectionalLightPass.setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    r_deferredShadingDirectionalLightPass.disableShader(RENDER_PASS1);



    r_deferredShadingPointLightPass_Skybox.enableShader(RENDER_PASS1);
        r_deferredShadingPointLightPass_Skybox.setPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingPointLightPass_Skybox.setColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingPointLightPass_Skybox.setNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
     //   r_deferredShadingPointLightPass_Skybox.setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        r_deferredShadingPointLightPass_Skybox.setScreenSize(512, 512);
    r_deferredShadingPointLightPass_Skybox.disableShader(RENDER_PASS1);

    r_deferredShadingDirectionalLightPass_Skybox.enableShader(RENDER_PASS1);
        r_deferredShadingDirectionalLightPass_Skybox.setPositionTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        r_deferredShadingDirectionalLightPass_Skybox.setColorTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        r_deferredShadingDirectionalLightPass_Skybox.setNormalTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        r_deferredShadingDirectionalLightPass_Skybox.setStencilTextureUnit(EG_GBuffer::GBUFFER_TEXTURE_TYPE_STENCIL);

        r_deferredShadingDirectionalLightPass_Skybox.setMatSpecularIntensity(1.0f);
        r_deferredShadingDirectionalLightPass_Skybox.setMatSpecularPower(32.0f);
        r_deferredShadingDirectionalLightPass_Skybox.setDirectionalLight(allLights.getDirectionalLight(0));
        r_deferredShadingDirectionalLightPass_Skybox.setScreenSize(512, 512);
    r_deferredShadingDirectionalLightPass_Skybox.disableShader(RENDER_PASS1);

#else

#endif










#if 0
void ExplosionGenerator::deferredShadingShow()
{

#if REFLECTION_EFFECT
 //   deferredShadingRenderToCubeMapTexture();
#endif
 //   getDepthTexture_FromLightPosion();
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
        firstPersonPovCamera.UpdateCameraRotation(m_pipeline);
    #if SKY_BOX
        m_skybox.UpdateRotationOnly_View_Pipeline(m_pipeline);
    #endif
        firstPersonPovCamera.UpdateCameraTranslation(m_pipeline);
       // thirdPersonPovCamera.c_position = firstPersonPovCamera.getEyePoint();
  /*
        thirdPersonPovCamera.setCharacterPosition(firstPersonPovCamera.getEyePoint().x,
                                                  firstPersonPovCamera.getEyePoint().y-5,
                                                  firstPersonPovCamera.getEyePoint().z);

        thirdPersonPovCamera.setPitch(firstPersonPovCamera.getPitch());
        thirdPersonPovCamera.setYaw(firstPersonPovCamera.getYaw());
    */
    }

    else
    {
        // thirdPersonPovCamera.Control(m_pipeline, m_skybox);
        m_orbitCamera.Control(m_pipeline, m_skybox);

/*
        firstPersonPovCamera.setEyePoint(thirdPersonPovCamera.c_position.x,
                                         thirdPersonPovCamera.c_position.y + 5,
                                         thirdPersonPovCamera.c_position.z);

        firstPersonPovCamera.setPitch(thirdPersonPovCamera.m_pitchDegrees);
        firstPersonPovCamera.setYaw(thirdPersonPovCamera.m_yawDegrees);
  */
    }



    renderShadowMap();


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    /// sets the color
//    glClearColor(0,0,0.5,1);
    glClearColor(0,0,0,0);      glClear(GL_COLOR_BUFFER_BIT); // clears the buffer
    glDisable(GL_DEPTH_TEST);   glDisable(GL_CULL_FACE);


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);    glClear(GL_DEPTH_BUFFER_BIT);




    gbuffer.StartFrame37();
    deferredShadingGeometryPass37(gbuffer);

    #if MRT_DEMO_MODE
        deferredShadingMrtDemoPass();
    #else
        glEnable(GL_STENCIL_TEST);

        #if MASS_LIGHTING
            for(unsigned int i = 0; i < allLights.getPointLightsCount(); i++)
            {
                cout << "i'm here" << endl;
                deferredShadingStencilPass37(i, gbuffer);
                deferredShadingPointLightPass37(i, gbuffer);
            }
        #endif

        glDisable(GL_STENCIL_TEST);
        deferredShadingDirectionalLightPass37(gbuffer);
        deferredShadingFinalPass37(gbuffer);
        glDisable(GL_BLEND);

        #if SMOKE_EFFECT
            RenderSmoke(true, true, Matrices, gbuffer.m_textures[EG_GBuffer::GBUFFER_TEXTURE_TYPE_DEPTH]);
        #endif
    #endif



#endif
}
#endif


/*
void ExplosionGenerator::deferredShadingMrtDemoPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gbuffer.bindForReading();

    int WINDOW_WIDTH = SCREEN_WIDTH;
    int WINDOW_HEIGHT = SCREEN_HEIGHT;

    GLint HalfWidth = (GLint)(WINDOW_WIDTH / 2.0f);
    GLint HalfHeight = (GLint)(WINDOW_HEIGHT / 2.0f);

    /// look at the glBlitFramebuffer specs:   1st four param is sourceXY, 2nd four param is destXY
    /// the 9th parameter takes one of the three: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, or GL_STENCIL_BUFFER_BIT

    /// Lower Left
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);    /// 0
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    /// Upper Left
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);     /// 1
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, HalfHeight, HalfWidth, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);


    if(!isDepthTextureMode)
    {
        /// Upper Right
        gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);      /// 2
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    else
    {
        /// Upper Right
        gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_DEPTH);      /// 5
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }

    if(!isStencilTextureMode)
    {
        /// Lower Right
        gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);    /// 3
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    else
    {
        /// Lower Right
        gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_STENCIL);    /// 4
        glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
}
*/


#if 0
void ExplosionGenerator::deferredShadingGeometryPass37(EG_GBuffer& GBuffer)
{
    /// 2nd Render pass of shadowMapping: camera's point of view


    GBuffer.BindForGeomPass37();
    m_pipeline.matrixMode(MODEL_MATRIX);

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if SKY_BOX
    r_Technique = &r_deferredShadingSkybox;
    r_deferredShading = &r_deferredShadingSkybox;
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
        r_deferredShading->setStencilFlag(glm::vec3(1.0,0.0,0.0));
        r_deferredShadingSkybox.setCubeMapTextureUnit(0);
        m_skybox.render(m_pipeline, r_Technique, RENDER_PASS1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
#endif

    glEnable(GL_DEPTH_TEST);
    r_deferredShading = &r_deferredShadingReflection;


    glm::vec3 eyePoint;
    if(isFirstPersonCamera)
        eyePoint = firstPersonPovCamera.getEyePoint();
    else
    {
#if ORBIT_CAMERA_FLAG
        eyePoint = m_orbitCamera.getEyePoint();
#else
        eyePoint = thirdPersonPovCamera.getEyePoint();
#endif
    }



/*
    o_reflectiveSphere.render(m_pipeline,
                          r_deferredShadingReflection,
                          RENDER_PASS1,
                          m_skybox.Dynamic_CubeMap_ColorTextureID,
                          smoothSphere,
                          eyePoint);
*/
#if POINT_LIGHT_BULBS
    r_deferredShading = &r_deferredShadingLightPos;
    r_deferredShadingLightPos.enableShader(RENDER_PASS1);
    m_pipeline.pushMatrix();
        r_deferredShading->setStencilFlag(glm::vec3(1.0,0.0,0.0));

        for(unsigned int i = 0; i < allLights.getPointLightsCount(); i++)
        {
            m_pipeline.pushMatrix();
            EG_PointLight l = allLights.getPointLight(i);
            m_pipeline.translate(l.position);
                m_pipeline.scale(0.2,0.2,0.2);
                r_deferredShadingLightPos.setLightColor(l.color);
                r_deferredShadingLightPos.loadUniformLocations(m_pipeline, RENDER_PASS1);
                sphere->draw();
            m_pipeline.popMatrix();
        }

    m_pipeline.popMatrix();
    r_deferredShadingLightPos.disableShader(RENDER_PASS1);
#endif


    r_deferredShadingGeometryPass.enableShader(RENDER_PASS1);
    m_pipeline.pushMatrix();

        r_deferredShadingGeometryPass.setStencilFlag(glm::vec3(0.0,0.0,0.0));


        r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();

      //  glDisable(GL_CULL_FACE);
        m_pipeline.pushMatrix();
            r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
            wall_negative_z->draw();
        m_pipeline.popMatrix();

        m_pipeline.pushMatrix();
            r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
            wall_positive_x->draw();
        m_pipeline.popMatrix();

        if(toggleFlag)
        {
            m_pipeline.pushMatrix();
                r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
                o_hugeWall->draw();
            m_pipeline.popMatrix();
        }


      //  glEnable(GL_CULL_FACE);

        r_Technique = &r_deferredShadingGeometryPass;

#if SPHERE_EFFECT
        l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

      //  thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS1);

        r_deferredShadingGeometryPass.setStencilFlag(glm::vec3(1.0,0.0,0.0));
        o_worldAxis.render(m_pipeline, r_Technique, RENDER_PASS1);


        glDepthMask(GL_FALSE);
        r_deferredShadingGeometryPass.disableShader(RENDER_PASS1);

    m_pipeline.popMatrix();

}





void ExplosionGenerator::deferredShadingGeometryPass37_Skybox(EG_GBuffer& GBuffer, pipeline tempPipeline)
{
    /// 2nd Render pass of shadowMapping: camera's point of view


    GBuffer.BindForGeomPass37();
    m_pipeline.matrixMode(MODEL_MATRIX);

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


#if SKY_BOX
    r_Technique = &r_deferredShadingSkybox;
    r_deferredShading = &r_deferredShadingSkybox;
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
        r_deferredShading->setStencilFlag(glm::vec3(1.0,0.0,0.0));
        r_deferredShadingSkybox.setCubeMapTextureUnit(0);

        m_skybox.renderStatic(tempPipeline, r_Technique, RENDER_PASS1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
#endif


    m_pipeline.matrixMode(MODEL_MATRIX);
    glEnable(GL_DEPTH_TEST);

    r_deferredShadingGeometryPass.enableShader(RENDER_PASS1);
    m_pipeline.pushMatrix();


        r_deferredShadingGeometryPass.setStencilFlag(glm::vec3(0.0,0.0,0.0));


        r_deferredShadingGeometryPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        ground->draw();


        m_pipeline.pushMatrix();
            wall_negative_z->draw();
            wall_positive_x->draw();
        m_pipeline.popMatrix();


        r_Technique = &r_deferredShadingGeometryPass;

#if SPHERE_EFFECT
        l_SphereEffect.render(m_pipeline, r_Technique, RENDER_PASS1, sphere);
#endif

    //    thirdPersonPovCamera.render(m_pipeline, r_Technique, RENDER_PASS1);

        r_deferredShadingGeometryPass.setStencilFlag(glm::vec3(1.0,0.0,0.0));
        o_worldAxis.render(m_pipeline, r_Technique, RENDER_PASS1);


        glDepthMask(GL_FALSE);


    m_pipeline.popMatrix();
    r_deferredShadingGeometryPass.disableShader(RENDER_PASS1);

}
#endif

#if 0
void ExplosionGenerator::deferredShadingStencilPass37(int index, EG_GBuffer& GBuffer)
{
    r_deferredShadingStencilPass.enableShader(RENDER_PASS1);

        // Disable color/depth write and enable stencil
        GBuffer.BindForStencilPass37();
        glEnable(GL_DEPTH_TEST);

        glDisable(GL_CULL_FACE);

        glClear(GL_STENCIL_BUFFER_BIT);

        // We need the stencil test to be enabled but we want it
        // to succeed always. Only the depth test matters.
        glStencilFunc(GL_ALWAYS, 0, 0);

        /// setting front and/or back stencil test actions
        // glStencilOpSeparate is different from glStencilOp where it is specifying specifying either front or back stencil test actions
        // but of course you can set but front and back with different test actions by using two glStencilOpSeparate calls
        /// three calls
        /// sfail: stencil test fail
        /// dpfail: stencil test passes but depth test fail
        /// both stencil and depth test passes
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
//        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);
//        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);


        m_pipeline.matrixMode(MODEL_MATRIX);
        m_pipeline.pushMatrix();




        EG_PointLight l = allLights.getPointLight(index);
        glm::vec3 transMatrix = l.position;
        m_pipeline.translate(transMatrix);
        m_pipeline.scale(r_deferredShadingPointLightPass.computePointLightBoundingSphere(l));


//        m_pipeline.scale(2);
        r_deferredShadingStencilPass.loadUniformLocations(m_pipeline, RENDER_PASS1);
        pointLightSphere->draw();
        m_pipeline.popMatrix();

    r_deferredShadingStencilPass.disableShader(RENDER_PASS1);
}
#endif

/*
void ExplosionGenerator::deferredShadingPointLightPass37(int index, EG_GBuffer& GBuffer)
{
    GBuffer.BindForLightPass37();

    r_deferredShadingPointLightPass.enableShader(RENDER_PASS1);

        r_deferredShadingPointLightPass.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());


        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);


        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);


        m_pipeline.pushMatrix();
        //    cout << "I'm drawing Point Light" << endl;



        EG_PointLight l = allLights.getPointLight(index);
        glm::vec3 transMatrix = l.position;
        m_pipeline.translate(transMatrix);
        m_pipeline.scale(r_deferredShadingPointLightPass.computePointLightBoundingSphere(l));

        r_deferredShadingPointLightPass.setPointLight(l);
        r_deferredShadingPointLightPass.loadUniformLocations(m_pipeline, RENDER_PASS1);


        pointLightSphere->draw();


        glCullFace(GL_BACK);
        glDisable(GL_BLEND);

        m_pipeline.popMatrix();
    r_deferredShadingPointLightPass.disableShader(RENDER_PASS1);
}





void ExplosionGenerator::deferredShadingPointLightPass37_Skybox(int index, EG_GBuffer& GBuffer)
{
    GBuffer.BindForLightPass37();

    r_deferredShadingPointLightPass_Skybox.enableShader(RENDER_PASS1);
        r_deferredShadingPointLightPass_Skybox.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());

//        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);


        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);


        m_pipeline.pushMatrix();
        //    cout << "I'm drawing Point Light" << endl;



        EG_PointLight l = allLights.getPointLight(index);
        glm::vec3 transMatrix = l.position;
        m_pipeline.translate(transMatrix);
        m_pipeline.scale(r_deferredShadingPointLightPass_Skybox.computePointLightBoundingSphere(l));

        r_deferredShadingPointLightPass_Skybox.setPointLight(l);
        r_deferredShadingPointLightPass_Skybox.loadUniformLocations(m_pipeline, RENDER_PASS1);


        pointLightSphere->draw();


        glCullFace(GL_BACK);
        glDisable(GL_BLEND);

        m_pipeline.popMatrix();
    r_deferredShadingPointLightPass_Skybox.disableShader(RENDER_PASS1);
}


void ExplosionGenerator::deferredShadingDirectionalLightPass37(EG_GBuffer& GBuffer)
{
    r_deferredShadingDirectionalLightPass.m_shaders[RENDER_PASS1]->useShader();
        GBuffer.BindForLightPass37();

        pipeline tempPipeline = m_pipeline;
        tempPipeline.loadIdentity();

        tempPipeline.matrixMode(PROJECTION_MATRIX);
        tempPipeline.pushMatrix();
        tempPipeline.ortho(-1,1,-1,1,-1,1);


        r_deferredShadingDirectionalLightPass.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());


        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, shadowMap);

        r_deferredShadingDirectionalLightPass.setShadowMapTextureUnit(6);
        r_deferredShadingDirectionalLightPass.loadUniformLocations(tempPipeline, RENDER_PASS1);

//        o_fullScreenQuad.quad->draw(r_deferredShadingDirectionalLightPass.m_shaders[RENDER_PASS1]->getProgramId());
        tempPipeline.popMatrix();


        glDisable(GL_BLEND);

	r_deferredShadingDirectionalLightPass.m_shaders[RENDER_PASS1]->delShader();
}


void ExplosionGenerator::deferredShadingDirectionalLightPass37_Skybox(EG_GBuffer& GBuffer)
{
    r_deferredShadingDirectionalLightPass_Skybox.m_shaders[RENDER_PASS1]->useShader();
        GBuffer.BindForLightPass37();

        pipeline tempPipeline = m_pipeline;
        tempPipeline.loadIdentity();

        tempPipeline.matrixMode(PROJECTION_MATRIX);
        tempPipeline.pushMatrix();
        tempPipeline.ortho(-1,1,-1,1,-1,1);


        r_deferredShadingDirectionalLightPass_Skybox.setEyeWorldPos(thirdPersonPovCamera.getEyePoint());


        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, shadowMap);

        r_deferredShadingDirectionalLightPass.setShadowMapTextureUnit(6);
        r_deferredShadingDirectionalLightPass_Skybox.loadUniformLocations(tempPipeline, RENDER_PASS1);

//        o_fullScreenQuad.quad->draw(r_deferredShadingDirectionalLightPass_Skybox.m_shaders[RENDER_PASS1]->getProgramId());
        tempPipeline.popMatrix();


        glDisable(GL_BLEND);

	r_deferredShadingDirectionalLightPass.m_shaders[RENDER_PASS1]->delShader();
}

void ExplosionGenerator::deferredShadingFinalPass37(EG_GBuffer& GBuffer)
{
    GBuffer.BindForFinalPass37();
    glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                      0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}




void ExplosionGenerator::deferredShadingRenderToCubeMapTexture()
{
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, 512, 512);

    /// for some reason if I start at i=0, the positive X face doesn't work
//    for(int i=-1; i<6; i++)
//    for(int i=-1; i<6; i++)
    for(int i=0; i<6; i++)
    {
//        if( i == NEGATIVE_Z || i == POSITIVE_X || i == NEGATIVE_X)
  //      if( i == NEGATIVE_Z )
     //   if( i != POSITIVE_X)
        {
            deferredShadingRenderToCubeMapTextureFace(i);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
}

*/
#if 0
void ExplosionGenerator::deferredShadingRenderToCubeMapTextureFace(int face)
{
    m_pipeline.matrixMode(PROJECTION_MATRIX);
    m_pipeline.loadIdentity();

    m_pipeline.perspective(90,       // the camera angle
                            1,              // width to height ratio
                            0.5,            // the near z clippFing coordinate
                            1000.0);


    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.loadIdentity();

    m_pipeline.RotateForReflection(face);

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

    glViewport(0,0,512,512);






    skyboxGBuffer.StartFrame37();

    glClear(GL_COLOR_BUFFER_BIT);

    pipeline tempPipeline = m_pipeline;

    m_pipeline.matrixMode(MODEL_MATRIX);
    tempPipeline.matrixMode(MODEL_MATRIX);
 //   m_skybox.RenderSkyBox(skyboxShader, m_pipeline);

    m_pipeline.matrixMode(VIEW_MATRIX);
    m_pipeline.translate(ReflectiveSphere_Pos.x,ReflectiveSphere_Pos.y,ReflectiveSphere_Pos.z);

    m_pipeline.matrixMode(MODEL_MATRIX);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);



    deferredShadingGeometryPass37_Skybox(skyboxGBuffer, tempPipeline);

    glEnable(GL_STENCIL_TEST);
    for(unsigned int i = 0; i < allLights.getPointLightsCount(); i++)
    {
        cout << "i'm here" << endl;
        deferredShadingStencilPass37(i, skyboxGBuffer);
        deferredShadingPointLightPass37_Skybox(i, skyboxGBuffer);
    }
    glDisable(GL_STENCIL_TEST);


    deferredShadingDirectionalLightPass37_Skybox(skyboxGBuffer);


    #if SMOKE_EFFECT
        if(face==NEGATIVE_Z)
            RenderSmoke(false, true, ReflectionSmoke, skyboxGBuffer.m_textures[EG_GBuffer::GBUFFER_TEXTURE_TYPE_DEPTH]);
    #endif



    glDepthMask(GL_TRUE);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_skybox.CubeMapFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)face, m_skybox.Dynamic_CubeMap_ColorTextureID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_skybox.Dynamic_CubeMap_DepthTextureID, 0);


	// set our G_Buffer as the source
    glBindFramebuffer(GL_READ_FRAMEBUFFER, skyboxGBuffer.m_fbo);
#if 1
    glReadBuffer(GL_COLOR_ATTACHMENT7);
    glBlitFramebuffer(0, 0, 512, 512,
                      0, 0, 512, 512, GL_COLOR_BUFFER_BIT, GL_LINEAR);
#else
    /// Upper Left
    gbuffer.setReadBuffer(EG_GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);     /// 1
    glBlitFramebuffer(0, 0, 512, 512,
                      0, 0, 512, 512,  GL_COLOR_BUFFER_BIT, GL_LINEAR);
#endif


}
#endif





