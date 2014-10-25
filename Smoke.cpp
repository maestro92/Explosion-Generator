
#include "Smoke.h"
#define SWAP(x0,x) {double * tmp=x0;x0=x;x=tmp;}


glm::vec3 ImpulsePosition( GridWidth / 2.0f, GridHeight - (int) SplatRadius / 2.0f, GridDepth / 2.0f);

Smoke::Smoke()
{

}

Smoke::~Smoke()
{

}


void Smoke::init()
{

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    Eulerian3D_Raycast = new shader("3D_Eulerian_Raycast.vs", "3D_Eulerian_Raycast.gs", "3D_Eulerian_Raycast.fs");

    Eulerian3D_Fill = new shader("3D_Eulerian_Vertex.vs", "3D_Eulerian_Fill.fs");
    Eulerian3D_Advect = new shader("3D_Eulerian_Vertex.vs", "3D_Eulerian_PickLayer.gs", "3D_Eulerian_Advect.fs");
    Eulerian3D_Jacobi = new shader("3D_Eulerian_Vertex.vs", "3D_Eulerian_PickLayer.gs", "3D_Eulerian_Jacobi.fs");
    Eulerian3D_SubtractGradient = new shader("3D_Eulerian_Vertex.vs", "3D_Eulerian_PickLayer.gs", "3D_Eulerian_SubtractGradient.fs");
    Eulerian3D_ComputeDivergence = new shader("3D_Eulerian_Vertex.vs", "3D_Eulerian_PickLayer.gs", "3D_Eulerian_ComputeDivergence.fs");
    Eulerian3D_ApplyImpulse = new shader("3D_Eulerian_Vertex.vs", "3D_Eulerian_PickLayer.gs", "3D_Eulerian_ApplyImpulse.fs");
    Eulerian3D_ApplyBuoyancy = new shader("3D_Eulerian_Vertex.vs", "3D_Eulerian_PickLayer.gs", "3D_Eulerian_Buoyancy.fs");


    myVbos.CubeCenter = CreatePointVbo(0,0,0);
    myVbos.FullscreenQuad = CreateQuadVbo();

/*
    Slabs.Velocity = CreateSlab3D_SameFBO(GridWidth, GridHeight, GridDepth, 3);
    Slabs.Density = CreateSlab3D_SameFBO(GridWidth, GridHeight, GridDepth, 1);
    Slabs.Pressure = CreateSlab3D_SameFBO(GridWidth, GridHeight, GridDepth, 1);
    Slabs.Temperature = CreateSlab3D_SameFBO(GridWidth, GridHeight, GridDepth, 1);
    Surfaces.Divergence = CreateVolume_SameFBO(GridWidth, GridHeight, GridDepth, 3);

    Eulerian3D_Fill->useShader();
    Surfaces.Obstacles = CreateVolume_SameFBO(GridWidth, GridHeight, GridDepth, 3);
    CreateObstacles_SameFBO(Surfaces.Obstacles);
    ClearSurface(Slabs.Temperature.Ping, AmbientTemperature);
*/

    f_Slab.Velocity = CreateSlab3D_SameFBO(GridWidth, GridHeight, GridDepth, 3);
    f_Slab.Density = CreateSlab3D_SameFBO(GridWidth, GridHeight, GridDepth, 1);
    f_Slab.Pressure = CreateSlab3D_SameFBO(GridWidth, GridHeight, GridDepth, 1);
    f_Slab.Temperature = CreateSlab3D_SameFBO(GridWidth, GridHeight, GridDepth, 1);
    f_Surfaces.Divergence = CreateVolume_SameFBO(GridWidth, GridHeight, GridDepth, 3);

    Eulerian3D_Fill->useShader();
    f_Surfaces.Obstacles = CreateVolume_SameFBO(GridWidth, GridHeight, GridDepth, 3);
    CreateObstacles_SameFBO(f_Surfaces.Obstacles);
    ClearSurface(f_Slab.Temperature.Ping, AmbientTemperature);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableVertexAttribArray(SlotPosition);

    Eulerian3D_ApplyBuoyancy->delShader();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





void Smoke::init2()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    myVbos.CubeCenter = CreatePointVbo(0,0,0);
    myVbos.FullscreenQuad = CreateQuadVbo();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableVertexAttribArray(SlotPosition);

    Eulerian3D_ApplyBuoyancy->delShader();
}


void Smoke::update(bool toggle)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    if(true)
    {
        GLuint pID;

        glBindBuffer(GL_ARRAY_BUFFER, myVbos.FullscreenQuad);
        glVertexAttribPointer(SlotPosition, 2, GL_SHORT, GL_FALSE, 2 * sizeof(short), 0);
        glViewport(0, 0, GridWidth, GridHeight);

            pID = Eulerian3D_Advect->getProgramId();
        Advect3D_SameFBO(f_Slab.Velocity.Ping, f_Slab.Velocity.Ping, f_Surfaces.Obstacles, f_Slab.Velocity.Pong, VelocityDissipation, pID);
        SwapSurfaces(&f_Slab.Velocity);

        Advect3D_SameFBO(f_Slab.Velocity.Ping, f_Slab.Temperature.Ping, f_Surfaces.Obstacles, f_Slab.Temperature.Pong, TemperatureDissipation, pID);
        SwapSurfaces(&f_Slab.Temperature);

        Advect3D_SameFBO(f_Slab.Velocity.Ping, f_Slab.Density.Ping, f_Surfaces.Obstacles, f_Slab.Density.Pong, DensityDissipation, pID);
        SwapSurfaces(&f_Slab.Density);

            pID = Eulerian3D_ApplyBuoyancy->getProgramId();
        ApplyBuoyancy3D_SameFBO(f_Slab.Velocity.Ping, f_Slab.Temperature.Ping, f_Slab.Density.Ping, f_Slab.Velocity.Pong, pID);
        SwapSurfaces(&f_Slab.Velocity);

            pID = Eulerian3D_ApplyImpulse->getProgramId();
        ApplyImpulse3D_SameFBO(f_Slab.Temperature.Ping, ImpulsePosition, ImpulseTemperature, pID);
 //       ApplyImpulse3D_SameFBO(f_Slab.Velocity.Ping, ImpulsePosition, ImpulseVelocity);
        ApplyImpulse3D_SameFBO(f_Slab.Density.Ping, ImpulsePosition, ImpulseDensity, pID);

            pID = Eulerian3D_ComputeDivergence->getProgramId();
        ComputeDivergence3D_SameFBO(f_Slab.Velocity.Ping, f_Surfaces.Obstacles, f_Surfaces.Divergence, pID);
        ClearSurface_SameFBO(f_Slab.Pressure.Ping.ColorTexture, 0);

            pID = Eulerian3D_Jacobi->getProgramId();
        for (int i = 0; i < NumJacobiIterations; ++i)
        {
            Jacobi3D_SameFBO(f_Slab.Pressure.Ping, f_Surfaces.Divergence, f_Surfaces.Obstacles, f_Slab.Pressure.Pong, pID);
            SwapSurfaces(&f_Slab.Pressure);
        }

            pID = Eulerian3D_SubtractGradient->getProgramId();
        SubtractGradient3D_SameFBO(f_Slab.Velocity.Ping, f_Slab.Pressure.Ping, f_Surfaces.Obstacles, f_Slab.Velocity.Pong, pID);
        SwapSurfaces(&f_Slab.Velocity);

    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Smoke::show(bool toggle)
{

}


void Smoke::Reset()
{

}




GLuint Smoke::CreateQuadVbo()
{
    short positions[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };
    GLuint vbo;
    GLsizeiptr size = sizeof(positions);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
    return vbo;
}





SlabPod Smoke::CreateSlab3D(GLsizei width, GLsizei height, GLsizei depth, int numComponents)
{
    SlabPod slab;
    slab.Ping = CreateVolume(width, height, depth, numComponents);
    slab.Pong = CreateVolume(width, height, depth, numComponents);
    return slab;
}


SurfacePod Smoke::CreateVolume(GLsizei width, GLsizei height, GLsizei depth, int numComponents)
{
    GLuint fboHandle;
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    GLuint textureHandle;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_3D, textureHandle);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch (numComponents) {
        case 1:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, width, height, depth, 0, GL_RED, GL_HALF_FLOAT, 0);
            break;
        case 2:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RG16F, width, height, depth, 0, GL_RG, GL_HALF_FLOAT, 0);
            break;
        case 3:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F, width, height, depth, 0, GL_RGB, GL_HALF_FLOAT, 0);
            break;
        case 4:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, width, height, depth, 0, GL_RGBA, GL_HALF_FLOAT, 0);
            break;
    }

    if (GL_NO_ERROR != glGetError())
    {
        cout << "Unable to create volume texture" << endl;
        exit(1);
    }

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);
    if (GL_NO_ERROR != glGetError())
    {
        cout << "Unable to attach color buffer" << endl;
        exit(1);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Unable to create FBO" << endl;
        exit(1);
    }

    SurfacePod surface = { fboHandle, textureHandle };

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    surface.Width = width;
    surface.Height = height;
    surface.Depth = depth;
    return surface;
}








void Smoke::CreateObstacles(SurfacePod dest)
{
    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glViewport(0, 0, dest.Width, dest.Height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint lineVbo;
    glGenBuffers(1, &lineVbo);
    GLuint circleVbo;
    glGenBuffers(1, &circleVbo);
    glEnableVertexAttribArray(0);

    for (int slice = 0; slice < dest.Depth; ++slice) {

        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture, 0, dest.Depth - 1 - slice);
        float z = dest.Depth / 2.0f;
        z = abs(slice - z) / z;
        float fraction = 1 - sqrt(z);
        float radius = 0.5f * fraction;

        if (slice == 0 || slice == dest.Depth - 1) {
            radius *= 100;
        }

        const bool DrawBorder = true;
        if (DrawBorder && slice != 0 && slice != dest.Depth - 1)
            {
            #define T 0.9999f
            float positions[] = { -T, -T, T, -T, T,  T, -T,  T, -T, -T };
            #undef T
            GLsizeiptr size = sizeof(positions);
            glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
            glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
            GLsizeiptr stride = 2 * sizeof(positions[0]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
            glDrawArrays(GL_LINE_STRIP, 0, 5);
        }

        const bool DrawSphere = false;
        if (DrawSphere || slice == 0 || slice == dest.Depth - 1)
        {
            const int slices = 64;
            float positions[slices*2*3];
            float twopi = 8*atan(1.0f);
            float theta = 0;
            float dtheta = twopi / (float) (slices - 1);
            float* pPositions = &positions[0];
            for (int i = 0; i < slices; i++) {
                *pPositions++ = 0;
                *pPositions++ = 0;

                *pPositions++ = radius * cos(theta);
                *pPositions++ = radius * sin(theta);
                theta += dtheta;

                *pPositions++ = radius * cos(theta);
                *pPositions++ = radius * sin(theta);
            }

            GLsizeiptr size = sizeof(positions);
            glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
            glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
            GLsizeiptr stride = 2 * sizeof(positions[0]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
            glDrawArrays(GL_TRIANGLES, 0, slices * 3);
        }
    }

    // Cleanup

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &lineVbo);
    glDeleteBuffers(1, &circleVbo);
}

void Smoke::ClearSurface(SurfacePod s, float v)
{
    glBindFramebuffer(GL_FRAMEBUFFER, s.FboHandle);
    glClearColor(v, v, v, v);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Smoke::SwapSurfaces(SlabPod* slab)
{
    SurfacePod temp = slab->Ping;
    slab->Ping = slab->Pong;
    slab->Pong = temp;
}







SlabPod Smoke::CreateSlab3D_SameFBO(GLsizei width, GLsizei height, GLsizei depth, int numComponents)
{
    SlabPod slab;
    slab.Ping = CreateVolume_SameFBO(width, height, depth, numComponents);
    slab.Pong = CreateVolume_SameFBO(width, height, depth, numComponents);
    return slab;
}

SurfacePod Smoke::CreateVolume_SameFBO(GLsizei width, GLsizei height, GLsizei depth, int numComponents)
{
    GLuint textureHandle;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_3D, textureHandle);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch (numComponents) {
        case 1:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, width, height, depth, 0, GL_RED, GL_HALF_FLOAT, 0);
            break;
        case 2:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RG16F, width, height, depth, 0, GL_RG, GL_HALF_FLOAT, 0);
            break;
        case 3:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F, width, height, depth, 0, GL_RGB, GL_HALF_FLOAT, 0);
            break;
        case 4:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, width, height, depth, 0, GL_RGBA, GL_HALF_FLOAT, 0);
            break;
    }
/*
    if (GL_NO_ERROR != glGetError())
    {
        cout << "Unable to create volume texture" << endl;
        exit(1);
    }
*/
    SurfacePod surface;

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    surface.Width = width;
    surface.Height = height;
    surface.Depth = depth;
    surface.ColorTexture = textureHandle;
    return surface;
}

void Smoke::CreateObstacles_SameFBO(SurfacePod dest)
{
    glViewport(0, 0, dest.Width, dest.Height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint lineVbo;
    glGenBuffers(1, &lineVbo);
    GLuint circleVbo;
    glGenBuffers(1, &circleVbo);
    glEnableVertexAttribArray(0);

    for (int slice = 0; slice < dest.Depth; ++slice)
    {

        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture, 0, dest.Depth - 1 - slice);
        float z = dest.Depth / 2.0f;
        z = abs(slice - z) / z;
        float fraction = 1 - sqrt(z);
        float radius = 0.5f * fraction;

        if (slice == 0 || slice == dest.Depth - 1) {
            radius *= 100;
        }

        const bool DrawBorder = true;
        if (DrawBorder && slice != 0 && slice != dest.Depth - 1)
            {
            #define T 0.9999f
            float positions[] = { -T, -T, T, -T, T,  T, -T,  T, -T, -T };
            #undef T
            GLsizeiptr size = sizeof(positions);
            glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
            glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
            GLsizeiptr stride = 2 * sizeof(positions[0]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
            glDrawArrays(GL_LINE_STRIP, 0, 5);
        }

        const bool DrawSphere = true;
        if (DrawSphere || slice == 0 || slice == dest.Depth - 1)
        {
            const int slices = 64;
            float positions[slices*2*3];
            float twopi = 8*atan(1.0f);
            float theta = 0;
            float dtheta = twopi / (float) (slices - 1);
            float* pPositions = &positions[0];
            for (int i = 0; i < slices; i++)
            {
                *pPositions++ = 0;
                *pPositions++ = 0;

                *pPositions++ = radius * cos(theta);
                *pPositions++ = radius * sin(theta);
                theta += dtheta;

                *pPositions++ = radius * cos(theta);
                *pPositions++ = radius * sin(theta);
            }

            GLsizeiptr size = sizeof(positions);
            glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
            glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
            GLsizeiptr stride = 2 * sizeof(positions[0]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
            glDrawArrays(GL_TRIANGLES, 0, slices * 3);
        }
    }

    // Cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &lineVbo);
    glDeleteBuffers(1, &circleVbo);
}

void Smoke::ClearSurface_SameFBO(GLuint tex_id, float v)
{
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_id ,0);
    glClearColor(v, v, v, v);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Smoke::ResetState_SameFBO()
{
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_3D, 0);
    glDisable(GL_BLEND);
}

void Smoke::ResetState()
{
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_3D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
}








#if 1

void Smoke::Advect3D_SameFBO(SurfacePod velocity, SurfacePod source, SurfacePod obstacles, SurfacePod dest, float dissipation, GLuint p)
{
    glUseProgram(p);

    SetUniform(p, "InverseSize", glm::vec3( float(1.0f/float(GridWidth)), float(1.0f/float(GridHeight)), float(1.0f/float(GridDepth))) );
    SetUniform(p, "TimeStep", TimeStep);
    SetUniform(p, "Dissipation", dissipation);
    SetUniform(p, "SourceTexture", 1);
    SetUniform(p, "Obstacles", 2);

    cout << "advecting" << endl;

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture ,0);
    if (GL_NO_ERROR != glGetError())
    {
        cout << "Advect3D Unable to attach color buffer" << endl;
        cout << glGetError() << endl;
    //    exit(1);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Unable to create FBO" << endl;
    }

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_3D, velocity.ColorTexture);
    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_3D, source.ColorTexture);
    glActiveTexture(GL_TEXTURE2);

    glBindTexture(GL_TEXTURE_3D, obstacles.ColorTexture);

    // http://www.opengl.org/wiki/GLAPI/glDrawArraysInstanced
    // glDrawArraysInstanced: behaves identically to glDrawArrays except that primcount
    // instances of the range of elements are executed and the value of internal counter instancedID advances for each iteration
    // instanceID​ is an internal 32-bit integer counter that may be read by a vertex shader as gl_InstanceID​

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState_SameFBO();
}



void Smoke::Jacobi3D_SameFBO(SurfacePod pressure, SurfacePod divergence, SurfacePod obstacles, SurfacePod dest, GLuint p)
{
 //   GLuint p = Eulerian3D_Jacobi->getProgramId();
    glUseProgram(p);

    SetUniform(p, "Alpha", -CellSize * CellSize);
    SetUniform(p, "InverseBeta", 0.1666f);
    SetUniform(p, "Divergence", 1);
    SetUniform(p, "Obstacles", 2);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture ,0);
    if (GL_NO_ERROR != glGetError())
    {
        cout << "Unable to attach color buffer" << endl;
    //    exit(1);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Unable to create FBO" << endl;
    //    exit(1);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, pressure.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, divergence.ColorTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, obstacles.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState_SameFBO();
}




void Smoke::SubtractGradient3D_SameFBO(SurfacePod velocity, SurfacePod pressure, SurfacePod obstacles, SurfacePod dest, GLuint p)
{
 //   GLuint p = Eulerian3D_SubtractGradient->getProgramId();
    glUseProgram(p);

    SetUniform(p, "GradientScale", GradientScale);
    SetUniform(p, "HalfInverseCellSize", 0.5f / CellSize);
    SetUniform(p, "Pressure", 1);
    SetUniform(p, "Obstacles", 2);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture ,0);
    if (GL_NO_ERROR != glGetError())
    {
        cout << "Unable to attach color buffer" << endl;
   //     exit(1);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Unable to create FBO" << endl;
    //    exit(1);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, velocity.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, pressure.ColorTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, obstacles.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState_SameFBO();
}




void Smoke::ComputeDivergence3D_SameFBO(SurfacePod velocity, SurfacePod obstacles, SurfacePod dest, GLuint p)
{
//    GLuint p = Eulerian3D_ComputeDivergence->getProgramId();
    glUseProgram(p);

    SetUniform(p, "HalfInverseCellSize", 0.5f / CellSize);
    SetUniform(p, "Obstacles", 1);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture ,0);
    if (GL_NO_ERROR != glGetError())
    {
        cout << "Unable to attach color buffer" << endl;
     //   exit(1);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Unable to create FBO" << endl;
    //    exit(1);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, velocity.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, obstacles.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState_SameFBO();
}




void Smoke::ApplyBuoyancy3D_SameFBO(SurfacePod velocity, SurfacePod temperature, SurfacePod density, SurfacePod dest, GLuint p)
{
//    GLuint p = Eulerian3D_ApplyBuoyancy->getProgramId();
    glUseProgram(p);

    SetUniform(p, "Temperature", 1);
    SetUniform(p, "Density", 2);
    SetUniform(p, "AmbientTemperature", AmbientTemperature);
    SetUniform(p, "TimeStep", TimeStep);
    SetUniform(p, "Sigma", SmokeBuoyancy);
    SetUniform(p, "Kappa", SmokeWeight);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture ,0);
    if (GL_NO_ERROR != glGetError())
    {
        cout << "Unable to attach color buffer" << endl;
        cout << glGetError() << endl;
   //     exit(1);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Unable to create FBO" << endl;
   //     exit(1);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, velocity.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, temperature.ColorTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, density.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState_SameFBO();
}



void Smoke::ApplyImpulse3D_SameFBO(SurfacePod dest, glm::vec3 position, float value, GLuint p)
{
//    GLuint p = Eulerian3D_ApplyImpulse->getProgramId();
    glUseProgram(p);

    SetUniform(p, "Point", position);
    SetUniform(p, "Radius", SplatRadius);
    SetUniform(p, "FillColor", glm::vec3(value, value, value));

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture ,0);
    if (GL_NO_ERROR != glGetError())
    {
        cout << "Unable to attach color buffer" << endl;
   //     exit(1);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Unable to create FBO" << endl;
 //       exit(1);
    }

    glEnable(GL_BLEND);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState_SameFBO();
}
#endif


GLuint Smoke::CreatePointVbo(float x, float y, float z)
{
    cout << "in CreatePointVbo" << endl;
    float p[] = {x, y, z};
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(p), &p[0], GL_STATIC_DRAW);
    return vbo;
}






void Smoke::SetUniform(GLuint programID, const char* name, int value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform1i(location, value);
}

void Smoke::SetUniform(GLuint programID, const char* name, float value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform1f(location, value);
}

void Smoke::SetUniform(GLuint programID, const char* name, float x, float y)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform2f(location, x, y);
}

void Smoke::SetUniform(GLuint programID, const char* name, glm::vec3 value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform3f(location, value.x, value.y, value.z);
}

void Smoke::SetUniform(GLuint programID, const char* name, glm::mat4 value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}







GLuint Smoke::CreatePyroclasticVolume(int n, float r)
{
    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_3D, handle);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned char *data = new unsigned char[n*n*n];
    unsigned char *ptr = data;

    float frequency = 3.0f / n;
    float center = n / 2.0f + 0.5f;
    srand (time(NULL));

    for(int x=0; x < n; ++x)
	{
        for (int y=0; y < n; ++y)
		{
            for (int z=0; z < n; ++z)
			{
                float dx = center-x;
                float dy = center-y;
                float dz = center-z;

                int rand_n = rand()%(6-2)+2;

/*
                float off = fabsf((float) PerlinNoise3D1(
                    x*frequency,
                    y*frequency,
                    z*frequency,
                    5,
                    6, 3));
*/
                float off = ((float) rand() / (RAND_MAX));
//                float off = 0;
                float d = sqrtf(dx*dx+dy*dy+dz*dz)/(n);
                bool isFilled = (d-off) < r;
                *ptr++ = isFilled ? 255 : 0;
            }
        }
        //PezDebugString("Slice %d of %d\n", x, n);
     //   cout << "Slice " << x << " of " << n << endl;
    }
    int i;
    i = glGetError();
    if(i!=0)
    {
        std::cout << "Error happened while creating the 3D texture: " << i << std::endl;
    }
    glTexImage3D(GL_TEXTURE_3D, 0,
                 GL_LUMINANCE,
                 n, n, n, 0,
                 GL_LUMINANCE,
                 GL_UNSIGNED_BYTE,
                 data);

    delete[] data;
    return handle;
}







