#include "texture.h"

Texture::Texture(GLenum TextureTarget, const string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName = FileName;
}




bool Texture::Load(bool generate)
{
    string filename = m_fileName;

    std::ifstream in(filename.c_str());

    if(!in.is_open())
    {
        std::cout << "Nor oepened1" << std::endl;
        return -1;
    }

    string path = filename.substr(0,(filename.find_last_of('/') != string::npos ?
                                     filename.find_last_of('/')+1 : 0));

    GLuint num;
    glGenTextures(1,&num);
    SDL_Surface* img = IMG_Load(filename.c_str());
    if(img == NULL)
    {
        cout << "Loadtexture img failed" << endl;
        return -1;
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
                                    255
                                   };           // alpha code

    // SDL_SWSURFACE means it's a software surface so we don't store it
    // in the video card
    // we're converting im2 to into this new SDL_PixelFormat format
    SDL_Surface* img2 = SDL_ConvertSurface(img, &pixel_format ,SDL_SWSURFACE);
    if(img2 == NULL)
    {
        cout << "img2 was not loaded" << std :: endl;
        return -1;
    }


    // tell OpenGL we want to use this texture and the texture type/target this texture is
    glBindTexture(GL_TEXTURE_2D,num);       //and use the texture, we have just generated

    //    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,img->w,img->h,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);
    // no else
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);        //we make the actual texture
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_BGR,GL_UNSIGNED_BYTE,img->pixels);        //we make the actual texture


    if(generate)
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); //if the texture is smaller, than the image, we get the avarege of the pixels next to it
    else
        // no need to Magnify the MAG_FILTER
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // openGL 3.0
    if(generate)
        glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(img);   //we delete the image, we don't need it anymore
    SDL_FreeSurface(img2);

    m_textureObj = num;
    return true;
}


void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
