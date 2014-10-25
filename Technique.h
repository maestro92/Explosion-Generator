#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include "shader.h"
using namespace std;

class Technique
{
    public:
        Technique();
        virtual ~Technique();

        virtual void init();
  //      virtual void Load();

        void EnableShader();
        void DisableShader();

    protected:
        shader* Shader;
};


#endif
