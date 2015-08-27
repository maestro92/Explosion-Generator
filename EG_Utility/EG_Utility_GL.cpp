#include "EG_Utility.h"



void EG_Utility::initSDL(int w, int h, SDL_Surface* & m_displaySurface)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    m_displaySurface = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE|SDL_OPENGL);
    //error check
    if (m_displaySurface == NULL)
    {
        //report error
        cerr << "Could not set up display surface!" << SDL_GetError() << endl;
        exit(1);
    }
}

void EG_Utility::exitSDL(SDL_Surface* & m_displaySurface)
{
    SDL_FreeSurface(m_displaySurface);
    SDL_Quit();
}

void EG_Utility::initGLEW()
{
    // initialize Glew
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      cout << "Error: %s\n" << glewGetErrorString(err) << endl;
    }
}


GLuint EG_Utility::createFBO()
{
    GLuint FBO;
    glGenFramebuffers(1, &FBO);
    return FBO;
}


void EG_Utility::errorCheckFBO()
{
    int i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "XXXXXXXXXX Error: Framebuffer is not OK, status=" << i << endl;
	}
    else
        cout << "Framebuffer Okay" << endl;
}



SDL_Surface* EG_Utility::loadRawImage(string filename)
{
    std::ifstream in(filename.c_str());

    if(!in.is_open())
    {
        std::cout << filename << " not found" << std::endl;
        return NULL;
    }

    string path = filename.substr(0,(filename.find_last_of('/') != string::npos ?
            filename.find_last_of('/')+1 : 0));

    SDL_Surface* img = IMG_Load(filename.c_str());
    if(img == NULL)
    {
        cout << filename << " Load image failed" << endl;
        return NULL;
    }

    return img;
}


SDL_Surface* EG_Utility::loadSDLImage(string filename)
{
    SDL_Surface* img = loadRawImage(filename);

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
        cout << "img not converted to SDL img " << filename << endl;
        return NULL;
    }

    SDL_FreeSurface(img);
    return img2;
}



GLuint EG_Utility::loadTexture(string filename)
{
    cout << "Loading Texture " << filename << endl;

    SDL_Surface* img2 = loadSDLImage(filename);

    unsigned int num;
    glGenTextures(1,&num);

    // tell OpenGL we want to use this texture
    glBindTexture(GL_TEXTURE_2D,num);       //and use the texture, we have just generated
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img2->w,img2->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);        //we make the actual texture

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // if you comment these two lines out, you will see the edges of the cube
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);      //we do it for vertically and horizontally (previous line)

    //we delete the image, we don't need it anymore
    glBindTexture(GL_TEXTURE_2D,0);
    SDL_FreeSurface(img2);
    return num;
}


GLuint EG_Utility::createTexture(int w, int h)
{
    GLuint textureID;
    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);

    setTextureParameters(w, h, GL_RGBA8, GL_RGBA);
    errorCheck();

    // unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}

/*
GLuint EG_Utility::create3DTexture(int w, int h, int d)
{
    GLuint textureID;
    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);



    return textureID;
}
*/



GLuint EG_Utility::createDepthTexture(int w, int h)
{
    GLuint textureID;

    glGenTextures(1,&textureID);
    /// glBindTexture: i'm binding this texture object as the current 2D texture
	glBindTexture(GL_TEXTURE_2D,textureID);

    setTextureParameters(w, h, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    errorCheck();

    // unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}



void EG_Utility::setTextureParameters(int w, int h, int internal_format, int format)
{
   	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}



GLuint EG_Utility::createCubemapTexture()
{
    /// need to pass in the pictures in the following order
    /// left, right, top, bottom, near, far
    GLuint tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    // set textures
    /// SCREEN_WIDTH and SCREEN_HEIGHT needs to be power of two
    for (int i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    /// this all applies to the one cubemap
    setCubemapTextureParameters();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return tex;
}


GLuint EG_Utility::createCubemapTexture(string* filenames)
{
    /// need to pass in the pictures in the following order
    /// left, right, top, bottom, near, far
    GLuint tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    for(int i=0; i<6; i++)
    {
        SDL_Surface* cubemap_img = loadSDLImage(filenames[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,
                        GL_RGBA, cubemap_img->w, cubemap_img->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, cubemap_img->pixels);

        SDL_FreeSurface(cubemap_img);
    }

    /// this all applies to the one cubemap
    setCubemapTextureParameters();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return tex;
}


void EG_Utility::setCubemapTextureParameters()
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void EG_Utility::errorCheck()
{
    int i;
    i = glGetError();
    if(i!=0)
    {
        std::cout << "Error happened while loading the texture: " << i << std::endl;
    }
}




EG_SurfacePod EG_Utility::createSurface(int width, int height)
{
    EG_SurfacePod pod;

    pod.depthTexture = createDepthTexture(width, height);

    // Create a FBO and attach the depth texture:
    glGenFramebuffers(1, &pod.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pod.FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pod.depthTexture, 0);

    pod.colorTexture = createTexture(width, height);

    // Attach the color buffer:
    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pod.colorTexture, 0);

    return pod;
}



void EG_Utility::setupFrameBuffer()
{
    setupFrameBuffer(0);
}

void EG_Utility::setupFrameBuffer(GLuint target)
{
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
