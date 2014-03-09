
#include "ParticleSystemPCH.h"


#include "ParticleEmitter.h"
#include "camera.h"
#include "Random.h"
#include "ParticleEffect.h"

#include "gl/gl.h"
#include "gl/glu.h"
#include "iostream"

using namespace std;

glm::vec3 player_pos;

ParticleEffect::ParticleEffect( unsigned int numParticles /* = 0 */ )
: m_pCamera( NULL )
, m_pParticleEmitter( NULL )
, m_ColorInterpolator( glm::vec4(1) )
, m_LocalToWorldMatrix(1)
, m_TextureID(0)
, m_Force( 0, -9.81f, 0 )	// set it as gravity
{
    Resize(numParticles);
//    position.change(-50,4,0);
//    rotation.change(0,0,0);
}

ParticleEffect::~ParticleEffect()
{
    if ( m_TextureID != 0 )
    {
        glDeleteTextures( 1, &m_TextureID );
        m_TextureID = 0;
    }
}

void ParticleEffect::SetCamera( Camera* pCamera )
{
    m_pCamera = pCamera;
}

// in the main program, a SphereEmitter is passed here
void ParticleEffect::SetParticleEmitter( ParticleEmitter* pEmitter )
{
    m_pParticleEmitter = pEmitter;
}


// colors is like a vector of colors here
// so a vector of color will be interpolated
void ParticleEffect::SetColorInterplator( const ColorInterpolator& colors )
{
    m_ColorInterpolator = colors;
}




Uint32 transparentColor1;




unsigned int ParticleEffect::loadTexture(string filename, int background, int generate)
{

    std::ifstream in(filename.c_str());

    if(!in.is_open())
    {
        std::cout << "Nor oepened" << std::endl;
        return -1;
    }

    string path = filename.substr(0,(filename.find_last_of('/') != string::npos ?
            filename.find_last_of('/')+1 : 0));

    unsigned int num;
    glGenTextures(1,&num);
    SDL_Surface* img = IMG_Load(filename.c_str());
    if(img == NULL)
    {
        cout << "Loadtexture img failed" << endl;
        return -1;
    }

    for(int i =0; i<100; i++)
    {
        //for
        transparentColor1 = SDL_MapRGB(img->format, 0, 0, 0);
        SDL_SetColorKey(img, SDL_SRCCOLORKEY, transparentColor1);
    }



    SDL_PixelFormat pixel_format = {NULL,
                                    32,             // converting to 32 bit pixel
                                    4,              // number of bytes
                                    0,0,0,0,        // byte lost
                                    0,0,0,0,        // byte shift
                                    0xff000000,     // red mask
                                    0x00ff0000,     // green mask
                                    0x0000ff00,     // blue mask
                                    0x000000ff,     // alpha mask
                                    0,              // color code
                                    255};           // alpha code

    // SDL_SWSURFACE means it's a software surface so we don't store it
    // in the video card
    // we're converting im2 to into this new SDL_PixelFormat format
    SDL_Surface* img2 = SDL_ConvertSurface(img, &pixel_format ,SDL_SWSURFACE);
    if(img2 == NULL)
    {
        cout << "img2 was not loaded" << std :: endl;
        return -1;
    }




    // tell OpenGL we want to use this texture
    glBindTexture(GL_TEXTURE_2D,num);       //and use the texture, we have just generated


    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);        //we make the actual texture
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_BGR,GL_UNSIGNED_BYTE,img->pixels);        //we make the actual texture


    if(generate)
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); //if the texture is smaller, than the image, we get the avarege of the pixels next to it
    else
        // no need to Magnify the MAG_FILTER
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    // if you comment these two lines out, you will see the edges of the cube
    if(background)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);      //we do it for vertically and horizontally (previous line)
    }

    // openGL 3.0
    if(generate)
        glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(img);   //we delete the image, we don't need it anymore
    SDL_FreeSurface(img2);
    return num;
}




unsigned int ParticleEffect::loadTexture_Normal(string filename, int background, int generate)
{

    std::ifstream in(filename.c_str());

    if(!in.is_open())
    {
        std::cout << "Nor oepened" << std::endl;
        return -1;
    }

    string path = filename.substr(0,(filename.find_last_of('/') != string::npos ?
            filename.find_last_of('/')+1 : 0));

    unsigned int num;
    glGenTextures(1,&num);
    SDL_Surface* img = IMG_Load(filename.c_str());
    if(img == NULL)
    {
        cout << "Loadtexture img failed" << endl;
        return -1;
    }

    for(int i =0; i<100; i++)
    {
        //for
        transparentColor1 = SDL_MapRGB(img->format, 0, 0, 0);
        SDL_SetColorKey(img, SDL_SRCCOLORKEY, transparentColor1);
    }



    SDL_PixelFormat pixel_format = {NULL,
                                    32,             // converting to 32 bit pixel
                                    4,              // number of bytes
                                    0,0,0,0,        // byte lost
                                    0,0,0,0,        // byte shift
                                    0xff000000,     // red mask
                                    0x00ff0000,     // green mask
                                    0x0000ff00,     // blue mask
                                    0x000000ff,     // alpha mask
                                    0,              // color code
                                    255};           // alpha code

    // SDL_SWSURFACE means it's a software surface so we don't store it
    // in the video card
    // we're converting im2 to into this new SDL_PixelFormat format
    SDL_Surface* img2 = SDL_ConvertSurface(img, &pixel_format ,SDL_SWSURFACE);
    if(img2 == NULL)
    {
        cout << "img2 was not loaded" << std :: endl;
        return -1;
    }




    // tell OpenGL we want to use this texture
    glBindTexture(GL_TEXTURE_2D,num);       //and use the texture, we have just generated


    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);        //we make the actual texture
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_BGR,GL_UNSIGNED_BYTE,img->pixels);        //we make the actual texture


    if(generate)
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); //if the texture is smaller, than the image, we get the avarege of the pixels next to it
    else
        // no need to Magnify the MAG_FILTER
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    // if you comment these two lines out, you will see the edges of the cube
    if(background)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);      //we do it for vertically and horizontally (previous line)
    }

    // openGL 3.0
    if(generate)
        glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(img);   //we delete the image, we don't need it anymore
    SDL_FreeSurface(img2);
    return num;
}




// loads a texture into m_textureID, which is a member variable
bool ParticleEffect::LoadTexture( const std::string& fileName )
{
    if ( m_TextureID != 0 )
    {
        glDeleteTextures(1, &m_TextureID );
    }

//    m_TextureID = SOIL_load_OGL_texture( fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS );
    m_TextureID = loadTexture(fileName);

    return ( m_TextureID != 0 );
}


// ramdomly distribute a single particle on a unit circle centered
// at the origin of the particle effect
void ParticleEffect::RandomizeParticle( Particle& particle )
{
    particle.m_fAge = 0.0f;
	// random number between 3 and 5 seconds
    particle.m_fLifeTime = RandRange( 3, 5 );

	// stores a random vector distributed onto a unit sphere
    glm::vec3 unitVec = RandUnitVec();

    particle.m_Position = unitVec * 1.0f;
    particle.m_Velocity = unitVec * RandRange( 10, 20 );
}

void ParticleEffect::RandomizeParticles()
{
	// randomize every particle
    for ( unsigned int i = 0; i < m_Particles.size(); ++i )
    {
        RandomizeParticle(m_Particles[i]);
    }
}

// if it has a reference to a valid ParticleEmitter class instance
// then we emit it using the EmitParticle method
void ParticleEffect::EmitParticle( Particle& particle)
{
	// abort is called if (m_pParticleEmitter != NULL) is false
    assert( m_pParticleEmitter != NULL );
    m_pParticleEmitter->EmitParticle( particle);
}

void ParticleEffect::EmitParticles()
{
    if ( m_pParticleEmitter == NULL )
    {
        RandomizeParticles();
    }
    else
    {
        for ( unsigned int i = 0; i < m_Particles.size(); ++i )
        {
            EmitParticle(m_Particles[i]);
        }
    }
}



// this modifies the vertex buffer to render the particle effect
// with scree-aligned quads
void ParticleEffect::BuildVertexBuffer()
{
    const glm::vec3 X( 0.5, 0, 0 );
    const glm::vec3 Y( 0, 0.5, 0 );
    const glm::vec3 Z( 0, 0 ,1.0 );

    glm::quat cameraRotation;

    if ( m_pCamera != NULL )
    {
        cameraRotation = glm::quat( glm::radians(m_pCamera->GetRotation()) );
    }

    // Make sure the vertex buffer has enough vertices to render the effect
    // If the vertex buffer is already the correct size, no change is made.
    m_VertexBuffer.resize(m_Particles.size() * 4, Vertex() );


    for ( unsigned int i = 0; i < m_Particles.size(); ++i )
    {
        Particle& particle = m_Particles[i];
        glm::quat rotation = glm::angleAxis( particle.m_fRotate, Z );

        unsigned int vertexIndex = i * 4;
        Vertex& v0 = m_VertexBuffer[vertexIndex + 0];   // Bottom-left
        Vertex& v1 = m_VertexBuffer[vertexIndex + 1];   // Bottom-right
        Vertex& v2 = m_VertexBuffer[vertexIndex + 2];   // Top-right
        Vertex& v3 = m_VertexBuffer[vertexIndex + 3];   // Top-left

        // Bottom-left
        v0.m_Pos = particle.m_Position + ( rotation * ( -X - Y ) * particle.m_fSize ) * cameraRotation;
        v0.m_Tex0 = glm::vec2( 0, 1 );
        v0.m_Diffuse = particle.m_Color;

        // Bottom-right
        v1.m_Pos = particle.m_Position + ( rotation * ( X - Y ) * particle.m_fSize ) * cameraRotation;
        v1.m_Tex0 = glm::vec2( 1, 1 );
        v1.m_Diffuse = particle.m_Color;

        // Top-right
        v2.m_Pos = particle.m_Position + ( rotation * ( X + Y ) * particle.m_fSize ) * cameraRotation;
        v2.m_Tex0 = glm::vec2( 1, 0 );
        v2.m_Diffuse = particle.m_Color;

        // Top-left
        v3.m_Pos = particle.m_Position + ( rotation * ( -X + Y ) * particle.m_fSize ) * cameraRotation;
        v3.m_Tex0 = glm::vec2( 0, 0 );
        v3.m_Diffuse = particle.m_Color;
    }

}




// used to update the positions, velocities, colors, rotations, sizes and age of the particle
void ParticleEffect::Update(float fDeltaTime)
{
	// looping through all particles
    for ( unsigned int i = 0; i < m_Particles.size(); ++i )
    {
        Particle& particle = m_Particles[i];

        particle.m_fAge += fDeltaTime;

        if ( particle.m_fAge > particle.m_fLifeTime )
        {
            if ( m_pParticleEmitter != NULL )
				EmitParticle(particle);
            else
				// this doesn't really get called
				RandomizeParticle(particle);
        }

       // float lifeRatio = glm::saturate(particle.m_fAge / particle.m_fLifeTime);
		float lifeRatio = particle.m_fAge / particle.m_fLifeTime;

		particle.m_Velocity += ( m_Force * fDeltaTime );
        particle.m_Position += ( particle.m_Velocity * fDeltaTime );
        particle.m_Color = m_ColorInterpolator.GetValue( lifeRatio );
        particle.m_fRotate = glm::lerp<float>( 0.0f, 720.0f, lifeRatio );
        particle.m_fSize = glm::lerp<float>( 5.0f, 0.0f, lifeRatio );
    }

    BuildVertexBuffer();
}






void ParticleEffect::Render()
{

    //glDisable(GL_DEPTH_TEST);           // Disables Depth Testing
/*
    glEnable(GL_BLEND);                 // Enable Blending
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   // Type Of Blending To Perform
    glEnable(GL_TEXTURE_2D);            // Enable textures
*/
    glPushMatrix();
		glMultMatrixf( glm::value_ptr(m_LocalToWorldMatrix) );


        {

          glDisable(GL_DEPTH_TEST);           // Disables Depth Testing
            glEnable(GL_TEXTURE_2D);            // Enable textures
         //   glShadeModel(GL_FLAT);
            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);                 // Enable Blending
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        }


		glBindTexture( GL_TEXTURE_2D, m_TextureID );

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );

		// tells openGL to point to a vertex array
		// number of elements, type(GL_FLOAT), size of vertex
		glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Pos) );
		glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Tex0) );
		glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Diffuse) );

		glDrawArrays( GL_QUADS, 0, m_VertexBuffer.size() );
	//	cout << "vertex size is" << m_VertexBuffer.size() << endl;

		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );

		glBindTexture( GL_TEXTURE_2D, 0 );

	#if _DEBUG
		if ( m_pParticleEmitter != NULL )
		{
			m_pParticleEmitter->DebugRender();
		}
	#endif

    glPopMatrix();
/*
 //   vector3d direction;
 //   direction.change(player_center - position);
 //   direction.normalize();

 //   rotation.y = acos(direction.z);
 //   if(direction.x >0)
 //       rotation.y = -rotation.y;

//    cout << "ParticleEffect rotation " << rotation.x*180/M_PI << " " << rotation.y*180/M_PI << " " << rotation.z*180/M_PI << " " << endl;


//    vector3d direction;
//    direction.change(player_center - vector3d(20,0,0));
//    direction.normalize();

//    cout << "rotation.y" << rotation.y << endl;

    glDisable(GL_DEPTH_TEST);           // Disables Depth Testing

    glEnable(GL_BLEND);                 // Enable Blending
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   // Type Of Blending To Perform
    glEnable(GL_TEXTURE_2D);            // Enable textures



    glPushMatrix();
		glMultMatrixf( glm::value_ptr(m_LocalToWorldMatrix) );



    //    glDisable(GL_DEPTH_TEST);           // Disables Depth Testing
        glEnable(GL_TEXTURE_2D);            // Enable textures
     //   glShadeModel(GL_FLAT);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);                 // Enable Blending
        glEnable(GL_ALPHA_TEST);


        // http://www.opengl.org/wiki/Transparency_Sorting

//        if(type == 0)
//            glAlphaFunc(GL_GREATER, 0);
//        else
            glAlphaFunc(GL_GREATER, 0);
    //    glBlendFunc(GL_ONE, GL_ONE);
        //
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   // Type Of Blending To Perform




//GL_SRC_ALPHA, GL_ONE for additive blending
//GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA for real transparency
//GL_ONE, GL_ONE for additive blending without alpha affecting the result


		glBindTexture( GL_TEXTURE_2D, m_TextureID );

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );

		// tells openGL to point to a vertex array
		// number of elements, type(GL_FLOAT), size of vertex
		glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Pos) );
		glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Tex0) );
		glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Diffuse) );

   //     glTranslatef(position.x-player_center.x,position.y-player_center.y,position.z-player_center.z);


  //      glTranslatef(position.x+offset.x,position.y+offset.y,position.z+offset.z);



  //
  //      glRotatef(20-player_center.x,1,0,0);
 //       glRotatef(acos(rotation.z),0,1,0);
    //    glRotatef(rotation.x*(180.0/M_PI), 1,0,0);
   //     glRotatef(-rotation.y*(180.0/M_PI), 0,1,0);
  //      glRotatef(rotation.y, 0,1,0);
  //      glRotatef(rotation.z*(180.0/M_PI), 0,0,1);
  //      glRotatef(20-player_center.x,0,0,1);


     //   glScalef(0.2,0.2,0.2);
//		if(type == 0)
//            glScalef(2,2,2);
//        else
            glScalef(2,2,2);

		glDrawArrays( GL_QUADS, 0, m_VertexBuffer.size() );
		cout << "vertex size is" << m_VertexBuffer.size() << endl;

		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );

		glBindTexture( GL_TEXTURE_2D, 0 );

	#if _DEBUG
		if ( m_pParticleEmitter != NULL )
		{
			m_pParticleEmitter->DebugRender();
		}
	#endif
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   // Type Of Blending To Perform
*/
}

void ParticleEffect::Resize( unsigned int numParticles )
{
	// resizes the particle so it contains n elements
	// the default Particle() constructor makes everything to 0
	m_Particles.resize( numParticles,  Particle() );

	// the default Vertex() constructor makes everything to 0 as well
	m_VertexBuffer.resize( numParticles * 4, Vertex() );
}
