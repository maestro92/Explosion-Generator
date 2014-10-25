#ifndef SHADER_H_
#define SHADER_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
class shader{
    unsigned int vs, fs, program;
    void loadFile(const char* fn, string & str);
    unsigned int loadShader(string& source, unsigned int shaderType);

public:
    shader(const char* vs_source, const char* fs_source);
    ~shader();
    void useShader();
    unsigned int getProgramId();
    void delShader();

};


#endif // SHADER_H_
