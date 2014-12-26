#ifndef SHADER_H_
#define SHADER_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>

enum AttributeSlot {
    SlotPosition,
    SlotTexCoord,
};


using namespace std;
class Shader{
    unsigned int vs, fs, program;
    unsigned int gs;
    string path;
    void loadFile(const char* fn, string & str);
    unsigned int loadShader(string& source, unsigned int ShaderType);



    public:

        Shader(const char* vs_source, const char* fs_source);
        Shader(const char* vs_source, const char* gs_source, const char* fs_source);
        ~Shader();
        void linkShader();
        void useShader();
        unsigned int getProgramId();
        void delShader();

    protected:

    private:

};


#endif // Shader_H_

