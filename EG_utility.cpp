
#include "EG_Utility.h"


float EG_Utility::DEGREE_TO_RADIAN = 0.0174;
float EG_Utility::RADIAN_TO_DEGREE = 57.32;

EG_Utility::EG_Utility()
{

}


EG_Utility::~EG_Utility()
{

}
/*
void EG_Utility::printGlmVec3(glm::vec3 v)
{
    cout << v.x << " " << v.y << " " << v.z << endl;
}

void EG_Utility::printGlmVec3(string s, glm::vec3 v)
{
    cout << s << endl;
    cout << v.x << " " << v.y << " " << v.z << endl;
}


void EG_Utility::printGlmMat(glm::mat4 m)
{
    for (int i=0; i<4; i++)
        cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << endl;

    cout << endl;
}


void EG_Utility::printGlmMat(string s, glm::mat4 m)
{
    cout << s << endl;
    for (int i=0; i<4; i++)
        cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << endl;

    cout << endl;
}
*/


void EG_Utility::debug(string s, string s2)
{
    cout << s << endl;
    cout << s2 << endl << endl;
}

void EG_Utility::debug(string s, int i)
{
    cout << s << endl;
    cout << i << endl << endl;
}

void EG_Utility::debug(string s, float f)
{
    cout << s << endl;
    cout << f << endl << endl;
}

void EG_Utility::debug(string s, glm::vec2 v)
{
    cout << s << endl;
    cout << v.x << " " << v.y << endl << endl;
}

void EG_Utility::debug(string s, glm::vec3 v)
{
    cout << s << endl;
    cout << v.x << " " << v.y << " " << v.z << endl << endl;
}

void EG_Utility::debug(string s, glm::mat4 m)
{
    cout << s << endl;
    for (int i=0; i<4; i++)
        cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << endl;

    cout << endl << endl;
}

glm::vec3 EG_Utility::toGlmVec(aiVector3D& v2)
{
    glm::vec3 v1;
    v1.x = v2.x;
    v1.y = v2.y;
    v1.z = v2.z;
    return v1;
}

glm::mat4 EG_Utility::toGlmMat(aiMatrix3x3 m2)
{
    glm::mat4 m1;
    m1[0][0] = m2.a1; m1[0][1] = m2.a2; m1[0][2] = m2.a3; m1[0][3] = 0.0f;
    m1[1][0] = m2.b1; m1[1][1] = m2.b2; m1[1][2] = m2.b3; m1[1][3] = 0.0f;
    m1[2][0] = m2.c1; m1[2][1] = m2.c2; m1[2][2] = m2.c3; m1[2][3] = 0.0f;
    m1[3][0] = 0.0f;  m1[3][1] = 0.0f;  m1[3][2] = 0.0f;  m1[3][3] = 1.0f;
    return m1;
}

glm::mat4 EG_Utility::toGlmMat(aiMatrix4x4& m2)
{
    glm::mat4 m1;
    m1[0][0] = m2.a1; m1[0][1] = m2.a2; m1[0][2] = m2.a3; m1[0][3] = m2.a4;
    m1[1][0] = m2.b1; m1[1][1] = m2.b2; m1[1][2] = m2.b3; m1[1][3] = m2.b4;
    m1[2][0] = m2.c1; m1[2][1] = m2.c2; m1[2][2] = m2.c3; m1[2][3] = m2.c4;
    m1[3][0] = m2.d1; m1[3][1] = m2.d2; m1[3][2] = m2.d3; m1[3][3] = m2.d4;
    return m1;
}


glm::mat4 EG_Utility::toGlmMat(const aiMatrix4x4& m2)
{
    glm::mat4 m1;
    m1[0][0] = m2.a1; m1[0][1] = m2.a2; m1[0][2] = m2.a3; m1[0][3] = m2.a4;
    m1[1][0] = m2.b1; m1[1][1] = m2.b2; m1[1][2] = m2.b3; m1[1][3] = m2.b4;
    m1[2][0] = m2.c1; m1[2][1] = m2.c2; m1[2][2] = m2.c3; m1[2][3] = m2.c4;
    m1[3][0] = m2.d1; m1[3][1] = m2.d2; m1[3][2] = m2.d3; m1[3][3] = m2.d4;
    return m1;
}



SDL_Surface* EG_Utility::loadRawImage(string filename)
{
    std::ifstream in(filename.c_str());

    if(!in.is_open())
    {
        std::cout << "filename not found" << std::endl;
        return NULL;
    }

    string path = filename.substr(0,(filename.find_last_of('/') != string::npos ?
            filename.find_last_of('/')+1 : 0));

    SDL_Surface* img = IMG_Load(filename.c_str());
    if(img == NULL)
    {
        cout << "Load image failed" << endl;
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
        cout << "img not converted to SDL img" << std :: endl;
        return NULL;
    }

    SDL_FreeSurface(img);
    return img2;
}



GLuint EG_Utility::loadTexture(string filename)
{
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
    SDL_FreeSurface(img2);
    return num;
}


GLuint EG_Utility::createTexture(int w, int h)
{
    GLuint textureID;
    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);

    setTextureParameters(w, h, GL_RGBA8);
    errorCheck();

    // unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}


GLuint EG_Utility::createDepthTexture(int w, int h)
{
    GLuint textureID;

    glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);

    setTextureParameters(w, h, GL_DEPTH_COMPONENT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    errorCheck();

    // unbind the texture
    glBindTexture(GL_TEXTURE_2D,0);
    return textureID;
}



void EG_Utility::setTextureParameters(int w, int h, int format)
{
   	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_FLOAT, NULL);

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

/*
glm::vec3 EG_Utility::scaleGlmVec3(glm::vec3 v, float s)
{
    return scaleGlmVec3(v,s,s,s);
}

glm::vec3 EG_Utility::scaleGlmVec3(glm::vec3 v, float s1, float s2, float s3)
{
    glm::vec3 newV(s1*v.x, s2*v.y, s3*v.z);
    return newV;
}
*/
glm::vec3 EG_Utility::scaleGlmVec3(const glm::vec3 v, float s)
{
    return scaleGlmVec3(v,s,s,s);
}

glm::vec3 EG_Utility::scaleGlmVec3(const glm::vec3 v, float s1, float s2, float s3)
{
    glm::vec3 newV(s1*v.x, s2*v.y, s3*v.z);
    return newV;
}
